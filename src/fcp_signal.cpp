#include "fcp_signal.hpp"

void FcpSignal::set_lenght(uint16_t length)
{
    this->signal.length = length;
}

void FcpSignal::set_offset(double offset)
{
    this->signal.offset = offset;
}

void FcpSignal::set_scale(double scale)
{
    this->signal.scale = scale;
}

void FcpSignal::set_start(uint16_t start)
{
    this->signal.start = start;
}

void FcpSignal::set_type(fcp_type_t type)
{
    this->signal.type = type;
}

void FcpSignal::set_endianess(fcp_endianess_t endianess)
{
    this->signal.endianess = endianess;
}

uint16_t FcpSignal::get_Start()
{
    return this->signal.start;
}

fcp_endianess_t FcpSignal::get_endianess()
{
    return this->signal.endianess;
}

uint16_t FcpSignal::get_lenght()
{
    return this->signal.length;
}

double FcpSignal::get_offset()
{
    return this->signal.offset;
}

double FcpSignal::get_scale()
{
    return this->signal.scale;
}

fcp_type_t FcpSignal::get_type() 
{
    return this->signal.type;
}

void FcpSignal::decompile(json j)
{
    this->signal.endianess = this->getEndianess(j["byte_order"]);
    this->signal.length = j["length"];
    this->signal.offset = j["offset"];
    this->signal.scale = j["scale"];
    this->signal.start = j["start"];
    this->signal.type = this->getType(j["type"]);
    this->mux = std::string(j["mux"]);
    this->mux_count = j["mux_count"];
    this->unit = j["unit"];
    this->comment = j["comment"];
    this->name = j["name"];
    this->MinValue = j["min_value"];
    this->MaxValue = j["max_value"];

    return;
}

double FcpSignal::decode_signal(CANdata msg)
{
    return fcp_decode_signal_double(msg, this->signal);
}

uint64_t FcpSignal::encode_signal(double signal)
{
    return fcp_encode_signal_double(signal, this->signal);
}

fcp_type_t FcpSignal::getType(std::string type)
{
    for (auto const& [key, val] : string_fcpType_map) {
        if (val == type) {
            return key;
        }
    }
    return FCP_UNSIGNED;
}

fcp_endianess_t FcpSignal::getEndianess(std::string byte_order)
{
    for (auto const& [key, val] : string_fcpEndianess_map) {
        if (val == byte_order) {
            return key;
        }
    }
    return FCP_LITTLE;
}
