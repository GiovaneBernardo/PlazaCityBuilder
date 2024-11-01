#pragma once
#include "Item.h"
#include <vector>

namespace Plaza {
	class RoadItem : public Item {
	public:
		RoadItem(const std::string& name) : Item(name) {

		}

		void OnHand(Hand& hand) override;
	};
	class ItemsRegister {
	public:
		static inline std::vector<std::shared_ptr<Item>> sItems = std::vector<std::shared_ptr<Item>>();
		static void Init();
	};
}