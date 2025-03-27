#ifndef __FCP_H__
#define __FCP_H__

#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <stdlib.h>
#include <string.h>

#include "fcp_command.hpp"
#include "fcp_common.hpp"
#include "fcp_config.hpp"
#include "fcp_device.hpp"
#include "fcp_log.hpp"
#include "fcp_message.hpp"
#include <yaml-cpp/yaml.h>

#include "json.hpp"

using json = nlohmann::json;

#define FILE_MAX_SIZE 1048576

#define GET_DEV_ID(sid) ((sid)&0x1F)
#define GET_MSG_ID(sid) (((sid) >> 5) & 0x3F)
#define GET_SID(dev_id, msg_id) (((msg_id) << 5) + (dev_id))

#include "candata.h"

struct badJsonFile : std::exception {
    const char* what() const noexcept { return "error_reading"; }
};

class Fcp {
public:
    std::map<std::string, FcpDevice> devices;
    std::map<std::string, FcpLog> logs;

    FcpDevice common;

    void decompile_file(std::string filename);
    void decompile(json j);

    std::pair<std::string, std::map<std::string, double>> decode_msg(CANdata);
    std::optional<CANdata> encode_msg(std::string dev_name, std::string msg_name, std::map<std::string, double> signals_list);
    CANdata encode_cmd(std::string dev_name, std::string cmd_name, std::string dst_id, uint16_t arg1, uint16_t arg2, uint16_t arg3);

    std::string get_dev_name(uint32_t sid);

    template <typename T>
    std::string get_msg_name(T sid)
    {
        FcpDevice* dev = get_device(CANdata::get_dev_id(sid));
        if (dev == NULL) {
            return "";
        }
        T msg_id = CANdata::get_msg_id(sid);

        if (dev->msgs.count(msg_id)) {
            return dev->msgs[msg_id]->name;
        } else {
            return "";
        }
    }

    template <typename T>
    FcpMessage* get_msg(T sid)
    {
        if (msgs.count(sid)) {
            return msgs[sid];
        } else {
            return nullptr;
        }
    }

    FcpMessage* get_msg(uint16_t sid);
    std::map<int, FcpMessage*> get_msgs(void);

    std::pair<std::string, FcpSignal*> get_signal(std::string name);
    FcpDevice* get_device(int deviceIdentifier);

private:
    std::map<int, FcpMessage*> msgs;
    std::map<int, FcpMessage*> common_msgs;
    std::map<int, std::string> dev_names;
};

#endif
