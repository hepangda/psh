// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#include "context.h"

#include <memory>

#include "command.h"
#include "commands/cd.h"
#include "commands/exit.h"
#include "commands/generic.h"
#include "utils.h"

namespace psh {

const std::vector<std::string>& Context::path_folders() { return path_folders_; }

std::optional<std::string_view> Context::env(std::string_view name) {
  return std::optional{getenv("PATH")};
  // return env_.contains(name) ? std::optional{env_.at("PATH")} : std::nullopt;
}

Context Context::fromEnvp(char** envp) {
  Context ctx;

  std::unordered_map<std::string_view, std::string_view> env;
  for (char** env_var = envp; *env_var != nullptr; env_var++) {
    std::string_view env_str = *env_var;
    size_t pos = env_str.find('=');
    if (pos != std::string_view::npos) {
      env.emplace(env_str.substr(0, pos), env_str.substr(pos + 1));
    }
  }

  ctx.envp_ = envp;
  ctx.path_folders_ = splitString(ctx.env("PATH").value_or(""), ':');

  ctx.command_chains_.emplace_back(std::make_shared<ExitCommand>());
  ctx.command_chains_.emplace_back(std::make_shared<CdCommand>());

  return ctx;
}

std::shared_ptr<Command> Context::findCommand(Expression& cmd) {
  for (auto& command : command_chains_) {
    if (command->match(cmd.execfile)) {
      cmd.is_right_cmd = 1;
      return command;
    }
  }

  return std::make_shared<GenericCommand>();
}

}  // namespace psh
