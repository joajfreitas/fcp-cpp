#ifndef _CANDATA
#define _CANDATA

#include <array>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <stdint.h>

inline uint8_t get_dev_id(uint32_t sid) { return (sid & 0x1F); }

inline uint32_t get_msg_id(uint32_t sid) { return (sid >> 5); }

class CanData {
public:
    CanData() = default;


    CanData(uint8_t dev_id, uint32_t msg_id, uint8_t dlc,
        std::array<uint16_t, 4> data, bool extended_frame = false)
        : sid(CanData::make_sid(dev_id, msg_id))
        , dlc(dlc)
        , data(data)
        , extended_frame(extended_frame)
    {

        assert(data.size() <= 4);
        assert(dlc <= 8);
        assert((extended_frame == false && sid <= 0x7FF) || (extended_frame == true && sid <= 0x1FFFFFFF));
    }

    CanData(uint32_t sid, uint8_t dlc, std::array<uint16_t, 4> data) : sid(sid & 0x1FFFFFFF), dlc(dlc), data(data), extended_frame(CanData::is_extended_frame(sid)) {
        assert(data.size() <= 4);
        assert(dlc <= 8);
        assert((extended_frame == false && sid <= 0x7FF )|| extended_frame == true); // No need to check if sid <= 0x1FFFFFFF since we already masked it, removing bits 29-31, since these flags are not needed to identify the message, and are only used for the bus 
    }

    /**
     * @brief Set the SID of the CanData object.
     * 
     * @param sid The SID to set.
     * @param extended_frame Whether the SID is an extended frame.
     * @details The SID (Standard Identifier) uniquely identifies the CAN message.
     * If the frame is standard, the SID must be ≤ 0x7FF (11 bits).
     * If extended, the SID can go up to 0x1FFFFFFF (29 bits).
     */
    void set_sid(uint32_t sid, bool extended_frame = false) {
        assert((extended_frame == false && sid <= 0x7FF) || (extended_frame == true && sid <= 0x1FFFFFFF));
        this->sid = sid;
        this->extended_frame = extended_frame;
    }

    /**
     * @brief Set the message ID part of the SID using make_sid.
     * 
     * @param msg_id The new message ID.
     * @details The new SID is computed by preserving the current device ID.
     */
    void set_msg_id(uint32_t msg_id) {
        uint8_t dev_id = get_dev_id(this->sid);
        this->sid = CanData::make_sid(dev_id, msg_id);
    }

    /**
     * @brief Set the device ID part of the SID using make_sid.
     * 
     * @param dev_id The new device ID.
     * @details The new SID is computed by preserving the current message ID.
     */
    void set_dev_id(uint8_t dev_id) {
        uint32_t msg_id = get_msg_id(this->sid);
        this->sid = CanData::make_sid(dev_id, msg_id);
    }

    /**
     * @brief Set the data length code (DLC) of the CanData object.
     * 
     * @param dlc The DLC to set.
     * @details The DLC indicates the number of data bytes in the message.
     * Valid DLC values range from 0 to 8.
     */
    void set_dlc(uint8_t dlc) {
        assert(dlc <= 8);
        this->dlc = dlc;
    }

    /**
     * @brief Set the data array of the CanData object.
     * 
     * @param data The data array to set.
     * @details The array can contain up to 4 elements (each 16-bit),
     * corresponding to the CAN frame payload.
     */
    void set_data(std::array<uint16_t, 4> data) {
        assert(data.size() <= 4);
        this->data = data;
    }


    void set_data(uint16_t data, uint8_t index) {
        assert(index < 4);
        this->data[index] = data;
    }

    /**
     * @brief Get the SID of the CanData object.
     *
     * @return uint32_t
     * @details The SID is a unique identifier for the message.
     */
    uint32_t get_sid() const { return sid; }

    /**
     * @brief Check if the frame is an extended frame id
     *
     * @return Boolean
     * @details The extended frame id is used for messages that are larger than
     * the standard frame id. The standard frame id is 11 bits, while the extended
     * frame id is 29 bits.
     */
    bool is_extended_frame() const { return extended_frame; }

    /**
     * @brief Get the data array of the CanData object.
     *
     * @return std::array<uint16_t, 4>
     * @details No more than 4 data elements are allowed in the array.
     */
    std::array<uint16_t, 4> get_data() { return data; }

    /**
     * @brief Get the data at a specific index of the CanData object.
     *
     * @param index The index of the data to get.
     *
     * @return uint16_t
     * @throws std::out_of_range if the index is out of range.
     */
    uint16_t get_data(uint8_t index) { return data.at(index); }

    /**
     * @brief Get the data length code (DLC) of the CanData object.
     *
     * @return uint8_t
     * @details The DLC is the number of data bytes in the message.
     * @details The DLC value are between 0-8, but take into consideration that
     * the data array is only 4 fields, this is due to the structure of a
     * CAN_frame (data = 8 * uint8_t) when our structure is 4*uint16_t
     */
    uint8_t get_dlc() { return dlc; }

private:
    uint32_t sid = 0;
    uint8_t dlc = 8;
    std::array<uint16_t, 4> data = { 0, 0, 0, 0 };
    bool extended_frame = false;

    static constexpr uint32_t make_sid(uint8_t dev_id, uint32_t msg_id)
    {
        return (msg_id << 5) | dev_id;
    }

    static constexpr uint32_t make_sid(uint8_t dev_id, uint8_t msg_id)
    {
        return (msg_id << 5) | dev_id;
    }

    static constexpr bool is_extended_frame(uint32_t sid)
    {
        return sid & 0x80000000 ;
    }

};

#endif
