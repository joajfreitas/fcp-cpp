#ifndef __FCP_DEVICE_H__
#define __FCP_DEVICE_H__

#include <stdlib.h>

#include "fcp_config.hpp"
#include "fcp_command.hpp"
#include "fcp_message.hpp"

#include "json.hpp"

using json = nlohmann::json;

class FcpDevice {
	public:
		int id;
		std::string name;
		std::map<std::string, FcpMessage *> msgs;
		std::map<std::string, FcpCommand> cmds;
		std::map<std::string, FcpConfig> cfgs;
		
		std::map<int, FcpMessage*> decompile(json j);

		std::string get_cmd_name(int cmd_id);
	
	private:
		std::map<int, std::string> cmd_names;
};

#endif
