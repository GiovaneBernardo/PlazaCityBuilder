#pragma once
#include "Engine/Core/Scripting/CppScriptFactory.h"
#include "Engine/Components/Core/Transform.h"
#include "Hand.h"

namespace Plaza {
	class PlayerScript : public CppScript {
	public:
		Hand mHand = Hand();

		Transform* bodyTransform = nullptr;
		Transform* cameraTransform = nullptr;
		float yaw = 0.0f;
		float pitch = 0.0f;
		void OnStart();
		void OnUpdate();

	private:
		void MoveCamera();
	};
	PL_REGISTER_SCRIPT(PlayerScript);
}