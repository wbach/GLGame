#include "LightPassShader.h"

void CLightPassShader::Init()
{
	InitShaderProgram("Data/Shaders/LightPassShader.vert", "Data/Shaders/LightPassShader.frag");
	Start();
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}
void CLightPassShader::GetAllUniformLocations()
{
	location_ScreenSize = GetUniformLocation("ScreenSize");

	location_TransformationMatrix = GetUniformLocation("TransformationMatrix");
	location_ProjectionMatrix = GetUniformLocation("ProjectionMatrix");
	location_ViewMatrix = GetUniformLocation("ViewMatrix");

	// Textures
	location_PositionMap = GetUniformLocation("PositionMap");
	location_ColorMap = GetUniformLocation("ColorMap");
	location_NormalMap = GetUniformLocation("NormalMap");

	// Lights 
	location_NumberOfLights = GetUniformLocation("NumberOfLights");
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		char tmpVariableName[50]; ; memset(tmpVariableName, 0, 50);

		sprintf(tmpVariableName, "lights[%i].m_Type", i);
		location_LightType.push_back(GetUniformLocation(tmpVariableName));

		sprintf(tmpVariableName, "lights[%i].m_Position", i);
		location_LightPosition.push_back(GetUniformLocation(tmpVariableName));

		sprintf(tmpVariableName, "lights[%i].m_Colour", i);
		location_LightColour.push_back(GetUniformLocation(tmpVariableName));

		sprintf(tmpVariableName, "lights[%i].m_Attenuation", i);
		location_LightAttenuation.push_back(GetUniformLocation(tmpVariableName));

		sprintf(tmpVariableName, "lights[%i].m_CutOff", i);
		location_LightCutOff.push_back(GetUniformLocation(tmpVariableName));
	}
}

void CLightPassShader::BindAttributes()
{
	BindAttribute(0, "Position");
	BindAttribute(1, "TexCoord");
	BindAttribute(2, "Normal");
	BindAttribute(3, "Tangent");
}
void CLightPassShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_TransformationMatrix, matrix);
}

void CLightPassShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ProjectionMatrix, matrix);
}

void CLightPassShader::LoadViewMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ViewMatrix, matrix);
}
void CLightPassShader::LoadLightNumber(const int& i) const
{
	LoadValue(location_NumberOfLights, i);
}
void CLightPassShader::LoadLight(const CLight& light, const int& i) const
{
	LoadValue(location_LightType[i], light.GetType());
	LoadValue(location_LightPosition[i], light.GetPosition());
	LoadValue(location_LightColour[i], light.GetColour());
	LoadValue(location_LightAttenuation[i], light.GetAttenuation());
	LoadValue(location_LightCutOff[i], light.GetAttenuation());
}

void CLightPassShader::LoadScreenSize(glm::vec2 screen_size)
{
	LoadValue(location_ScreenSize, screen_size);
}

void CLightPassShader::ConnectTextureUnits() const
{
	LoadValue(location_PositionMap, 0);
	LoadValue(location_ColorMap, 1);
	LoadValue(location_NormalMap, 2);
}
