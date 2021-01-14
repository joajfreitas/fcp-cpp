#include "fcp_message.hpp"
#include <stdlib.h>

#include <iostream>

#include "signal_parser.h"

using namespace std;

void FcpMessage::decompile(json j)
{
    this->id = j["id"];
    this->name = j["name"];
    this->dlc = j["dlc"];
    this->frequency = j["frequency"];
    this->muxed = false;
    this->mux = "";

    for (auto& el : j["signals"].items()) {
        FcpSignal fcp_signal;
        fcp_signal.decompile(el.value());
        if (fcp_signal.mux_count > 1) {
            this->muxed = true;
            this->mux = fcp_signal.mux;
        }
        this->signals[el.key()] = fcp_signal;
    }

    return;
}

std::pair<std::string, std::map<std::string, double>> FcpMessage::decode_msg(CANdata msg)
{
    std::map<std::string, double> signals;
    
    if (this->muxed == false) {
        for (auto& el : this->signals) {
            auto signal = el.second;
            auto name = el.first;
            signals[name] = signal.decode_signal(msg);
        }
    }
    else {
        auto mux_signal = this->signals[this->mux];
        signals[this->mux] = mux_signal.decode_signal(msg);
        unsigned mux_index = signals[this->mux];

        for (auto& el : this->signals) {
            if (el.first == this->mux) {
                continue;
            }

            auto signal = el.second;
            auto name = el.first;
            signals[name + to_string(mux_index)] = signal.decode_signal(msg);
        }
    }


    return std::make_pair(this->name, signals);
}

CANdata FcpMessage::encode_msg(std::map<std::string, double> decoded_msg)
{
    CANdata msg;
    uint64_t* ptr = (uint64_t*)&msg.data;
    uint64_t word = 0;

    msg.dlc = this->dlc;

    for (auto& el : this->signals)
        word |= el.second.encode_signal(decoded_msg[el.first]);

    *ptr = word;

    return msg;
}
