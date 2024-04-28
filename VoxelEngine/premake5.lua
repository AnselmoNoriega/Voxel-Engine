include "Dependencies.lua"

workspace "VoxelEngine"
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
	include "NotVoxel/vendor/GLFW"
	include "NotVoxel/vendor/glad"
	include "NotVoxel/vendor/imgui"
group ""

group "Core"
	include "NotVoxel"
group ""

group "Tools"
	include "NotEditor"
group ""