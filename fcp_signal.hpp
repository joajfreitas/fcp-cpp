#ifndef __FCP_SIGNAL_H__
#define __FCP_SIGNAL_H__

#include <stdlib.h>
#include <stdint.h>
#include <iostream>

#include "candata.h"

#include "signal_parser.h"

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class FcpSignal {
	public:
		void decompile(json j);
		double decode_signal(CANdata msg);
		uint64_t encode_signal(double signal);
		fcp_type_t getType (std::string type);
		fcp_endianess_t getEndianess (std::string type);
		string mux;
		unsigned mux_count;
	
	private:
		fcp_signal_t signal;

};

#endif
