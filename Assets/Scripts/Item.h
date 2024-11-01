#pragma once
#include <cstdint>
#include <string>
#include "Engine/Core/UUID.h"

namespace Plaza {
	class Hand;
	class Item {
	public:
		uint64_t mItemUuid = 0;
		std::string mItemName = "";

		Item() { mItemUuid = UUID::NewUUID(); };
		Item(const std::string& name) {
			mItemName = name;
			mItemUuid = UUID::NewUUID();
		}
		virtual void OnHand(Hand& hand) {}
	};
}