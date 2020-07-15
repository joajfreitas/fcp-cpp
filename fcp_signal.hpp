#ifndef __FCP_SIGNAL_H__
#define __FCP_SIGNAL_H__

#include <stdlib.h>
#include <stdint.h>
#include <iostream>

#include "candata.h"

extern "C" {
	#include "signal_parser.h"
}

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class FcpSignal {
	public:
		std::string name;
		int start;
		int length;
		double scale;
		double offset;
		std::string unit;
		std::string comment;
		int min_value;
		int max_value;
		std::string type;
		std::string byte_order;
		std::string mux;
		int mux_count;

		void decompile(json j);
		double decode_signal(CANdata msg);
		uint64_t encode_signal(double signal);

};

#endif
