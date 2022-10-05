project "Optick"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines
	{
		--"OPTICK_ENABLE_GPU",
		--"OPTICK_ENABLE_GPU_VULKAN",
		--"OPTICK_ENABLE_GPU_D3D12"
	}


	files
	{
		"src/**.cpp",
		"include/**.h"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"