#include "fcp_config.hpp"

void FcpConfig::decompile(json j)
{
    this->name = j["name"];
    this->id = j["id"];
    this->comment = j["comment"];
    return;
}

void FcpConfig::setId(int id)
{
    this->id = id;
}

void FcpConfig::setName(std::string name)
{
    this->name = name;
}

void FcpConfig::setComment(std::string comment)
{
    this->comment = comment;
}
