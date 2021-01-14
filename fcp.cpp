#include <fstream>
#include <iostream>

#include <stdint.h>
#include <stdlib.h>

#include "fcp.hpp"
#include "fcp_device.hpp"
#include "fcp_message.hpp"

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

void Fcp::decompile_file(std::string filename)
{
    char buffer[FILE_MAX_SIZE];
    std::ifstream file;
    file.open(filename);

    file.read(buffer, FILE_MAX_SIZE);
    file.close();

    auto j = json::parse(buffer);

    this->decompile(j);

    return;
}

void Fcp::decompile(json j)
{
    std::map<int, FcpMessage*> _msgs;
    for (auto& el : j["devices"].items()) {
        FcpDevice fcp_dev;
        _msgs = fcp_dev.decompile(el.value());

        this->msgs.insert(_msgs.begin(), _msgs.end());

        this->devices[el.key()] = fcp_dev;
        this->dev_names[fcp_dev.id] = fcp_dev.name;
    }

    _msgs = this->common.decompile(j["common"]);
    this->common_msgs.insert(_msgs.begin(), _msgs.end());
    this->dev_names[0] = "common";

    for (auto& el : j["logs"].items()) {
        FcpLog fcp_log;
        fcp_log.decompile(el.value());

        this->logs[el.key()] = fcp_log;
    }

    return;
}

std::pair<std::string, std::map<std::string, double>> Fcp::decode_msg(CANdata msg)
{
    if (this->msgs.count(msg.sid)) {
        return this->msgs.at(msg.sid)->decode_msg(msg);
    }
    else if (this->common_msgs.count(msg.sid)) {
        auto msg_handle = this->common_msgs.at(msg.sid);
        auto decoded = msg_handle->decode_msg(msg);
        decoded.first = this->devices[this->get_dev_name(decoded.second["dst"])].get_cmd_name(decoded.second["id"]);
        return decoded;
    }
    else {
        throw std::exception();
    }
}

CANdata Fcp::encode_msg(std::string dev_id, std::string msg_id, std::map<std::string, double> signals)
{

    CANdata msg = this->devices[dev_id].msgs[msg_id]->encode_msg(signals);
    msg.sid = GET_SID(this->devices[dev_id].id, this->devices[dev_id].msgs[msg_id]->id);
    return msg;
}

CANdata Fcp::encode_cmd(std::string dev_id, std::string cmd_id, std::string dst_id, uint16_t arg1, uint16_t arg2, uint16_t arg3)
{

    CANdata msg;

    uint16_t _dst_id = this->devices[dst_id].id;
    uint16_t _cmd_id = this->devices[dst_id].cmds[cmd_id].id;

    msg = this->common.msgs["send_cmd"]->encode_msg({ { "id", _cmd_id }, { "dst", _dst_id }, { "arg1", arg1 }, { "arg2", arg2 }, { "arg3", arg3 } });
    msg.sid = GET_SID(this->devices[dev_id].id, 1);

    return msg;
}

std::string Fcp::get_dev_name(uint16_t sid)
{
    int dev_id = GET_DEV_ID(sid);
    int count = this->dev_names.count(dev_id);

    if (count)
        return this->dev_names[dev_id];
    else
        return "";
}

std::string Fcp::get_msg_name(uint16_t sid)
{

    int count = this->msgs.count(sid);
    if (count) {
        return this->msgs[sid]->name;
    } else {
        int msg_id = GET_MSG_ID(sid);
        int common_sid = GET_SID(0, msg_id);
        count = this->common_msgs.count(common_sid);
        if (count)
            return this->common_msgs[common_sid]->name;
        else
            return "";
    }
}
