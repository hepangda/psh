// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#pragma once
#include "src/command.h"

namespace psh {

class ExitCommand : public Command {
   public:
    [[noreturn]] virtual int execute(Context &ctx, Expression &cmd) override;
    virtual bool match(std::string_view patterns) override;
};

}  // namespace psh
