#pragma once
#include "../Camera/Camera.h"
#include "../Terrain/Terrain.h"

class CMousePicker 
{
public:
	CMousePicker();
	CMousePicker(shared_ptr<CCamera> cam, glm::vec2 window_size, glm::mat4 projection);
	glm::vec3 GetMousePointOnTerrain(glm::vec2 mous_pose, CTerrain& terrain, bool& is_col);
private:
	glm::vec3 CalculateMouseRay(glm::vec2 mouse_pos);
	glm::vec3 ToWorldCoords(glm::vec4  eyeCoords);
	glm::vec4 ToEyeCoords(glm::vec4 clipCoords);
	glm::vec2 GetNormalisedDeviceCoordinates(float mouseX, float mouseY);

	//**********************************************************

	glm::vec3 GetPointOnRay(glm::vec3 ray, float distance);
	glm::vec3 BinarySearch(CTerrain& terrain, int count, float start, float finish, glm::vec3 ray);
	bool IntersectionInRange(float start, float finish, glm::vec3 ray, CTerrain& terrain);
	bool IsUnderGround(glm::vec3 testPoint, CTerrain& terrain);

	int			RECURSION_COUNT;
	float		RAY_RANGE;
	glm::vec3	m_CurrentRay;

	glm::mat4	m_ProjectionMatrix;
	shared_ptr<CCamera>	m_Camera;

	glm::vec2	m_WindowSize;

};