#ifndef __FCP_CONFIG_H__
#define __FCP_CONFIG_H__

#include <stdlib.h>

#include "json.hpp"

using json = nlohmann::json;

class FcpConfig {
	public:
		int id;
		std::string name;
		
		void decompile(json j);
};

#endif
