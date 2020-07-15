#include <stdlib.h>
#include "fcp_message.hpp"

#include <iostream>

#include "signal_parser.h"

using namespace std;

void FcpMessage::decompile(json j) {
	this->id = j["id"];
	this->name = j["name"];
	this->dlc = j["dlc"];
	this->frequency = j["frequency"];

	for (auto& el: j["signals"].items()) {
		FcpSignal fcp_signal;
		fcp_signal.decompile(el.value());
		this->signals[el.key()] = fcp_signal;
	}

	return;
}

std::pair<std::string, std::map<std::string, double>> FcpMessage::decode_msg(CANdata msg) {
	std::map<std::string, double> signals;
	
	for (auto& el: this->signals) {
		signals[el.first] = el.second.decode_signal(msg);
	}
	return std::make_pair(this->name, signals);
}

CANdata FcpMessage::encode_msg(std::map<std::string, double> decoded_msg){
	CANdata msg;
	uint64_t *ptr = (uint64_t *)&msg.data;
	uint64_t word = 0;

	msg.dlc = this->dlc;

	for (auto& el: this->signals)
		word |= el.second.encode_signal(decoded_msg[el.first]);

	*ptr = word;

	return msg;
}