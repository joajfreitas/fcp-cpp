#include "fcp_message.hpp"
#include <stdlib.h>

#include <iostream>

#include "signal_parser.hpp"

void FcpMessage::decompile(json j)
{
    this->id = j["id"];
    this->name = j["name"];
    this->dlc = j["dlc"];
    this->frequency = j["frequency"];
    this->muxed = false;
    this->mux = "";

    if (j.contains("extended_frame")) {
        this->extended_frame = true;
    }

    for (auto& el : j["signals"].items()) {
        FcpSignal fcp_signal;
        fcp_signal.decompile(el.value());
        if (fcp_signal.mux_count > 1) {
            this->muxed = true;
            this->mux = fcp_signal.mux;
        }
        this->signals_list[el.key()] = fcp_signal;
    }

    return;
}

std::pair<std::string, std::map<std::string, double>> FcpMessage::decode_msg(CANdata msg)
{
    std::map<std::string, double> signals_list;

    if (this->muxed == false) {
        for (auto& el : this->signals_list) {
            auto signal = el.second;
            auto name = el.first;
            signals_list[name] = signal.decode_signal(msg);
        }
    } else {
        auto mux_signal = this->signals_list[this->mux];
        signals_list[this->mux] = mux_signal.decode_signal(msg);
        unsigned mux_index = signals_list[this->mux];

        for (auto& el : this->signals_list) {
            if (el.first == this->mux) {
                continue;
            }

            auto signal = el.second;
            auto name = el.first;
            if (signal.mux_count==1)
            {
                signals_list[name] = signal.decode_signal(msg);    
            }
            else{
                signals_list[name + std::to_string(mux_index)] = signal.decode_signal(msg);
            }
        }
    }

    return std::make_pair(this->name, signals_list);
}

CANdata FcpMessage::encode_msg(std::map<std::string, double> decoded_msg)
{
    CANdata msg;

    uint64_t* ptr = (uint64_t*)&msg.data;
    uint64_t word = 0;

    msg.dlc = this->dlc;

    for (auto& el : this->signals_list)
        word |= el.second.encode_signal(decoded_msg[el.first]);

    *ptr = word;

    return msg;
}

void FcpMessage::add_signal(FcpSignal new_signal)
{
    this->signals_list[new_signal.name] = new_signal;
}

