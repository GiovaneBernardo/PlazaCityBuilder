#include "ItemsRegister.h"
#include "Engine/Core/UUID.h"
#include "RoadsBuilder.h"
#include "Hand.h"

namespace Plaza {
	void ItemsRegister::Init() {
		sItems.push_back(std::make_shared<RoadItem>("Road1"));
	}

	void RoadItem::OnHand(Hand& hand) {
		RoadsBuilder::Update(hand);
	}
}