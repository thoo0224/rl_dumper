#pragma once

#include <bakkesmod/plugin/bakkesmodplugin.h>
#include <fmt/format.h>
#include <memory>

static std::shared_ptr<CVarManagerWrapper> s_GlobalCvarWrapper;

#define LOG(Template, ...) \
	if(s_GlobalCvarWrapper) \
		s_GlobalCvarWrapper->log(fmt::format(Template, __VA_ARGS__))