#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in vec3 Normal;                                               
layout (location = 3) in vec3 Tangent;


uniform mat4 TransformationMatrix ;
uniform mat4 ProjectionMatrix ;
uniform mat4 ViewMatrix ;
                         
uniform float IsUseNormalMap;
uniform float IsUseFakeLighting ;
               
out vec2 TexCoord0;                                                                 
out vec3 Normal0;                                                                   
out vec3 WorldPos0;                                                                 
out vec3 PassTangent;
out float UseNormalMap;
void main()
{       
    gl_Position    = ProjectionMatrix * ViewMatrix * TransformationMatrix * vec4(Position, 1.0);
    TexCoord0      = TexCoord;                  
    Normal0        = (TransformationMatrix * vec4(Normal, 0.0)).xyz;   
    WorldPos0      = (TransformationMatrix * vec4(Position, 1.0)).xyz;

	if( IsUseNormalMap > .5f) 
	{
		PassTangent = (TransformationMatrix * vec4(Tangent, 0.0)).xyz; 
		UseNormalMap = 1.f;
	}
	else
	{
		UseNormalMap = 0.f;
		PassTangent = vec3(.0f) ;
	}

	if (IsUseFakeLighting > .5f)
    {
        Normal0 = vec3(.0f ,1.f, .0f) ;
    }
}