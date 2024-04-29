project "NotEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/NotVoxel/vendor/spdlog/include",
		"%{wks.location}/NotVoxel/src",
		"%{wks.location}/NotVoxel/vendor",
		"%{IncludeDir.Glm}"
	}

	links
	{
		"NotVoxel"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "VOXEL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "VOXEL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "VOXEL_DIST"
		runtime "Release"
		optimize "on"