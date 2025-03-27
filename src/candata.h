#ifndef _CANDATA
#define _CANDATA

#include <stdint.h>
#include <type_traits>

enum class MessageType{
    CAN_2_0A = 0, 
    CAN_2_0B
};

struct CANdata {
    MessageType type;
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
                uint32_t e_msg_id : 24; // Most significant
            };
            uint32_t eid; // Extended ID (29-bit)
        };
    } id;

    uint16_t dlc : 4;  // Data length code (0-8)
    uint16_t data[4];  // Payload

    /**
     * @brief Create an extended ID frame with a given device and message ID
     * 
     * @param dev_id The device ID
     * @param msg_id The message ID
     * @param dlc - Data lenght code (0-4)
     * @param data - Array of 4 16-bit data words
     * 
     * @return CANdata
     * 
     * @note The message EID is constructed as follows:
     *      - The 5 least significant bits are the device ID
     *      - The 24 most significant bits are the message ID
     */
    static CANdata  MakeExtendedIdFrame(uint16_t dev_id, uint32_t msg_id, uint16_t dlc, uint16_t data[4]) {
        return CANdata{ 
            .type = MessageType::CAN_2_0B,
            .id = { .eid = CANdata::make_id(dev_id, msg_id) }, 
            .dlc = dlc, 
            .data = {data[0], data[1], data[2], data[3]}
        };
    }

    /**
     * @brief Create an extended ID frame with a given EID
     * 
     * @param eid The extended ID
     * @param dlc - Data lenght code (0-4)
     * @param data - Array of 4 16-bit data words
     * 
     * @return CANdata
     */
    static CANdata MakeExtendedIdFrame(uint32_t eid, uint16_t dlc, uint16_t data[4]) {
        return CANdata{
            .type = MessageType::CAN_2_0B,
            .id = { .eid = eid },
            .dlc = dlc,
            .data = { data[0], data[1], data[2], data[3] }
        };
    }

    /**
     * @brief Create a standard ID frame with a given device and message ID
     * 
     * @param dev_id The device ID
     * @param msg_id The message ID
     * @param dlc - Data lenght code (0-4)
     * @param data - Array of 4 16-bit data words
     * 
     * @return CANdata
     * 
     * @note The message SID is constructed as follows: 
     *     - The 5 least significant bits are the device ID 
     *     - The 6 most significant bits are the message ID
     */
    static CANdata MakeStandardIdFrame(uint16_t dev_id, uint16_t msg_id, uint16_t dlc, uint16_t data[4]) {
        return CANdata{
            .type = MessageType::CAN_2_0A,
            .id = { .sid = CANdata::make_id(dev_id, msg_id) },
            .dlc = dlc,
            .data = { data[0], data[1], data[2], data[3] }
        };
    }

    /**
     * @brief Create a standard ID frame with a given SID
     * 
     * @param sid The standard ID 
     * @param dlc - Data lenght code (0-4)
     * @param data - Array of 4 16-bit data words
     * 
     * @return CANdata
     */
    static CANdata MakeStandardIdFrame(uint16_t sid, uint16_t dlc, uint16_t data[4]) {
        return CANdata{
            .type = MessageType::CAN_2_0A,
            .id = { .sid = sid },
            .dlc = dlc,
            .data = { data[0], data[1], data[2], data[3] }
        };
    }

    /**
     * @brief Create a generic CAN frame (standard or extended) depending on the message ID type
     * 
     * @tparam T Either uint16_t (standard ID) or uint32_t (extended ID)
     * @param dev_id The device ID
     * @param msg_id The message ID
     * @param dlc - Data lenght code (0-4)
     * @param data - Array of 4 16-bit data words
     * 
     * @return CANdata
     */
    template <typename T>
    static typename std::enable_if<
        std::is_same<T, uint16_t>::value || std::is_same<T, uint32_t>::value,
        CANdata
    >::type
    MakeFrame(uint16_t dev_id, T msg_id, uint16_t dlc, uint16_t data[4]) {
        if constexpr (std::is_same<T, uint16_t>::value) {
            return MakeStandardIdFrame(dev_id, msg_id, dlc, data);
        } else {
            return MakeExtendedIdFrame(dev_id, msg_id, dlc, data);
        }
    }

    /**
     * @brief Create a message ID from a device ID and a message ID
     * 
     * @param dev_id The device ID
     * @param msg_id The message ID
     * 
     * @return T
     */
    template <typename T>
    static T make_id (uint16_t dev_id, T msg_id) {
        return (dev_id << 5) + msg_id;
    }

    /**
     * @brief Get the device ID from a message ID
     * 
     * @param sid The message ID
     * 
     * @return T
     */
    template <typename T>
    static T get_dev_id (T sid) {
        return sid & 0x1F;
    }

    /**
     * @brief Get the message ID from a message ID
     * 
     * @param sid The message ID
     * 
     * @return T
     */
    template <typename T>
    static T get_msg_id (T sid) {
        return (sid >> 5);
    }

    uint32_t get_message_id() {
        if (this->type == MessageType::CAN_2_0A) {
            return static_cast<uint16_t>(this->id.sid);
        } else {
            return this->id.eid;
        }   
    }
};

#endif
