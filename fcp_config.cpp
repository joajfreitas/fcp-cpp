#include "fcp_config.hpp"

void FcpConfig::decompile(json j) {
	this->name = j["name"];
	std::string tmp = j["id"];
	this->id = stoi(tmp);
	return;
}
