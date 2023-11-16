// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#include "cd.h"

#include <unistd.h>

namespace psh {

int CdCommand::execute(Context &ctx, Expression &cmd) {
    const std::string &dir = (cmd.arguments.size() <= 1) ? "." : cmd.arguments.at(1);

    // Change the shell's working directory, if it fails, output the reason.
    if (chdir(dir.c_str()) != 0) {
        perror("psh");
        return -1;
    }
    return 0;
}

bool CdCommand::match(std::string_view patterns) { return patterns == "cd"; }

}  // namespace psh
