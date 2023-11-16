// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#pragma once

#include <string>
#include <vector>

namespace psh {

class Context;

class Expression {
   public:
    // Whether the command is correct. If it's 0, then it's correct. Other numbers represent error codes.
    int is_right_cmd = 0;

    // The name of the file to execute.
    std::string execfile;

    // Argument list.
    std::vector<std::string> arguments;

    // Whether standard input has been redirected, i.e., whether there is a '<' syntax element.
    bool is_redirect_stdin = false;

    // New standard input name.
    std::string filename_in;

    // Whether standard output has been redirected, i.e., whether there is a '>' syntax element.
    bool is_redirect_stdout = false;

    // false means truncate output, true means append output.
    bool stdout_mode = false;

    // New standard output filename.
    std::string filename_out;

    // Whether it is specified to run in the background, i.e., whether there is a '&' syntax element.
    bool is_background = false;

    // Whether it is a pipe, i.e., whether there is a '|' syntax element.
    bool is_pipe = false;

    // Save pipe command.
    std::string pipe_prompt[2];

    static Expression fromString(const std::string &cmd);

    int execute(Context &context);
};

}  // namespace psh