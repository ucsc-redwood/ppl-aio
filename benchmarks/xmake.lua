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


add_requires("benchmark")

includes("cpu")