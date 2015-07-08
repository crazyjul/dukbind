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

	includedirs { "Catch/include", "duktape/src", "include", "src", "tests" }

	files
	{
		"tests/main.cpp",
		"duktape/src/*.c"
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


if _ACTION == "clean" then
	os.rmdir("lib")
	os.rmdir("tests/bin")
end
