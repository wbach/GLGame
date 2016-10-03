#version 330

in vec3 TextureCoords;

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 MaterialSpecular;  

uniform samplerCube DayCubeMap;
uniform samplerCube NightCubeMap;
uniform float		BlendFactor ;
uniform vec3		FogColour ;

const float LowerLimit = 0;
const float UpperLimit = 0.1;

void main(void)
{
    vec4 texture1	  = texture(DayCubeMap, TextureCoords);
    vec4 texture2	  = texture(NightCubeMap, TextureCoords);
	vec4 final_colour = mix(texture1, texture2, BlendFactor);

    float factor	= (TextureCoords.y - LowerLimit)/(UpperLimit - LowerLimit );
    factor			= clamp(factor, 0.f, 1.f);
    final_colour	= mix(vec4(FogColour, 1.f), final_colour, factor);

	WorldPosOut      = TextureCoords;					
	DiffuseOut       = final_colour.rgb;	
	NormalOut        = vec3(0.f, 1.f, 0.f);					
	MaterialSpecular = vec3(0.f);
}