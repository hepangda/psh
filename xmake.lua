add_requires("readline")

target("psh")
    set_kind("binary")
    add_headerfiles("src/*.h")
    add_files("src/*.cc")
    add_packages("readline")