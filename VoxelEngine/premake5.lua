include "Dependencies.lua"

workspace "GenesisFramework"
	architecture "x86_64"
	startproject "NotEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Engine/vendor/GLFW"
	include "Engine/vendor/glad"
	include "Engine/vendor/imgui"
group ""

group "Core"
	include "Engine"
group ""

group "Tools"
	include "NotEditor"
group ""