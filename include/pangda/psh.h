// psh
// Copyright (C) 2017 hepangda

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#pragma once
#ifndef _HEADER_PANGDA_SHELL__
#define _HEADER_PANGDA_SHELL__

#include<unistd.h>
#include<cstdio>
#include<string>
#include<vector>
#include<functional>
#include<map>

typedef std::vector<std::string> argument_t;

struct command_t {
    int is_right_cmd = 0;               //命令是否是正确的，若为0则说明是正确的，其他数字代表错误码
    std::string execfile;               //执行的文件名
    argument_t arguments;               //参数列表
    bool is_redirect_stdin = false;     //是否重定向了标准输入，即是否存在<语法元素
    std::string filename_in;            //新的标准输入名
    bool is_redirect_stdout = false;    //是否重定向了标准输出，即是否存在>语法元素
    bool stdout_mode = false;           //false表示截断输出，true表示追加输出
    std::string filename_out;           //新的标准输出文件名
    bool is_background = false;         //是否指定在后台运行，即是否存在&语法元素
    bool is_pipe = false;               //是否是一个管道，即是否存在|语法元素
    std::string pipe_prompt[2];         //保存管道命令
};

command_t parse_command(std::string command);
std::string string_trim(std::string s);
std::string get_tip();
int exec_command(command_t &cmd);
int psh_error(int error);
int shellfunc_exit(command_t);
int shellfunc_logout(command_t);
int shellfunc_cd(command_t);

#endif