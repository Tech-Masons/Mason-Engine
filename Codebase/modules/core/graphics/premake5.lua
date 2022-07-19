project "graphics"
    kind "staticlib"
    
    language "c++"
    cppdialect "c++latest"

    location "."
    
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
        "../mathematics/src",
	   "../platform/src",
    }
    libdirs{
        "../../../bin/lib/%{cfg.buildcfg}/"
    }
    links {
        "d3d12",
        "dxguid",
        "mathematics",
	   "platform"
    }
    defines {
        
    }
    
    filter "system:windows"
        staticruntime "off"
        systemversion "latest"

        defines { "WIN32" }

    filter "configurations:Debug"
        symbols "on"
        buildoptions "/MDd"
        
        defines {  "_DEBUG" }
        
    filter "configurations:Release"
        optimize "on"
        symbols "on"
        buildoptions "/MD"
        
        defines { "_DEBUG" }
    
    filter "configurations:Release"
        optimize "on"
        symbols "off"
        buildoptions "/MD"

        defines { "NDEBUG" }