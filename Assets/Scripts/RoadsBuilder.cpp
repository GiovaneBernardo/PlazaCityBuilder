#include "Engine/Core/PreCompiledHeaders.h"
#include "RoadsBuilder.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Core/Input/Cursor.h"
//#include "Engine/Core/Scripting/CppHelper.h
//#include "Engine/Core/Script.h"
//#include "Engine/Core/Scene.h"
//#include "Engine/Core/Physics.h"
//#include "Engine/Components/Physics/RigidBody.h"
//#include "Engine/Components/Physics/Collider.h"

namespace Plaza {
	class MyQueryFilterCallback2 : public physx::PxQueryFilterCallback {
	public:
		virtual physx::PxQueryHitType::Enum preFilter(
			const physx::PxFilterData& filterData,
			const physx::PxShape* shape,
			const physx::PxRigidActor* actor,
			physx::PxHitFlags& queryFlags) override
		{
			if (actor && actor->userData == mEntityToIgnore) {
				return physx::PxQueryHitType::eNONE;
			}
			return physx::PxQueryHitType::eBLOCK;
		}

		// Set the entity to ignore
		void setEntityToIgnore(void* entityToIgnore) {
			mEntityToIgnore = entityToIgnore;
		}

		physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor) override {
			return physx::PxQueryHitType::eTOUCH;
		}

	private:
		void* mEntityToIgnore = nullptr;
	};

	Mesh* CreateRoadMesh(unsigned int x, unsigned int y, unsigned int z) {
		std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
		std::vector<unsigned int> indices = std::vector<unsigned int>();//x * z * 3
		std::vector<glm::vec3> normals = std::vector<glm::vec3>();
		std::vector<glm::vec2> uvs = std::vector<glm::vec2>();

		for (unsigned int i = 0; i < x; ++i) {
			for (unsigned int j = 0; j < z; ++j) {
				vertices.push_back(glm::vec3(i, 1.0f, j));
				normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
				uvs.push_back(glm::vec2(float(i % 10) / float(10), float(j % 10) / float(10)));
			}
		}

		int depth = x;
		int width = z;

		for (int z = 0; z < depth - 1; z++) {
			for (int x = 0; x < width - 1; x++) {
				int bottomLeft = z * width + x;
				int topleft = (z + 1) * width + x;
				int topRight = (z + 1) * width + x + 1;
				int bottomRight = z * width + x + 1;

				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topleft);

				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
				indices.push_back(topRight);
			}
		}

		// Recalculate normals
		for (int i = 0; i < indices.size(); i += 3) {
			normals[indices[i]] = glm::vec3(0.0f);
			normals[indices[i + 1]] = glm::vec3(0.0f);
			normals[indices[i + 2]] = glm::vec3(0.0f);
		}

		for (int i = 0; i < indices.size(); i += 3) {
			int index0 = indices[i];
			int index1 = indices[i + 1];
			int index2 = indices[i + 2];
			glm::vec3 v1 = vertices[index1] - vertices[index0];
			glm::vec3 v2 = vertices[index2] - vertices[index0];
			glm::vec3 normal = glm::cross(v1, v2);
			normal = glm::normalize(normal);
			normals[index0] += normal;
			normals[index1] += normal;
			normals[index2] += normal;
		}

		std::vector<glm::vec3> tangents;
		std::vector<unsigned int> materials{ 0 };
		Mesh* mesh = Application::Get()->mRenderer->CreateNewMesh(vertices, normals, uvs, tangents, indices, materials, false, {}, {});
		return mesh;
	}

	void RoadsBuilder::Init() {
		sCameraEntity = CppHelper::FindEntity("CameraEntity");
		sRoadPreviewEntity = Scene::GetActiveScene()->GetEntityByName("RoadPreview");

		// Add a new mesh for the road preview
		MeshRenderer* meshRenderer = sRoadPreviewEntity->GetComponent<MeshRenderer>();
		Mesh* newMesh = CreateRoadMesh(10, 10, 10);//new Mesh();
		AssetsManager::AddMesh(newMesh);
		meshRenderer->ChangeMesh(newMesh);
	}

	void RoadsBuilder::Update(Hand& hand) {
		RaycastHit hit = RaycastAtMousePosition();

		if (!hit.missed)
			sRoadPreviewEntity->GetComponent<Transform>()->SetRelativePosition(hit.point);

		RoadsBuilder::UpdateRoadPreview();

		static bool pressingMouseLastFrame = false;
		if (Input::GetMouseDown(0)) {
			if (!pressingMouseLastFrame) {
				sSelectingPoint += 1;
				RoadsBuilder::Click();
				pressingMouseLastFrame = true;
			}
		}
		else
			pressingMouseLastFrame = false;
	}

	void RoadsBuilder::Click() {
		switch (sSelectingPoint) {
		case 0: PlaceFirstPoint(); break;
		case 1: PlaceSecondPoint(); break;
		case 2: PlaceThirdPoint(); break;
		}
	}

	void RoadsBuilder::PlaceFirstPoint() {
		RaycastHit hit = RaycastAtMousePosition();
		sPoints.push_back(hit.point);
	}

	void RoadsBuilder::PlaceSecondPoint() {
		RaycastHit hit = RaycastAtMousePosition();
		sPoints.push_back(hit.point);

		Mesh* roadMesh = sRoadPreviewEntity->GetComponent<MeshRenderer>()->mesh;
		std::random_device rd;
		std::mt19937 gen(rd());
		int min = 3;
		int max = 10;
		std::uniform_int_distribution<int> dist(min, max);
		int random_integer = dist(gen);
		*roadMesh = *CreateRoadMesh(dist(gen), dist(gen), dist(gen));
		Mesh* startedMesh = RoadsBuilder::GetStraightRoadMesh(sPoints[0], sPoints[1], 3.0f);//VulkanRenderer::GetRenderer()->RestartMesh(roadMesh);
		sRoadPreviewEntity->GetComponent<MeshRenderer>()->ChangeMesh(startedMesh);



		sPoints.clear();
		sSelectingPoint = -1;
	}

	void RoadsBuilder::PlaceThirdPoint() {
		RaycastHit hit = RaycastAtMousePosition();
		sPoints.push_back(hit.point);
	}

	void RoadsBuilder::UpdateRoadPreview() {
		if (sSelectingPoint != 0)
			return;

		//Mesh* roadMesh = sRoadPreviewEntity->GetComponent<MeshRenderer>()->mesh;
		//roadMesh = VulkanRenderer::GetRenderer()->RestartMesh(roadMesh);
	}

	RaycastHit RoadsBuilder::RaycastAtMousePosition() {
		glm::vec3 rayDirection = sCameraEntity->GetComponent<Camera>()->ScreenPositionToRay(Input::Cursor::GetMousePosition(), Input::GetScreenSize());

		RaycastHit hit = RaycastHit();
		Physics::Raycast(sCameraEntity->GetComponent<Transform>()->GetWorldPosition(), rayDirection, hit);
		return hit;
	}
}