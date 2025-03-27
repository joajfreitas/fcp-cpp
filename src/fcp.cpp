#include <fstream>
#include <iostream>

#include <stdint.h>
#include <stdlib.h>
#include <type_traits>
#include <optional>

#include "fcp.hpp"
#include "fcp_device.hpp"
#include "fcp_message.hpp"

#include "json.hpp"

using json = nlohmann::json;

void Fcp::decompile_file(std::string filename)
{
    char buffer[FILE_MAX_SIZE] = {0};
    std::ifstream file;
    file.open(filename);

    file.read(buffer, FILE_MAX_SIZE);
    if (buffer[0] == 0)
    {
        throw badJsonFile();
    }
    file.close();

    auto j = json::parse(buffer);
    this->decompile(j);

    return;
}

void Fcp::decompile(json j)
{
    std::map<int, FcpMessage *> _msgs;
    for (auto &el : j["devices"].items())
    {
        FcpDevice fcp_dev;
        _msgs = fcp_dev.decompile(el.value());

        this->msgs.insert(_msgs.begin(), _msgs.end());

        this->devices[el.key()] = fcp_dev;
        this->dev_names[fcp_dev.id] = fcp_dev.name;
    }

    _msgs = this->common.decompile(j["common"]);
    this->common_msgs.insert(_msgs.begin(), _msgs.end());
    this->dev_names[0] = "common";

    for (auto &el : j["logs"].items())
    {
        FcpLog fcp_log;
        fcp_log.decompile(el.value());

        this->logs[el.key()] = fcp_log;
    }

    return;
}

std::pair<std::string, std::map<std::string, double>> Fcp::decode_msg(CANdata msg)
{
    uint32_t sid = msg.get_message_id();

    if (this->msgs.count(sid))
    {
        return this->msgs.at(sid)->decode_msg(msg);
    }
    else if (this->common_msgs.count(sid))
    {
        return this->common_msgs.at(sid)->decode_msg(msg);
    }
    else
    {
        throw std::exception();
    }
}

std::optional<CANdata> Fcp::encode_msg(std::string dev_name, std::string msg_name, std::map<std::string, double> signals_list)
{
    if (dev_name == "common")
    {
        for (auto const &[key, val] : this->common_msgs)
        {
            if (val->name == msg_name)
            {
                return common_msgs[key]->encode_msg(signals_list, 0);
            }
        }
    }
    else
    {
        if (devices.count(dev_name) == 0 || devices[dev_name].msgs.count(msg_name) == 0) {
            throw std::exception();
        }

        return this->devices[dev_name].msgs[msg_name]->encode_msg(signals_list, this->devices[dev_name].id);
    }

    return std::nullopt;
}

CANdata Fcp::encode_cmd(std::string dev_id, std::string cmd_id, std::string dst_id, uint16_t arg1, uint16_t arg2, uint16_t arg3)
{

    uint16_t _dst_id = this->devices[dst_id].id;
    uint16_t _cmd_id = this->devices[dst_id].cmds[cmd_id].id;

    CANdata msg = this->common.msgs["send_cmd"]->encode_msg({{"id", _cmd_id}, {"dst", _dst_id}, {"arg1", arg1}, {"arg2", arg2}, {"arg3", arg3}}, this->devices[dev_id].id);
    msg.id.sid = CANdata::make_id(this->devices[dev_id].id, 1);

    return msg;
}

std::string Fcp::get_dev_name(uint32_t sid)
{
    int dev_id = CANdata::get_dev_id(sid);
    int count = this->dev_names.count(dev_id);

    if (count)
        return this->dev_names[dev_id];
    else
        return "";
}

std::pair<std::string, FcpSignal *> Fcp::get_signal(std::string name)
{
    for (auto dev : this->devices)
    {
        for (auto msg : dev.second.msgs)
        {
            for (auto signal : msg.second->signals_list)
            {
                if (name.find(signal.first) != std::string::npos)
                {
                    return std::make_pair(signal.first, &msg.second->signals_list[signal.first]);
                }
            }
        }
    }

    return std::make_pair("", nullptr);
}

FcpDevice *Fcp::get_device(int deviceIdentifier)
{

    for (auto dev : this->devices)
    {
        if (dev.second.id == deviceIdentifier)
        {
            return &this->devices[dev.first];
        }
    }
    return NULL;
}

std::map<int, FcpMessage*> Fcp::get_msgs()
{
    return msgs;
}
