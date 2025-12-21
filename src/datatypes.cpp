#include "datatypes.h"

#include <ostream>

// implement ostream operator for fcp::CanMessage
std::ostream& operator<<(std::ostream& os, const fcp::CanMessage& msg) {
    os << "timestamp: " << msg.timestamp << ", id: " << msg.id << ", dlc: " << static_cast<int>(msg.dlc) << ", data: [";
    for (size_t i = 0; i < msg.dlc; ++i) {
	os << static_cast<int>(msg.data[i]);
	if (i < msg.dlc - 1) {
	    os << ", ";
	}
    }
    os << "]";
    return os;
}


std::ostream& operator<<(std::ostream& os, const fcp::Decoded& msg) {
    os << "timestamp: " << msg.timestamp << ",\nmessage_name: " << msg.message_name << ",\nsignals: {\n";
    size_t count = 0;
    for (const auto& [key, value] : msg.signals) {
	os << "\t" << key << ": " << value;
	if (count < msg.signals.size() - 1) {
	    os << ", " << std::endl;
	}
	++count;
    }
    os << "}";
    return os;
}
