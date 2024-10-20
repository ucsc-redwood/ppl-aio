target("test-glm")
    set_kind("binary")
    add_files("test-glm.cpp")
    add_packages("gtest", "glm")
target_end()

target("test-vulkan")
    set_kind("binary")
    add_includedirs("$(projectdir)/include")
    add_files("test-vulkan.cpp")
    add_packages("gtest", "vulkan-headers")
    add_packages("vulkansdk")

