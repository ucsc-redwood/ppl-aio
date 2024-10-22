target("bench-cpu")
    set_kind("binary")
    add_includedirs("$(projectdir)/include")
    add_files("main.cpp")
    add_packages("benchmark", "glm")
    add_deps("ppl")

