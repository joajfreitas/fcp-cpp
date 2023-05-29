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
        void setCmdNames(std::pair<int, std::string>);
		
		std::map<int, FcpMessage*> decompile(json j);

		std::string get_cmd_name(int cmd_id);
		std::string get_cfg_name (int cfg_if);

        FcpMessage *get_message(int messageIdentifier);

	private:
        std::map<int, std::string>  cmd_names;
		std::map<int, std::string> cfgs_names;
};

#endif
