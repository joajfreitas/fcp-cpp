#include "fcp_command.hpp"
#include "fcp_message.hpp"

void FcpCommand::decompile(json j)
{

    this->name = j["name"];
    this->id = j["id"];
    this->n_args = j["n_args"];
    this->comment = j["comment"];

    for (auto& el : j["args"].items()) {
        FcpArgument fcp_arg;
        fcp_arg.decompile(el.value());
        this->args[el.key()] = fcp_arg;

        for (auto& el : j["rets"].items()) {
            FcpArgument fcp_arg;
            fcp_arg.decompile(el.value());
            this->rets[el.key()] = fcp_arg;
        }
    }

    return;
}
