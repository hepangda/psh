// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#pragma once

#include <string>

#include "expression.h"

namespace psh {

bool isBlankString(const std::string &s);
std::string getPrompt();

int psh_error(int error);
std::vector<std::string> splitString(std::string_view strv, char sep);

}  // namespace psh
