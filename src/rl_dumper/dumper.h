#pragma once
#include <concepts>
#include <fstream>
#include <memory>
#include <bakkesmod/plugin/bakkesmodplugin.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>

// We need those just for the dump constraints
#include "objects/product.h"

static std::shared_ptr<GameWrapper> s_GlobalGameWrapper;
static char* s_CurrentDir;

inline void SaveToFile(const char* Directory, const char* FileName, nlohmann::json& Json)
{
	char* Path = new char[strlen(Directory) + strlen(FileName) + 1];
	strcpy(Path, Directory);
	strcat(Path, FileName);
	LOG("Dumped to file: {}", Path);

	std::ofstream File(Path, std::fstream::out);
	std::string Content = Json.dump(1);
	File << Content;

	delete[] Path;
}

template<typename Type>
concept Dumpable = requires (nlohmann::json& Json, Type& Val)
{
	{ Json << Val } -> std::same_as<nlohmann::json&>;
};

template<Dumpable Type>
void Dump(ArrayWrapper<Type>& Array, const char* FileName)
{
	nlohmann::json Json = nlohmann::json::array();
	for(int Idx = 0; Idx < Array.Count(); Idx++)
	{
		Type Val = Array.Get(Idx);
		if (!Val)
			continue;

		nlohmann::json Obj;
		Obj << Val;

		Json.push_back(Obj);
	}

	SaveToFile(s_CurrentDir, FileName, Json);
}

template<Dumpable Type>
void Dump(Type& Val, const char* FileName)
{
	nlohmann::json Json;
	Json << Val;

	SaveToFile(s_CurrentDir, FileName, Json);
}

inline void DumpProducts(std::vector<std::string>& Args)
{
	bool ExportTextures = false;
#ifdef EXPORT_TEXTURES
	for (auto& Arg : Args)
	{
		if(Arg == "--textures")
		{
			ExportTextures = true;
		}
	}
#endif

	ItemsWrapper ItemsWrapper = s_GlobalGameWrapper->GetItemsWrapper();
	ArrayWrapper<ProductWrapper> Products = ItemsWrapper.GetAllProducts();

	if(ExportTextures)
	{
		for (int Idx = 0; Idx < Products.Count(); Idx++)
		{
			ProductWrapper Product = Products.Get(Idx);
			if (!Product)
				continue;

			s_GlobalCvarManager->executeCommand(fmt::format("brank_exportthumbnail {}", Product.GetID()), true);
			break;
		}
	}

	Dump(Products, "products.json");
}