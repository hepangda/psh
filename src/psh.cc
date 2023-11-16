// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#include "psh.h"

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>

#include <cstdlib>
#include <functional>

#include "context.h"

int main(int argc, char *argv[], char **envp) {
    // Block signals [SIGINT SIGQUIT SIGSTOP SIGTSTP]
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    auto context = psh::Context::fromEnvp(envp);
    while (true) {
        auto prompt = psh::getPrompt();
        std::string st = readline(prompt.c_str());  // Get user input

        // If the user input is not all blank chars, save the command in the command history.
        // Otherwise, skip processing this command and directly get the next command.
        if (psh::isBlankString(st)) {
            continue;
        } else {
            add_history(st.c_str());
        }

        auto cmd = psh::Expression::fromString(st);

        // If the command involves a pipe, execute each command in the pipeline separately.
        if (cmd.is_pipe) {
            auto pipe1 = psh::Expression::fromString(cmd.pipe_prompt[0]);
            auto pipe2 = psh::Expression::fromString(cmd.pipe_prompt[1]);
            
            // If the first command in the pipeline has error, 
            // do not proceed with the execution of the second command.
            if (pipe1.execute(context) != 0) {
                continue;
            }
            pipe2.execute(context);
        } else if (cmd.is_right_cmd) {
            psh::psh_error(cmd.is_right_cmd);
            continue;
        }

        cmd.execute(context);
    }
    return 0;
}