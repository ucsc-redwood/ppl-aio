target("test-host-kernels")
    set_kind("binary")
    add_includedirs("$(projectdir)/include")
    add_files("test-host-kernels.cpp")
    add_packages("gtest", "glm")
    add_deps("ppl")
