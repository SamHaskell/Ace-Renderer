workspace "Renderer"
    configurations { "debug", "release" }
    platforms { "mac", "web" }

    project "Renderer"
    kind "WindowedApp"
    location "build"
    language "C++"
    cppdialect "C++20"
    targetextension ""
    
    targetdir ("bin/%{cfg.platform}/%{cfg.buildcfg}")
    objdir ("bin/intermediates/%{cfg.platform}/%{cfg.buildcfg}")

    files {
        -- source
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp",

        -- imgui
        "vendor/imgui/*.h",
        "vendor/imgui/*.cpp",
        "vendor/imgui/backends/imgui_impl_sdlrenderer2.h",
        "vendor/imgui/backends/imgui_impl_sdlrenderer2.cpp",
        "vendor/imgui/backends/imgui_impl_sdl2.h",
        "vendor/imgui/backends/imgui_impl_sdl2.cpp",
    }

    includedirs {
        "src",
        "vendor/imgui"

    }

    filter "platforms:mac"
        defines { "PLATFORM_MAC" }
        links { "SDL2" }
        includedirs { "vendor/sdl/include" }

    filter "platforms:web"
        defines { "PLATFORM_WEB" }
        architecture "x86"
        linkoptions { "-s ALLOW_MEMORY_GROWTH" }
        buildoptions { "-s USE_SDL=2" }
        linkoptions { "-s USE_SDL=2" }

    filter "configurations:debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "RELEASE" }
        optimize "On"