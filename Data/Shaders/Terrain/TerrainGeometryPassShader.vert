#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in vec3 Normal;                                               
layout (location = 3) in vec3 Tangent;

uniform mat4 TransformationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ToShadowMapSpace;
                                        
out vec2 TexCoord0;                                                                 
out vec3 Normal0;                                                                   
out vec3 WorldPos0;                                                                 
out vec4 ShadowCoords;


void main()
{    
	vec4 model_view_position  = ViewMatrix * TransformationMatrix * vec4(Position, 1.0);
    gl_Position    = ProjectionMatrix * model_view_position;
    TexCoord0      = TexCoord;                  
    Normal0        = (TransformationMatrix * vec4(Normal, 0.0)).xyz;   
    WorldPos0      = (TransformationMatrix * vec4(Position, 1.0)).xyz;

	const float shadow_distance		= 250.f ;
	const float transition_distance = 10.f;

	float distance_to_cam   = length(model_view_position.xyz) ;
	ShadowCoords			= ToShadowMapSpace * vec4(WorldPos0, 1.f); 
	distance_to_cam			= distance_to_cam - (shadow_distance - transition_distance);
	distance_to_cam			= distance_to_cam / shadow_distance;
	ShadowCoords.w			= clamp(1.f - distance_to_cam, 0.f, 1.f);
}