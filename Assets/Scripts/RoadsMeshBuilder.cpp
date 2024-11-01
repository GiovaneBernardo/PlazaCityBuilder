#include "Engine/Core/PreCompiledHeaders.h"
#include "RoadsBuilder.h"

namespace Plaza {
	Mesh* RoadsBuilder::GetStraightRoadMesh(const glm::vec3& point1, const glm::vec3& point2, float width) {
		std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
		std::vector<unsigned int> indices = std::vector<unsigned int>();
		std::vector<glm::vec3> normals = std::vector<glm::vec3>();
		std::vector<glm::vec2> uvs = std::vector<glm::vec2>();


		vertices.push_back(glm::vec3(point1.x - width, point1.y, point1.z));
		vertices.push_back(glm::vec3(point1.x + width, point1.y, point1.z));

		vertices.push_back(glm::vec3(point2.x - width, point1.y, point1.z));
		vertices.push_back(glm::vec3(point2.x + width, point1.y, point1.z));

		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(1, 1));

		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);

		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

		std::vector<glm::vec3> tangents;
		std::vector<unsigned int> materials{ 0 };
		Mesh* mesh = Application::Get()->mRenderer->CreateNewMesh(vertices, normals, uvs, tangents, indices, materials, false, {}, {});
		return mesh;
	}
}