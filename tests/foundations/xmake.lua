target("test-glm")
    set_kind("binary")
    add_files("test-glm.cpp")
    add_packages("gtest", "glm")

target("test-vulkan")
    set_kind("binary")
    add_includedirs("$(projectdir)/include")
    add_files("test-vulkan.cpp")
    add_packages("gtest", "vulkan-headers")

if is_plat("linux") then
target("test-cuda")
    set_kind("binary")
    add_files("test-cuda.cu")
    add_packages("gtest")
    add_cugencodes("native")
end

