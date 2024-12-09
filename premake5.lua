-- premake5.lua
workspace "AdventOfCode2024"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Day08"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "Day01/Build-Day01.lua"
include "Day02/Build-Day02.lua"
include "Day03/Build-Day03.lua"
include "Day04/Build-Day04.lua"
include "Day05/Build-Day05.lua"
include "Day06/Build-Day06.lua"
include "Day07/Build-Day07.lua"
include "Day08/Build-Day08.lua"
-- include "Day09/Build-Day09.lua"
-- include "Day10/Build-Day10.lua"
-- include "Day11/Build-Day11.lua"
-- include "Day12/Build-Day12.lua"
-- include "Day13/Build-Day13.lua"
-- include "Day14/Build-Day14.lua"
-- include "Day15/Build-Day15.lua"
-- include "Day16/Build-Day16.lua"
-- include "Day17/Build-Day17.lua"
-- include "Day18/Build-Day18.lua"
-- include "Day19/Build-Day19.lua"
-- include "Day20/Build-Day20.lua"
-- include "Day21/Build-Day21.lua"
-- include "Day22/Build-Day22.lua"
-- include "Day23/Build-Day23.lua"