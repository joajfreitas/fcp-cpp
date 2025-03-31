#ifndef _CANDATA
#define _CANDATA

#include <array>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <stdint.h>

inline uint8_t get_dev_id(uint32_t sid)
{
    return (sid & 0x1F);
}

inline uint32_t get_msg_id(uint32_t sid)
{
    return (sid >> 5);
}

class CANdata {
public:
    CANdata(uint8_t dev_id, uint32_t msg_id, uint8_t dlc, std::array<uint16_t, 4> data, bool extended_frame = false)
        : sid(CANdata::make_sid(dev_id, msg_id))
        , dlc(dlc)
        , data(data)
        , extended_frame(extended_frame)
    {

        assert(data.size() <= 4);
        assert(dlc < 8);
        assert(extended_frame == false && sid <= 0x7FF);
    }

    uint32_t get_sid() const
    {
        return sid;
    }

    bool is_extended_frame() const
    {
        return extended_frame;
    }

    std::array<uint16_t, 4> get_data()
    {
        return data;
    }

    uint16_t get_data(uint8_t index)
    {
        assert(index < 4);
        return data[index];
    }

    uint8_t get_dlc()
    {
        return dlc;
    }

private:
    uint32_t sid = 0;
    bool extended_frame = false;
    uint8_t dlc = 8;
    std::array<uint16_t, 4> data = { 0, 0, 0, 0 };

    static uint32_t constexpr make_sid(uint8_t dev_id, uint32_t msg_id)
    {
        return (msg_id << 5) | dev_id;
    }

    static constexpr uint32_t make_sid(uint8_t dev_id, uint8_t msg_id)
    {
        return (msg_id << 5) | dev_id;
    }
};

#endif
