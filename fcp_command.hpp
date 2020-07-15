#ifndef __FCP_COMMAND_H__
#define __FCP_COMMAND_H__

#include "fcp_argument.hpp"

#include "json.hpp"

using json = nlohmann::json;

class FcpCommand {
	public:
		int id;
		int n_args;
		std::string name;
		std::string comment;
		std::map<std::string, FcpArgument> args;
		std::map<std::string, FcpArgument> rets;
		void decompile(json j);
};

#endif
