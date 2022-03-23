#pragma once

#include <bakkesmod/plugin/bakkesmodplugin.h>
#include <fmt/format.h>
#include <memory>

static std::shared_ptr<CVarManagerWrapper> s_GlobalCvarManager;

#define LOG(Template, ...) \
	if(s_GlobalCvarManager) \
		s_GlobalCvarManager->log(fmt::format(Template, __VA_ARGS__))