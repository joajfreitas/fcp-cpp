#include "fcp_log.hpp"

#include <iostream>

using namespace std;

void FcpLog::decompile(json j)
{
    this->id = j["id"];
    this->n_args = j["n_args"];
    this->name = j["name"];
    this->comment = j["comment"];
}
