#include "fcp_device.hpp"
#include "fcp.hpp"
#include "fcp_message.hpp"
#include "fcp_command.hpp"

#include "json.hpp"

#include <iostream>

using namespace std;

using json = nlohmann::json;

std::map<int, FcpMessage *> FcpDevice::decompile(json j) {
	
	std::map<int, FcpMessage *> msgs;

	this->id = j["id"];
	this->name = j["name"];

	for (auto& el : j["msgs"].items()) {
		FcpMessage *fcp_msg = new FcpMessage;
		fcp_msg->decompile(el.value());
		this->msgs[el.key()] = fcp_msg;

		int sid = (fcp_msg->id << 5) + this->id;
		msgs[sid] = fcp_msg;
	}

	for (auto& el: j["cmds"].items()) {
		FcpCommand fcp_cmd;
		fcp_cmd.decompile(el.value());
		this->cmds[el.key()] = fcp_cmd;

		this->cmd_names[fcp_cmd.id] = fcp_cmd.name;
	}

	for (auto& el: j["cfgs"].items()) {
		FcpConfig fcp_config;
		fcp_config.decompile(el.value());
		this->cfgs[el.key()] = fcp_config;
	}

	return msgs;		
}

std::string FcpDevice::get_cmd_name(int cmd_id){
	int count = this->cmd_names.count(cmd_id);

	if (count)
		return this->cmd_names[cmd_id];
	else 
		return "";
}

