#pragma once
#include <vector>
namespace DebugMeshes
{
	enum Ids
	{
		SPHERE_PLANE = 0,
		COUNT
	};
}
class CDebugRenderer
{
public:
	//void Init()
	int m_ModelsId[DebugMeshes::COUNT];
};