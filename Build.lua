-- premake5.lua
workspace "AdventOfCode2024"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Day05"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "Day01/Build-Day01.lua"
include "Day02/Build-Day02.lua"
include "Day03/Build-Day03.lua"
include "Day04/Build-Day04.lua"
include "Day05/Build-Day05.lua"