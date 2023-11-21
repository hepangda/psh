// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#pragma once

#include <string_view>

#include "context.h"
#include "expression.h"

namespace psh {

class Command {
 public:
  virtual int execute(Context &ctx, Expression &cmd) = 0;
  virtual bool match(std::string_view patterns) = 0;
};

}  // namespace psh
