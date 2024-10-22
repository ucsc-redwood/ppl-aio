add_rules("mode.debug", "mode.release")

-- Project-wise settings
set_languages("c++20")
set_warnings("allextra")

if is_plat("android") then
    package("benchmark")
        set_kind("library")
        add_deps("cmake") 
        set_urls("https://github.com/google/benchmark.git")
        add_versions("v1.9.0", "12235e24652fc7f809373e7c11a5f73c5763fc4c") 

        on_install(function (package)
            local configs = {}
            table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
            table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
            table.insert(configs, "-DBENCHMARK_DOWNLOAD_DEPENDENCIES=on")
            table.insert(configs, "-DHAVE_THREAD_SAFETY_ATTRIBUTES=0")
            import("package.tools.cmake").install(package, configs)
        end)
    package_end()
end

add_requires("glm")
add_requires("gtest", "benchmark")

-- vulakn works for both Android and Linux
add_requires("vulkan-headers")

-- before_build(function(target)
--     os.exec("python3 ./scripts/compile-shaders.py")
-- end)


includes("tests")
includes("benchmarks")
includes("demo")
includes("ppl")

after_build(function(target)
    platform = target:plat()
    arch = target:arch()
    build_path = ""
    -- local symsdir = path.join("$(buildir)", "$(plat)", "syms", "$(mode)", "$(arch)")
    local validationlayers = target:pkg("vulkan-validationlayers")
    if validationlayers then
        local arch = target:arch()
        os.vcp(path.join(validationlayers:installdir(), "lib", arch, "*.so"), path.join(os.scriptdir(), "..", "libs", arch))
    end
    if is_mode("release") then
        build_path = "$(buildir)/" .. platform .. "/" .. arch .. "/release/"
    else
        build_path = "$(buildir)/" .. platform .. "/" .. arch .. "/debug/"
    end
    os.cp("shaders/compiled_shaders/**.spv", build_path)
    print("Copied compiled shaders to " .. build_path)
    
end)
