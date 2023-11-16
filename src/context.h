// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <memory>

#include "expression.h"

namespace psh {

class Command;

class Context {
   public:
    static Context fromEnvp(char **envp);
    const std::vector<std::string> &path_folders();
    std::shared_ptr<Command> findCommand(Expression &cmd);

   private:
    Context() = default;
    std::optional<std::string_view> env(std::string_view name);

    char **envp_;
    std::unordered_map<std::string_view, std::string_view> env_;
    std::vector<std::shared_ptr<Command>> command_chains_;
    std::vector<std::string> path_folders_;
};

}  // namespace psh
