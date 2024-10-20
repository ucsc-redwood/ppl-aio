add_rules("mode.debug", "mode.release")

set_languages("c++20")
set_warnings("allextra")

add_requires("glm")
add_requires("gtest")

-- vulakn works for both Android and Linux
add_requires("vulkan-headers")

includes("tests")
includes("benchmarks")
includes("demo")
