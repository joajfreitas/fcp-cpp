#ifndef __FCP_H__
#define __FCP_H__

#include <stdlib.h>
#include <string.h>
#include <map>
#include <iostream>

#include "fcp_common.hpp"
#include "fcp_device.hpp"
#include "fcp_log.hpp"
#include "fcp_message.hpp"
#include "fcp_config.hpp"
#include "fcp_command.hpp"


#include "json.hpp"

using json = nlohmann::json;

#define FILE_MAX_SIZE 1048576

#define GET_DEV_ID(sid) ((sid) & 0x1F)
#define GET_MSG_ID(sid) (((sid) >> 5) & 0x3F)
#define GET_SID(dev_id, msg_id) (((msg_id) << 5) + (dev_id))

#include "candata.h"

class Fcp {
	public:
		std::map<std::string, FcpDevice> devices;
		std::map<std::string, FcpLog> logs;

		FcpDevice common;

		void decompile_file(std::string filename);
		void decompile(json j);

		std::pair<std::string, std::map<std::string, double>> decode_msg(CANdata);
		CANdata encode_msg(std::string dev_id, std::string msg_id, std::map<std::string, double> signals);
		CANdata encode_cmd(std::string dev_id, std::string cmd_id, std::string dst_id, uint16_t arg1, uint16_t arg2, uint16_t arg3);
		std::string get_dev_name(uint16_t sid);
		std::string get_msg_name(uint16_t sid);


	private:
		std::map<int, FcpMessage *> msgs;
		std::map<int, FcpMessage *> common_msgs;
		std::map<int, std::string > dev_names;
};

#endif
