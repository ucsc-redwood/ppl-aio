target("test")
    set_kind("binary")
    add_includedirs("$(projectdir)/include")
    add_files("*.cpp")
    add_packages("gtest", "glm")

