#ifndef _CANDATA
#define _CANDATA

#include <stdint.h>
//#include <type_traits>


enum class MESSAGE_TYPE{
    CAN_2_0A = 0, 
    CAN_2_0B
};

struct CANdata {
    MESSAGE_TYPE type;
    union {
        union {
            struct {
                uint16_t dev_id : 5; // Least significant
                uint16_t msg_id : 6; // Most significant
            };
            uint16_t sid; // Standard ID (11-bit)
        };  // Union ensures only one ID is stored at a time
        union {
            struct {
                uint32_t e_msg_id : 10; // Most significant
            };
            uint32_t eid; // Extended ID (29-bit)
        };
    } id;

    uint16_t dlc : 4;  // Data length code (0-8)
    uint16_t data[4];  // Payload
    
    template <typename T>
    static T make_id (uint16_t dev_id, T msg_id) {
        return (dev_id << 5) + msg_id;
    }

    template <typename T>
    static T get_dev_id (T sid) {
        return sid & 0x1F;
    }

    template <typename T>
    static T get_msg_id (T sid) {
        return (sid >> 5);
    }

};






#endif

