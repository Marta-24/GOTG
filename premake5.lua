workspace "Wiwa"
	architecture "x86_64"
	startproject "WiwaEditor"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["Wiwa"] = "%{wks.location}/Wiwa/src"
IncludeDirs["GLFW"] = "%{wks.location}/Wiwa/vendor/GLFW/include"
IncludeDirs["Glew"] = "%{wks.location}/Wiwa/vendor/Glew/include/GL"
IncludeDirs["ImGui"] = "%{wks.location}/Wiwa/vendor/imgui"
IncludeDirs["GLM"] = "%{wks.location}/Wiwa/vendor/glm"
IncludeDirs["PCG"] = "%{wks.location}/Wiwa/vendor/PCG/include"
IncludeDirs["JSON"] = "%{wks.location}/Wiwa/vendor/rapidjson"
IncludeDirs["XML"] = "%{wks.location}/Wiwa/vendor/pugixml/src"
IncludeDirs["mono"] = "%{wks.location}/Wiwa/vendor/mono/include"
IncludeDirs["SPDLOG"] = "%{wks.location}/Wiwa/vendor/spdlog/include"
IncludeDirs["Optick"] = "%{wks.location}/Wiwa/vendor/Optick/include"
IncludeDirs["assimp"] = "%{wks.location}/Wiwa/vendor/assimp/include"
IncludeDirs["ImGuizmo"] = "%{wks.location}/Wiwa/vendor/imguizmo"
IncludeDirs["GLI"] = "%{wks.location}/Wiwa/vendor/gli/gli"

LibraryDirs = {}
LibraryDirs["mono"] = "%{wks.location}/Wiwa/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDirs.mono}/libmono-static-sgen.lib"

Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["Version"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"


group "Dependencies"
	include "Wiwa/vendor/GLFW"
	include "Wiwa/vendor/imgui"
	include "Wiwa/vendor/Optick"
	include "Wiwa/vendor/imguizmo"
group ""

group "Core"
	include "Wiwa"
	include "Wiwa-ScriptCore"
group ""

include "WiwaEditor"

