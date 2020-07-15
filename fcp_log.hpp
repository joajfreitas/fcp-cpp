#ifndef __FCP_LOG_H__
#define __FCP_LOG_H__

#include <stdlib.h>

#include "json.hpp"

using json = nlohmann::json;

class FcpLog {
	public:
		int id;
		std::string name;
		int n_args;
		std::string comment;
		std::string string;

		void decompile(json j);
};

#endif
