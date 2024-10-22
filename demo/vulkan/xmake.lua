
target("demo-vulkan")
    set_kind("binary")
    add_includedirs("$(projectdir)/include")
    add_includedirs("./include")
    add_files("main.cpp")
    add_files("$(projectdir)/include/volk.c")
    add_packages("glm", "vulkan-headers")
    -- add_deps("ppl")
    