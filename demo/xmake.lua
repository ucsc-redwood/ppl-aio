includes("cpu-info")
includes("micro-bench")

target("demo-cpu-only")
    set_kind("binary")
    add_includedirs("$(projectdir)/include")
    add_files("cpu_only.cpp")
    add_packages("glm", "cli11")
    add_deps("ppl")
    