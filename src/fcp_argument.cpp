#include "fcp_argument.hpp"

void FcpArgument::decompile(json j) {
	std::string tmp = j["id"];
	this->id = stoi(tmp);
	this->name = j["id"];
	this->comment = j["comment"];
	return;
}
