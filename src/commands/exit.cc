// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#include "exit.h"

namespace psh {

[[noreturn]] int ExitCommand::execute(Context &ctx, Expression &cmd) { exit(0); }

bool ExitCommand::match(std::string_view patterns) { return patterns == "exit" || patterns == "logout"; }

}  // namespace psh
