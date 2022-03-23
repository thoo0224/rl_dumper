#pragma once
#include <bakkesmod/plugin/bakkesmodplugin.h>
#include <nlohmann/json.hpp>

#define SWITCH_RETURN_NAME SWITCH_RETURN_VAL
#define SWITCH(VAL, DEFAULT_VAL) auto SWITCH_RETURN_NAME = DEFAULT_VAL; \
    switch(VAL) {
#define CASE(IDX, RETURN) case IDX: \
    SWITCH_RETURN_VAL = (RETURN); \
    break;
#define END_SWITCH }

inline const char* GetQualityDisplayName(const unsigned char Quality)
{
	SWITCH(Quality, "Unkown")
		CASE(0, "Common")
		CASE(1, "Uncommon")
		CASE(2, "Rare")
		CASE(3, "Very Rare")
		CASE(4, "Import")
		CASE(5, "Exotic")
		CASE(6, "Black Market")
		CASE(7, "Premium")
		CASE(8, "Limited")
		CASE(9, "Legacy")
	END_SWITCH

	return SWITCH_RETURN_NAME;
}

inline nlohmann::json& operator<<(nlohmann::json& Json, ArrayWrapper<ProductAttributeWrapper>& ProductAttributes)
{
	nlohmann::json Array = nlohmann::json::array();
	for (int Idx = 0; Idx < ProductAttributes.Count(); Idx++)
	{
		ProductAttributeWrapper Attribute = ProductAttributes.Get(Idx);
		if (!Attribute)
			continue;

		nlohmann::json Attr;
		Attr["Label"] = Attribute.GetLabel().ToString();
		Attr["Type"] = Attribute.GetAttributeType();
		Attr["TypeName"] = Attribute.GetTypename();

		Array.push_back(Attr);
	}

	Json["Attributes"] = Array;
	return Json;
}

inline nlohmann::json& operator<<(nlohmann::json& Json, ProductWrapper& Product)
{
	Json["Id"] = Product.GetID();
	Json["Label"] = Product.GetLabel().ToString();

	nlohmann::json Slot;
	Slot["Label"] = Product.GetDisplayLabelSlot().ToString();
	Slot["Index"] = Product.GetSlot().GetSlotIndex();
	Json["Slot"] = Slot;

	nlohmann::json Asset;
	Asset["ThumbnailAssetPath"] = Product.GetThumbnailAssetPath().ToString();
	Asset["ThumbnailAssetName"] = Product.GetThumbnailAssetName();
	Asset["AssetPackageName"] = Product.GetAssetPackageName();
	Asset["AssetPath"] = Product.GetAssetPath().ToString();
	Json["Asset"] = Asset;

	nlohmann::json Quality;
	Quality["Index"] = Product.GetQuality();
	Quality["Label"] = GetQualityDisplayName(Product.GetQuality());
	Json["Quality"] = Quality;

	Json["SortLabel"] = Product.GetSortLabel().ToString();
	Json["TrademarkLabel"] = Product.GetTrademarkLabel().ToString();
	Json["LongLabel"] = Product.GetLongLabel().ToString();

	Json["IsBlueprint"] = Product.IsBlueprint();
	Json["IsPaintable"] = Product.IsPaintable();
	Json["IsLicensed"] = Product.IsLicensed();
	Json["IsSchematic"] = Product.IsSchematic();
	Json["IsPlatformExclusive"] = Product.IsPlatformExclusive();
	Json["IsCurrency"] = Product.IsCurrency();
	Json["IsContainer"] = Product.IsContainer();
	Json["IsContainerKey"] = Product.IsContainerKey();
	Json["IsContainerUnlocked"] = Product.IsContainerUnlocked();

	ArrayWrapper<ProductAttributeWrapper> Attributes = Product.GetAttributes();
	Json << Attributes;

	return Json;
}