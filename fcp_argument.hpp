#ifndef __FCP_ARGUMENT_H__
#define __FCP_ARGUMENT_H__

#include "json.hpp"

using json = nlohmann::json;

class FcpArgument {
	public:
		int id;
		std::string name;
		std::string comment;

		void decompile(json j);
};

#endif
