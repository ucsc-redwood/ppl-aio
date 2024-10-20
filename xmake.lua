add_rules("mode.debug", "mode.release")

-- set_languages("cxx17")
set_languages("c++20")
set_warnings("allextra")

add_requires("glm")
add_requires("gtest")

-- vulakn works for both Android and Linux
add_requires("vulkan-headers")
-- add_requires("vulkan-headers", "vulkan-validationlayers")
-- add_requires("vulkansdk")

includes("tests")
includes("benchmarks")
includes("demo")
