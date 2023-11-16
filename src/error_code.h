// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#pragma once

namespace psh {

enum class PshErrorCode {
    NONE = 0,
    BUILTIN_COMMANDS = 1,
    COMMAND_NOT_EXIST = 100,
    FORK_FAILED = 200,
    CANNOT_OPEN_FILE = 201,
    ERROR_PATH_ENV = 300,
    SYNTAX_ERROR = 400,
    IS_PIPE = 490,
};


}  // namespace psh
