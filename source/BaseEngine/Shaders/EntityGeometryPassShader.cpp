#include "EntityGeometryPassShader.h"

void CEntityGeometryPassShader::Init()
{
	InitShaderProgram("Data/Shaders/EntityGeometryPassShader.vert", "Data/Shaders/EntityGeometryPassShader.frag");
	Start();
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}
void CEntityGeometryPassShader::GetAllUniformLocations()
{
	location_TransformationMatrix	= GetUniformLocation("TransformationMatrix");
	location_ProjectionMatrix		= GetUniformLocation("ProjectionMatrix");
	location_ViewMatrix				= GetUniformLocation("ViewMatrix");

	location_UseNormalMap	= GetUniformLocation("IsUseNormalMap");
	//Textures
	location_ModelTexture	= GetUniformLocation("gColorMap");
	location_NormalMap		= GetUniformLocation("NormalMap");
	location_ShadowMap		= GetUniformLocation("ShadowMap");

	//Mesh Material
	location_MaterialAmbient  = GetUniformLocation("ModelMaterial.m_Ambient");
	location_MaterialDiffuse  = GetUniformLocation("ModelMaterial.m_Diffuse");
	location_MaterialSpecular = GetUniformLocation("ModelMaterial.m_Specular");
}
void CEntityGeometryPassShader::ConnectTextureUnits()
{
	LoadValue(location_ModelTexture, 0);
	LoadValue(location_NormalMap, 1);
	LoadValue(location_ShadowMap, 2);
}
void CEntityGeometryPassShader::BindAttributes()
{
	BindAttribute(0, "Position");
	BindAttribute(1, "TexCoord");
	BindAttribute(2, "Normal");
	BindAttribute(3, "Tangent");
}
void CEntityGeometryPassShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_TransformationMatrix, matrix);
}

void CEntityGeometryPassShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ProjectionMatrix, matrix);
}

void CEntityGeometryPassShader::LoadViewMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ViewMatrix, matrix);
}
void CEntityGeometryPassShader::LoadUseNormalMap(const float& use) const
{
	LoadValue(location_UseNormalMap, use);
}

void CEntityGeometryPassShader::LoadMeshMaterial(const SMaterial& material) const
{
	LoadValue(location_MaterialAmbient, material.ambient);
	LoadValue(location_MaterialDiffuse, material.diffuse);
	LoadValue(location_MaterialSpecular, material.specular);
}
