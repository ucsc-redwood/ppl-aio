add_rules("mode.debug", "mode.release")

set_languages("cxx17")
set_warnings("all")

add_requires("glm")
add_requires("gtest")

includes("tests")
includes("benchmarks")
includes("demo")
