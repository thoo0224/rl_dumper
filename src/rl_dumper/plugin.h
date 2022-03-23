#pragma once

#include <bakkesmod/plugin/bakkesmodplugin.h>

class Plugin final : BakkesMod::Plugin::BakkesModPlugin
{
public:
	virtual ~Plugin() = default;

	void onLoad() override;
};