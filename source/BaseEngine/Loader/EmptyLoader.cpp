#include "EmptyLoader.h"

CEmptyLoader::CEmptyLoader()
	: CModel()
{
}

void CEmptyLoader::InitModel(string file_name)
{
}

void CEmptyLoader::CleanUp()
{
	for (CMesh& mesh : m_Meshes)
	{
		mesh.CleanUp();
	}
	m_Meshes.clear();
}
