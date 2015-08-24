solution "Dukbind"
	configurations { "Debug", "Release" }

project "Dukbind"
	targetname  "dukbind"
	language    "C++"
	kind        "StaticLib"
	flags       { "ExtraWarnings", "StaticRuntime" }
	includedirs { "duktape/src", "include", "src" }

	files
	{
		"src/**.h", "src/**.cpp",
	}

	configuration "Debug"
		targetdir   "lib/debug"
		defines     "_DEBUG"
		flags       { "Symbols" }

	configuration "Release"
		targetdir   "lib/release"
		defines     "NDEBUG"
		flags       { "OptimizeSize" }

	configuration "vs*"
		defines     { "_CRT_SECURE_NO_WARNINGS" }

	configuration "vs2005"
		defines	{"_CRT_SECURE_NO_DEPRECATE" }


	if _ACTION == "clean" then
	end

project "Test"

	targetname  "dukbind_test"
	language    "C++"
	kind        "ConsoleApp"
	flags       { "ExtraWarnings", "StaticRuntime" }
	defines		{ "DUKBIND_TEST" }

	includedirs { "Catch/include", "duktape/src-separate", "include", "src", "tests" }

	files
	{
		"tests/**.cpp",
		"src/**.cpp",
		"duktape/src-separate/*.c"
	}

	configuration "Debug"
		targetdir   "tests/bin/debug"
		defines     "_DEBUG"
		flags       { "Symbols" }

	configuration "Release"
		targetdir   "tests/bin/release"
		defines     "NDEBUG"
		flags       { "OptimizeSize" }

	configuration "vs*"
		defines     { "_CRT_SECURE_NO_WARNINGS" }

	configuration "vs2005"
		defines	{"_CRT_SECURE_NO_DEPRECATE" }

	configuration "linux" or "macosx"
		buildoptions { "--std=c++11" }


if _ACTION == "clean" then
	os.rmdir("lib")
	os.rmdir("tests/bin")
end
