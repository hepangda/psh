// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#include "generic.h"

#include <dirent.h>
#include <fcntl.h>
#include <fmt/format.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "src/command.h"
#include "src/utils.h"

using psh::details::SearchResult;

namespace psh {

int GenericCommand::execute(Context &ctx, Expression &cmd) {
  auto search_result = findExecfile(ctx, cmd.execfile);

  if (search_result.type == SearchResult::Type::FileNotFound) {
    cmd.is_right_cmd = 100;
    return -1;
  } else if (search_result.type == SearchResult::Type::FolderNotFound) {
    cmd.is_right_cmd = 300;
    return -1;
  }

  std::string &path = search_result.path;  // Find the absolute path of the command

  // Construct the parameter list in accordance with system call requirements
  char *arglist[cmd.arguments.size() + 1];
  char args[200][256];
  for (auto i = 0u; i < cmd.arguments.size(); i++) {
    strcpy(args[i], cmd.arguments[i].c_str());
    arglist[i] = (char *)args[i];
  }
  arglist[cmd.arguments.size()] =
      (char *)nullptr;  // Ensure that the parameter list is terminated with NULL

  pid_t child = fork();

  if (child < 0) {
    psh_error(200);
    return -1;
  }

  // Parent process
  if (child > 0) {
    // If it is a background process, let it run independently. Output its ID and continue reading
    // the next command.
    if (cmd.is_background) {
      printf("[Process id] %d\n", (int)child);
      return 0;
    } else {
      int ret;

      //  Wait for the child process to finish. We don't use wait() here because:
      // If a process with the '&' syntax has finished before this point,
      // calling wait here would retrieve the status of the previous background process.
      // This could lead to the current command not being finished yet,
      // and the parent process exiting the waiting loop to start waiting for new input.
      if (waitpid(child, &ret, 0) == -1) {
        perror("psh");
        return -1;
      }
      return 0;
    }
  }

  // Child process
  if (child == 0) {
    if (cmd.is_redirect_stdin) {
      int fd = open(cmd.filename_in.c_str(), O_RDONLY);
      if (fd < 0) {
        psh_error(201);
        exit(0);
      }
      // The reason for not using dup is that dup only copies file descriptors,
      // and it does not allow specifying what the new file descriptor should be.
      // Here, the new descriptor is set to the standard input file descriptor.
      // dup2() will automatically close the original STDIN descriptor
      // and set the file descriptor of fd to STDIN_FILENO.
      dup2(fd, STDIN_FILENO);
    }

    if (cmd.is_redirect_stdout) {
      mode_t mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;
      int flag = O_WRONLY | O_CREAT;
      flag |= (cmd.stdout_mode) ? O_APPEND : O_TRUNC;
      int fd = open(cmd.filename_out.c_str(), flag, mode);
      if (fd < 0) {
        psh_error(202);
        exit(0);
      }
      dup2(fd, STDOUT_FILENO);
    }

    int ret = execv(path.c_str(), arglist);

    if (ret == -1) {
      perror("psh");
      exit(-1);
    }
    // kill child process
    exit(0);
  }
  return -1;
}

bool GenericCommand::match(std::string_view patterns) { return true; }

SearchResult isExistedInPath(std::string_view path, std::string_view filename) {
  DIR *dp = opendir(path.data());

  if (dp == NULL) {
    return {SearchResult::Type::FolderNotFound};
  }

  dirent *dirp;
  while ((dirp = readdir(dp)) != nullptr) {
    if (dirp->d_name == filename) {
      if (path == "./") {
        if (!filename.starts_with("./")) {
          continue;
        }

        char *current_dir = getcwd(NULL, 0);
        auto current_dir_sv = std::string_view{current_dir};
        auto ret = SearchResult{fmt::format("{}{}", current_dir_sv, filename.substr(1))};
        free(current_dir);
        return ret;
      }

      return {fmt::format("{}/{}", path, filename)};
    }
  }

  return {SearchResult::Type::FileNotFound};
}

SearchResult GenericCommand::findExecfile(Context &ctx, std::string_view execfile) {
  // If the command is not a built-in command, then sequentially search for the executable file of
  // the command in the directories specified by the environment variable.

  auto envpath = ctx.path_folders();

  auto result = isExistedInPath("./", execfile);
  if (result.type == SearchResult::Type::Found) {
    return result;
  }

  for (auto it : envpath) {
    result = isExistedInPath(it, execfile);
    if (result.type == SearchResult::Type::Found) {
      return result;
    }
  }
  return result;
}

}  // namespace psh
