workspace "Mason Engine"
    architecture "x64"

    -- any of the iRunables should be considered for a startup project
    -- this is just temporary as we are setting up the debugging enviorment
    -- if the project doesnt start right off the bat its probably because VS is being 
    -- a pain and set core as the startup project...
    startproject "modules/tool/editor"
    
    configurations { "Debug",  "Release", "Distribution" }

    flags { "MultiProcessorCompile" }
    
    -- core modules
    include "modules/core/graphics"
    include "modules/core/core"
    -- include "modules/core/audio"
    -- include "modules/core/input"
    
    -- other modules
    include "modules/physics"
    include "modules/mathematics"
    
    -- tool modules
    include "tools/editor"
    
    -- test module
    include "tests"

   