#include "candata.h"
#include <iostream>


int main (  ){
    
    uint32_t eid = (2 << 17) - 1 ;
    std::cout << "EID: " << eid << std::endl;

    CANdata msg = {
        MESSAGE_TYPE::CAN_2_0A, 
        .id = {
            .dev_id = 2, 
            .msg_id = 3,
        },

        .dlc = 2, 
        .data = {0x12, 0x34}
    };
    std::cout << "Device ID: " << msg.id.dev_id << std::endl;
    std::cout << "Message ID: " << msg.id.msg_id << std::endl;
    std::cout << "Message sid " << msg.id.sid << std::endl;
    std::cout << "Message eid " << msg.id.eid << std::endl;
    std::cout << "Message e_msg_id " << msg.id.e_msg_id << std::endl;
    std::cout << "DLC: " << msg.dlc << std::endl;
    std::cout << "Data: ";

    for (int i = 0; i < msg.dlc; i++){
        std::cout << std::hex << msg.data[i] << " ";
    }

    std::cout << std::endl;
    std::cout << std::dec << msg.make_id(2,3) << std::endl;
    std::cout << std::dec << msg.get_dev_id(msg.id.eid) << std::endl;
    std::cout << std::dec << msg.get_msg_id(msg.id.eid) << std::endl;
    std::cout << std::dec << CANdata::get_msg_id(msg.id.eid) << std::endl;





return 0;
}