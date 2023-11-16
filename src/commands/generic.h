// psh
// Copyright (C) 2017-2023 hepangda

// This program is free software, distributed under the GPL v3.
// For full terms see the included LICENSE and COPYLEFT file.
#pragma once
#include <string_view>

#include "src/command.h"

namespace psh {

namespace details {
struct SearchResult {
    enum class Type {
        Found,
        FolderNotFound,
        FileNotFound,
    };

    Type type;
    std::string path;

    SearchResult(Type type) : type(type) {}
    SearchResult(std::string path) : type(Type::Found), path(path) {}
};
}  // namespace details

class GenericCommand : public Command {
   public:
    virtual int execute(Context &ctx, Expression &cmd) override;
    virtual bool match(std::string_view patterns) override;

   private:
    details::SearchResult findExecfile(Context &ctx, std::string_view execfile);
};

}  // namespace psh
