project "core"
    kind "staticLib"
    language "c++"
    cppdialect "c++latest"
    
    location "."

    configurations { "Debug",  "Release", "Distribution" }
    flags { "MultiProcessorCompile" }

   -- ../release/windows/debug/x64/
   outputdir = "%{cfg.system}/%{cfg.buildcfg}/%{cfg.architecture}"
    
    targetdir ("../../../release/" .. outputdir)
    objdir ("../../../release/" .. outputdir)

    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.c"
    }

    includedirs {
        "src/", 
        "../../../packages/",
        "../graphics/src",
        "../../mathematics/src",
    }
    libdirs{
        "../../../bin/lib/%{cfg.buildcfg}/"
    }
    links {
        "graphics",
        "mathematics"
    }

    filter "system:windows"
        
        staticruntime "off"
        systemversion "latest"

        defines { "WIN32" }

    filter "configurations:Debug"
        symbols "on"
        buildoptions "/MDd"
        defines {  "_DEBUG", }
        
    filter "configurations:Release"
        defines {
            "_DEBUG",
        }
        optimize "on"
        symbols "on"
        buildoptions "/MD"

    filter "configurations:Distribution"
        defines {  "NDEBUG" }
        optimize "on"
        symbols "off"
        buildoptions "/MD"
