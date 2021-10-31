#include "fcp_log.hpp"

#include <iostream>

void FcpLog::decompile(json j)
{
    this->id = j["id"];
    this->n_args = j["n_args"];
    this->name = j["name"];
    this->comment = j["comment"];
    this->string = j["string"];
}
