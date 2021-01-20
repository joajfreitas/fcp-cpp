#include "fcp_argument.hpp"

void FcpArgument::decompile(json j)
{
    this->id = j["id"];
    this->name = j["name"];
    this->comment = j["comment"];
    return;
}
