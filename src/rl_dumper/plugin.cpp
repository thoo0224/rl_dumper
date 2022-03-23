#include "pch.h"

#include <chrono>
#include "logger.h"
#include "plugin.h"
#include "dumper.h"

BAKKESMOD_PLUGIN(Plugin, "dumper", "1.0.0", PLUGINTYPE_REPLAY)

using namespace std::chrono;

#define MEASURE_FUNC(Function, ...) \
		auto now = high_resolution_clock::now(); \
		Function(__VA_ARGS__); \
		auto end = high_resolution_clock::now(); \
		auto duration = duration_cast<milliseconds>(end - now); \
		LOG("{} took {} ms", #Function, duration.count()); \

#define BIND_STATIC(Function) \
	[this](std::vector<std::string> _) { \
		MEASURE_FUNC(Function) \
	}

#define BIND_STATIC_ARGS(Function) \
	[this](std::vector<std::string> Args) { \
		MEASURE_FUNC(Function, Args) \
	}

void Plugin::onLoad()
{
	s_GlobalCvarManager = this->cvarManager;
	s_GlobalGameWrapper = this->gameWrapper;

	std::filesystem::path Path = std::filesystem::current_path();
	const char* PathStr = Path.string().c_str();
	s_CurrentDir = new char[strlen(PathStr) + 2];
	strcpy(s_CurrentDir, PathStr);
	strcat(s_CurrentDir, "\\");

	this->cvarManager->registerNotifier("dump_products", 
		BIND_STATIC_ARGS(DumpProducts), "dumper_dump_products", PERMISSION_ALL);
}