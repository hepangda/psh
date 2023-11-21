// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#pragma once

#include <unistd.h>

#include <string>

#include "expression.h"
#include "utils.h"
#define DEBUG_LOG(str, ...) \
  printf("Function: %s, Line: %d," str "\n", __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
