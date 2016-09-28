#version 400
#define MAX_LIGHTS 10

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords ;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
uniform mat4 transformationMatrix ;
uniform mat4 projectionMatrix ;
uniform mat4 viewMatrix ;
uniform float isShadows ;
uniform float numberOfLights ;
uniform vec3 lightPosition[MAX_LIGHTS] ;
uniform float useNormalMap;
uniform float useFakeLighting ;
uniform float viewDistance ;
uniform mat4 toShadowMapSpace ;
uniform vec4 plane ;
out vec2 textCoords ;

out float nLights ;
out vec3 surfaceNormal ;
out vec3 toLightVector[MAX_LIGHTS];
out vec3 toCameraVector ;
out vec3 pass_tangent ;
out float useNormalMapping ;
out float skipRender ;
out vec4 shadowCoords ;


//texture atlases
uniform float numberOfRows ;
uniform vec2 offset;

out float visibility;				//Fog parametr
const float density = 0.0025 ;
const float gradient = 2.5 ;
const float shadowDistance = 150.0 ;
const float transitionDistance = 10.0;
void main()
{
	useNormalMapping = useNormalMap;
	pass_tangent = vec3(0,0,0) ;
	if( useNormalMap > 0.5) pass_tangent = (transformationMatrix * vec4(tangent, 0.0)).xyz; 
	 
	vec4 worldPosition = transformationMatrix * vec4(position,1.0) ;
	gl_ClipDistance[0] = dot(worldPosition,plane) ;
	
	mat4 modelViewMatrix = viewMatrix * transformationMatrix;
	vec4 positionRelativeToCam = modelViewMatrix * vec4(position,1.0);
	gl_Position = projectionMatrix * positionRelativeToCam;	
	
	
	if (useFakeLighting > 0.5)
    {
        surfaceNormal = vec3(0.0f,1.0f,0.0f) ;
    }
	else{
		surfaceNormal = (transformationMatrix * vec4(normal,0.0)).xyz;
	}
	nLights = numberOfLights;
	textCoords = (textureCoords/numberOfRows) + offset ;
		
	for (int i=0 ; i<nLights ; i++){
		toLightVector[i] = lightPosition[i] - worldPosition.xyz ;
	}

    
	
	toCameraVector = (inverse(viewMatrix) * vec4(0,0,0,1)).xyz - worldPosition.xyz ;
	//Fog parametr
	float distance = length(positionRelativeToCam.xyz) ;
    visibility = exp(-pow((distance*((1.5/viewDistance))),gradient));
    visibility = clamp(visibility,0.0f,1.0f) ;
	
	if(isShadows > 0.5f) {
		shadowCoords = toShadowMapSpace*worldPosition;
		float shadDistance = distance - (shadowDistance - transitionDistance);
		shadDistance = shadDistance / transitionDistance;
		shadowCoords.w = clamp(1.0 - shadDistance,0.0, 1.0);
	}else 
		shadowCoords = vec4(-1,0,0,0);
	
	float vDistance = distance - (viewDistance - transitionDistance);
    vDistance = vDistance / transitionDistance;
    skipRender = clamp(1.0 - vDistance,0.0, 1.0);
	
}