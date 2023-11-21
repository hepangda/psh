// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#include "utils.h"

#include <fmt/format.h>
#include <pwd.h>
#include <sys/utsname.h>

#include <cstring>
#include <string_view>

#include "psh.h"

namespace psh {

// Get the shell prompt
// Returns something like: "[username@hostname folder] #"
std::string getPrompt() {
  static passwd *current_passwd;
  static char current_hostname[65];
  static char *current_dir;

  current_passwd = getpwuid(getuid());
  current_dir = getcwd(NULL, 0);
  gethostname(current_hostname, 65);

  auto current_username = std::string_view{(current_passwd == NULL) ? "" : current_hostname};
  auto working_dir = std::string_view{current_dir};
  auto shown_dir = std::string_view{working_dir.cbegin() + working_dir.find_last_of('/') + 1};
  if (shown_dir == "") {
    shown_dir = "/";
  }
  char symbol = (geteuid() == 0) ? '#' : '$';

  return fmt::format("[{}@{} {}]{} ", current_username, current_hostname, shown_dir, symbol);
}

// Output error information based on error code
int psh_error(int error) {
  switch (error) {
    case 1:
      break;
    case 100:
      printf("psh: command not found.\n");
      break;
    case 201:
    case 202:
      printf("psh: file doesn\'t exist.\n");
      break;
    case 300:
      printf("psh: environment error.\n");
      break;
    case 400:
    case 401:
    case 403:
      printf("psh: error present.\n");
      break;
  }
  return -1;
}

bool isBlankString(const std::string &s) {
  for (auto i : s) {
    if (!isblank(i)) {
      return false;
    }
  }
  return true;
}

std::vector<std::string> splitString(std::string_view strv, char sep) {
  std::vector<std::string> result;
  size_t start = 0;
  size_t end = strv.find(sep);

  while (end != std::string_view::npos) {
    result.emplace_back(strv.substr(start, end - start));
    start = end + 1;
    end = strv.find(sep, start);
  }

  result.emplace_back(strv.substr(start));
  return result;
}

}  // namespace psh