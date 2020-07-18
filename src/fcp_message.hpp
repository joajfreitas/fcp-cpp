#ifndef __FCP_MESSAGE_H__
#define __FCP_MESSAGE_H__

#include "json.hpp"

#include "candata.h"
#include "fcp_signal.hpp"

using json = nlohmann::json;

class FcpMessage {
	public:
		int id;
		int dlc;
		int frequency;
		std::string name;
		std::map<std::string, FcpSignal> signals;

		void decompile(json j);
		std::pair<std::string, std::map<std::string, double>> decode_msg(CANdata msg);
		CANdata encode_msg(std::map<std::string, double> decoded_msg);
		CANdata parse_big_endian(CANdata msg, std::pair<std::string, FcpSignal> el);

};

#endif
