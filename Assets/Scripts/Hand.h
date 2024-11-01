#pragma once
#include "Engine/Core/Scripting/CppScriptFactory.h"
#include "Item.h"

namespace Plaza {
	class Hand {
	public:
		Item* mItemInHand = nullptr;

		void Update() {
			mItemInHand->OnHand(*this);
		}
	};
}