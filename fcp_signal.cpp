#include "fcp_signal.hpp"

using namespace std;

void FcpSignal::decompile(json j) {
		this->name			= j["name"];
		this->start			= j["start"];
		this->length		= j["length"];
		this->scale			= j["scale"];
		this->offset		= j["offset"];
		this->unit			= j["unit"];
		this->comment		= j["comment"];
		this->min_value		= j["min_value"];
		this->max_value		= j["max_value"];
		this->type			= j["type"];
		this->byte_order	= j["byte_order"];
		this->mux			= j["mux"];
		this->mux_count		= j["mux_count"];
	return;
}

double FcpSignal::decode_signal(CANdata msg) {
	if (this->type == "unsigned") {
		return decode_signal_unsigned_as_double(msg, this->start, this->length,
				this->scale, this->offset);
	}
	else if (this->type == "signed") {
		return decode_signal_signed_as_double(msg, this->start, this->length,
				this->scale, this->offset);
	}
	else {
		return 0;
	}
}

uint64_t FcpSignal::encode_signal(double signal){
	if (this->type == "unsigned") {
		return encode_signal_unsigned_from_double(signal, this->start, this->length,
				this->scale, this->offset);
	}
	else if (this->type == "signed") {
		return encode_signal_signed_from_double(signal, this->start, this->length,
				this->scale, this->offset);
	}
	else {
		return 0;
	}
}
