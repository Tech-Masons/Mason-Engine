project "editor"
    kind "consoleapp"
    language "c++"
    cppdialect "c++latest"
    
    location "."   

    -- ../release/windows/debug/x64/
    outputdir = "%{cfg.system}/%{cfg.buildcfg}/%{cfg.architecture}"
    
    targetdir ("../../release/" .. outputdir)
    objdir ("../../release/" .. outputdir)

    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.c"
    }



    includedirs {
        "src/",
        "../../packages/",

        -- core engine
        "../../modules/core/core/src",      
        "../../modules/core/graphics/src",
        "../../modules/mathematics/src",
    
        -- engine tests
        "../../tests/src/"
    }
    libdirs{
        "../../bin/lib/"
    }
    links {
        "core",
        "mathematics",
        "graphics",
        "tests"
    }
    defines{
        
    }
    

    filter "system:windows"
        staticruntime "off"
        systemversion "latest"
        postbuildcommands(
            "xcopy /E /I  \"assets\\\" \"../../release/".. (outputdir) .. "\\\" /Y"
        );
        defines {
            "WIN32",
        }
    filter "configurations:Debug"
        defines { 
            "_DEBUG"
        }
        symbols "on"
        buildoptions "/MDd"
        
    filter "configurations:Release"
        defines {
            "_DEBUG"
        }
        optimize "on"
        symbols "on"
        buildoptions "/MD"
    filter "configurations:Release"
        defines {
            "NDEBUG"
        }
        optimize "on"
        symbols "off"
        buildoptions "/MD"
    
