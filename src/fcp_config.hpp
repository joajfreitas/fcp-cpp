#ifndef __FCP_CONFIG_H__
#define __FCP_CONFIG_H__

#include <stdlib.h>

#include "json.hpp"

using json = nlohmann::json;

class FcpConfig {
public:
    int id;
    std::string name;
    std::string comment;

    void decompile(json j);
    void setId(int id);
    void setName(std::string name);
    void setComment(std::string comment);
};

#endif
