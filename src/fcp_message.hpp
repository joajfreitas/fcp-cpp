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
    bool muxed = false;
    bool extended_frame = false;
    std::string mux;
    std::string name;
    std::map<std::string, FcpSignal> signals_list; // std::string -> nome do sinal
    // signal_list["DU_value"]

    void decompile(json j);
    std::pair<std::string, std::map<std::string, double>> decode_msg(CANdata msg);

    CANdata encode_msg(std::map<std::string, double> decoded_msg, uint16_t dev_id);
    CANdata parse_big_endian(CANdata msg, std::pair<std::string, FcpSignal> el);

    void add_signal(FcpSignal new_signal);
};

#endif
