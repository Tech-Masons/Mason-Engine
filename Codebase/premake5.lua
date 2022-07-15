workspace "Wraith Engine"
    architecture "x64"
    configurations { "Debug", "Pre-Release", "Release" }
    -- outputs 
    
    targetdir ("Build/%{cfg.buildcfg}")
    objdir ("Build/%{cfg.buildcfg}")

    startproject "Editor"


project "Engine"
    -- settings 
    
    kind "staticlib" 
    language "c++"
    cppdialect "c++latest"
    location "codebase/%{prj.name}"

    -- project include files
    files {
        "codebase/%{prj.name}/**.c",
        "codebase/%{prj.name}/**.h",
        "codebase/%{prj.name}/**.cpp",
        "codebase/%{prj.name}/**.hpp",
    }

    -- project defines
    defines {
       
    }

    flags {
        "MultiProcessorCompile"

    }

    -- project include directories
    includedirs {
        "codebase/%{prj.name}/src",             -- project root
        "codebase/%{prj.name}/dependencies/",    -- imgui 
    }

    libdirs {
        
    }
    links { 
        "d3d12.lib",
    }



    filter "configurations:Debug"
        defines { "_DEBUG" }
        -- /MTd
        staticruntime "on"
        runtime "Debug"

    filter "configurations:Release"
        defines { "NDEBUG" }
        -- /MT
        staticruntime "on"
        runtime "Release"
   
    filter "system:Windows"
        defines { "_WINDOWS" }


    filter{}

project "Editor"
    -- settings 
    kind "windowedapp" 
    language "c++"
    cppdialect "c++latest"
    location "codebase/%{prj.name}"

    -- project include files
    files {
        "codebase/%{prj.name}/**.c",
        "codebase/%{prj.name}/**.h",
        "codebase/%{prj.name}/**.cpp",
        "codebase/%{prj.name}/**.hpp",
        "codebase/%{prj.name}/**.hlsl",

    }

    flags {
        "MultiProcessorCompile"
    }
    -- project defines
    defines {
   
    }

    -- project include directories
    includedirs {
        "codebase/%{prj.name}/src/",
        "codebase/Engine/src/",
        "codebase/Engine/dependencies/",   

    }

    libdirs {}
    
    links { "Engine" }


    filter "configurations:Debug"
        defines { "_DEBUG" }
        -- /MTd
        staticruntime "on"
        runtime "Debug"

    filter "configurations:Release"
        defines { "NDEBUG" }
        -- /MT
        staticruntime "on"
        runtime "Release"

    filter "system:Windows"
        defines { "_WINDOWS" }

    filter {"files:**-vert.hlsl"} 
        shadertype "Vertex"
        shadermodel "6.5"
        shaderobjectfileoutput "assets/shaders/bin/%{file.basename}.cso"
        
        filter {"files:**-pixl.hlsl"}
        shadertype "Pixel"
        shadermodel "6.5"
        shaderobjectfileoutput "assets/shaders/bin/%{file.basename}.cso"
        
    filter{}
