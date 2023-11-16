-- psh
-- Copyright (C) 2017-2023 hepangda

-- This program is free software, distributed under the GPL v3.
-- For full terms see the included LICENSE and COPYLEFT file.

add_requires("readline", "fmt")

target("psh")
    set_kind("binary")
    add_includedirs(".")
    add_headerfiles("src/*.h")
    add_files("src/*.cc", "src/commands/*.cc")
    add_packages("readline", "fmt")
    set_languages("cxx20")
