#include "fcp_signal.hpp"

using namespace std;

void FcpSignal::decompile(json j) {
		
	this->signal.endianess 	= this->getEndianess(j["byte_order"]);
	this->signal.length 	= j["length"];
	this->signal.offset 	= j["offset"];
	this->signal.scale 		= j["scale"];
	this->signal.start 		= j["start"];
	this->signal.type 		= this->getType(j["type"]);

	return;
}

double FcpSignal::decode_signal(CANdata msg) {
	return fcp_decode_signal_double(msg, this->signal);
}

uint64_t FcpSignal::encode_signal(double signal){
	return fcp_encode_signal_double(signal, this->signal);
}

fcp_type_t FcpSignal::getType (std::string type){
	
	if (type == "signed")
		return SIGNED;
	else if (type == "float")
		return FLOAT;
	else if (type == "double")
		return DOUBLE;
	else 
		return UNSIGNED;
}

fcp_endianess_t FcpSignal::getEndianess (std::string byte_order){
	
	if (byte_order == "big_endian")
		return BIG;
	else 
		return LITTLE;
}