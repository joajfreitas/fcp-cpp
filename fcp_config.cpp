#include "fcp_config.hpp"

void FcpConfig::decompile(json j)
{
    this->name = j["name"];
    this->id = j["id"];
    return;
}
