// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#include "expression.h"

#include <dirent.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <cstdlib>
#include <cstring>
#include <string_view>

#include "command.h"
#include "context.h"
#include "psh.h"

namespace psh {

std::vector<std::string> tokenize_command(const std::string &cmd) {
  std::vector<std::string> tokens;
  std::string token;
  for (std::size_t i = 0; i < cmd.size(); ++i) {
    char c = cmd[i];
    if (c == ' ' || c == '<' || c == '|' || c == '&') {
      if (!token.empty()) {
        tokens.emplace_back(token);
        token.clear();
      }
      if (c != ' ') {
        tokens.emplace_back(std::string(1, c));
      }
    } else if (c == '>') {
      if (!token.empty()) {
        tokens.emplace_back(token);
        token.clear();
      }
      if (i + 1 < cmd.size() && cmd[i + 1] == '>') {
        tokens.emplace_back(">>");
        ++i;  // Skip the next character
      } else {
        tokens.emplace_back(">");
      }
    } else {
      token += c;
    }
  }
  if (!token.empty()) {
    tokens.emplace_back(token);
  }
  return tokens;
}

// Build a pipeline command.
// Implementation of the pipeline: split the pipeline into two commands with redirection,
// and execute each command separately.
static void pipe_buildcmd(Expression &origin_cmd) {
  auto in = origin_cmd.arguments;
  origin_cmd.pipe_prompt[0] = origin_cmd.pipe_prompt[1] = "";

  bool front = true;
  for (auto i : in) {
    // If the | is not detected, it means we are still on the left side of the pipeline.
    if (i != "|") {
      if (front) {
        origin_cmd.pipe_prompt[0] += i + " ";
      } else {
        origin_cmd.pipe_prompt[1] += i + " ";
      }
    } else {
      front = false;
    }
  }
  // Complete the left/right side command of the pipeline.
  origin_cmd.pipe_prompt[0] += " > /tmp/psh_pipefile";
  origin_cmd.pipe_prompt[1] += " < /tmp/psh_pipefile";
}

// Remove positions in the parameter table occupied by syntax elements
static void setarg_command(Expression &cmdt) {
  std::vector<std::string> ret, in = cmdt.arguments;
  int sz = in.size();

  // If the command does not contain any syntax elements, excluding pipes as handling them is done
  // earlier.
  if (!(cmdt.is_redirect_stdin || cmdt.is_redirect_stdout || cmdt.is_background)) {
    return;
  }

  // Iterate through the contents of the parameter list
  for (int i = 0; i < sz; i++) {
    if (in[i] == ">" || in[i] == "<" || in[i] == ">>") {
      // If it is <>, skip an additional content
      i++;
    } else if (in[i] == "&") {
      // If it is &, skip directly; the loop will automatically increment i at the end
      continue;
    } else {
      // If there are no syntax elements, it is part of the parameters and should be retained
      ret.push_back(in[i]);
    }
  }
  cmdt.arguments = ret;
}

static void setmark_command(Expression &cmdt) {
  // Start checking each element from the position after the command name
  for (auto it = cmdt.arguments.begin() + 1; it != cmdt.arguments.end(); it++) {
    if (*it == ">" || *it == ">>") {
      cmdt.is_redirect_stdout = true;
      if (*it == ">>") cmdt.stdout_mode = true;
      if (it + 1 == cmdt.arguments.end()) {
        cmdt.is_right_cmd = 400;
        break;
      }
      cmdt.filename_out = *++it;
      continue;
    } else if (*it == "<") {
      cmdt.is_redirect_stdin = true;
      if (it + 1 == cmdt.arguments.end()) {
        cmdt.is_right_cmd = 401;
        break;
      }
      cmdt.filename_in = *++it;
      continue;
    } else if (*it == "|") {
      cmdt.is_pipe = true;
      cmdt.is_right_cmd = 490;
      pipe_buildcmd(cmdt);
      break;
    } else if (*it == "&") {
      if (it + 1 == cmdt.arguments.end()) {
        cmdt.is_background = true;
      } else {
        cmdt.is_right_cmd = 403;
        break;
      }
    }
  }
  if (cmdt.is_right_cmd || cmdt.is_pipe) {
    return;
  }

  // Remove redundant syntax elements within the sentence
  setarg_command(cmdt);
}

Expression Expression::fromString(const std::string &cmd) {
  Expression ret;
  ret.arguments = tokenize_command(cmd);
  ret.execfile = ret.arguments[0];
  setmark_command(ret);
  return ret;
}

int Expression::execute(Context &context) {
  if (is_right_cmd) {
    psh_error(is_right_cmd);
    return -1;
  }

  auto command = context.findCommand(*this);
  int ret = command->execute(context, *this);

  if (is_right_cmd) {
    psh_error(is_right_cmd);
    return -1;
  }
  return ret;
}

}  // namespace psh
