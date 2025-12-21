#pragma once

#include <array>
#include <map>
#include <string>
#include <cstdint>

namespace fcp {

struct CanMessage {
        std::uint64_t timestamp;
	std::uint32_t id;
	std::uint8_t dlc;
	std::array<uint8_t, 8> data;
};

struct Decoded {
        std::uint64_t timestamp;
        std::string message_name;
        std::map<std::string, double> signals;
};


}

std::ostream& operator<<(std::ostream& os, const fcp::CanMessage& msg);
std::ostream& operator<<(std::ostream& os, const fcp::Decoded& msg);
