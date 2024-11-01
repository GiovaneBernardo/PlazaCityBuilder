#pragma once
#include "Item.h"
#include "Hand.h"
#include "ItemsRegister.h"
#include "Engine/Core/Scripting/CppHelper.h"


namespace Plaza {
	class RoadsBuilder {
	public:
		static inline int sSelectingPoint = -1;
		static inline std::vector<glm::vec3> sPoints = std::vector<glm::vec3>();
		static inline Entity* sCameraEntity = nullptr;
		static inline Entity* sCameraTransform = nullptr;
		static inline Entity* sRoadPreviewEntity = nullptr;
		static void Init();
		static void Update(Hand& hand);
		static void Click();
		static void UpdateRoadPreview();

		static Mesh* GetStraightRoadMesh(const glm::vec3& point1, const glm::vec3& point2, float width);
	private:
		static void PlaceFirstPoint();
		static void PlaceSecondPoint();
		static void PlaceThirdPoint();

		static RaycastHit RaycastAtMousePosition();
	};
}