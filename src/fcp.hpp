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
using SignalMap = std::map<std::string, double>;
using DecodedMsg = std::pair<std::string, SignalMap>;

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

    void decompile_file(std::string filename);
    void decompile(json j);

    DecodedMsg decode_msg(CanData);
    std::optional<CanData> encode_msg(std::string dev_name, std::string msg_name, SignalMap signal_map);
    std::optional<CanData> encode_cmd(std::string dev_name, std::string cmd_name, std::string dst_id, uint16_t arg1, uint16_t arg2, uint16_t arg3);

    std::string get_dev_name(uint32_t sid);
    std::string get_msg_name(uint32_t sid);

    std::map<int, FcpMessage*> get_msgs(void);

    std::pair<std::string, FcpSignal*> get_signal(std::string name);
    FcpDevice* get_device(int deviceIdentifier);
    FcpDevice* get_common();

    FcpMessage* get_msg(uint32_t sid);

private:
    FcpDevice common;

    std::map<int, FcpMessage*> msgs;
    std::map<int, FcpMessage*> common_msgs;
    std::map<int, std::string> dev_names;
};

#endif
