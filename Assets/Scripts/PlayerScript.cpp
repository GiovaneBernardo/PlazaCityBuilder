#include "PlayerScript.h"
#include "Engine/Core/PreCompiledHeaders.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Core/Input/Cursor.h"
#include "Engine/Core/Time.h"
#include "ItemsRegister.h"
#include "RoadsBuilder.h"
//#include "Engine/Core/Scripting/CppHelper.h

namespace Plaza {
	void PlayerScript::OnStart() {
		bodyTransform = Scene::GetActiveScene()->GetComponent<Transform>(Scene::GetActiveScene()->GetEntityByName("Body")->uuid);
		cameraTransform = Scene::GetActiveScene()->GetComponent<Transform>(Scene::GetActiveScene()->GetEntityByName("CameraEntity")->uuid);
		ItemsRegister::Init();
		RoadsBuilder::Init();
		this->mHand.mItemInHand = ItemsRegister::sItems[0].get();

	}
	void PlayerScript::OnUpdate() {
		this->MoveCamera();
		this->mHand.mItemInHand->OnHand(mHand);
		//Plaza::cpp
	}

	void PlayerScript::MoveCamera() {
		float speed = 500.0f * Time::deltaTime;
		glm::vec3 movement = glm::vec3(0.0f);
		if (Input::GetKeyDown(GLFW_KEY_W))
			movement.z += 1.0f * speed;
		if (Input::GetKeyDown(GLFW_KEY_S))
			movement.z += -1.0f * speed;
		if (Input::GetKeyDown(GLFW_KEY_A))
			movement.x += 1.0f * speed;
		if (Input::GetKeyDown(GLFW_KEY_D))
			movement.x += -1.0f * speed;

		bodyTransform->MoveTowards(movement);

		if (Application::Get()->focusedMenu == "Scene" && Input::GetMouseDown(1) == GLFW_PRESS) {
			glm::vec2 mouseDelta = glm::vec2(Input::Cursor::deltaX, Input::Cursor::deltaY);
			float sensitivity = 0.3f * Time::deltaTime;
			bodyTransform->SetRelativeRotation(bodyTransform->GetLocalQuaternion() * glm::quat(glm::vec3(0.0f, -mouseDelta.x * sensitivity, 0.0f)));
			cameraTransform->SetRelativeRotation(cameraTransform->GetLocalQuaternion() * glm::quat(glm::vec3(mouseDelta.y * sensitivity, 0.0f, 0.0f)));
		}
	}
}