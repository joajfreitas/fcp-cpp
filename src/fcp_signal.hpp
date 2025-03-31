#ifndef __FCP_SIGNAL_H__
#define __FCP_SIGNAL_H__

#include <iostream>
#include <stdint.h>
#include <stdlib.h>

#include "candata.h"

#include "signal_parser.hpp"

#include "json.hpp"

using json = nlohmann::json;

class FcpSignal {
public:
    void decompile(json j);
    double decode_signal(CANdata msg);
    uint64_t encode_signal(double signal);
    fcp_type_t getType(std::string type);
    fcp_endianess_t getEndianess(std::string type);
    fcp_signal_t get_signal() { return this->signal; };

    std::string getTypeReadInt(fcp_type_t type);
    std::string getEndianessReadInt(fcp_endianess_t endianess);

    uint16_t get_Start();
    uint16_t get_lenght();
    double get_scale();
    double get_offset();
    fcp_type_t get_type();
    fcp_endianess_t get_endianess();

    void set_lenght(uint16_t length);
    void set_start(uint16_t start);
    void set_scale(double scale);
    void set_offset(double offset);
    void set_type(fcp_type_t type);
    void set_endianess(fcp_endianess_t endianess);

    int MinValue;
    int MaxValue;
    std::string mux;
    unsigned mux_count;
    std::string unit;
    std::string comment;
    std::string name;

    std::map<fcp_type_t, std::string> string_fcpType_map = {
        { FCP_SIGNED, "signed" },
        { FCP_UNSIGNED, "unsigned" },
        { FCP_FLOAT, "float" },
        { FCP_DOUBLE, "double" }
    };

    std::map<fcp_endianess_t, std::string> string_fcpEndianess_map = {
        { FCP_BIG, "big_endian" },
        { FCP_LITTLE, "little_endian" }
    };

private:
    fcp_signal_t signal;
};

#endif
