#version 330
struct SMaterial
{
	vec3  m_Ambient;
	vec3  m_Diffuse;
	vec3  m_Specular;
	float m_ShineDamper;
};                                                                     
in vec2 TexCoord0;                                                                  
in vec3 Normal0;                                                                    
in vec3 WorldPos0;                                                                  
in vec3 PassTangent;
in float UseNormalMap;

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 MaterialSpecular;  
								
uniform sampler2D gColorMap;
uniform sampler2D NormalMap;
uniform sampler2D ShadowMap;

uniform SMaterial ModelMaterial;

vec3 CalcBumpedNormal(vec3 surface_normal, vec3 pass_tangent, sampler2D normal_map,vec2 text_coords)
{                                                                                           
    vec3 normal = normalize(surface_normal);                                                       
    vec3 tangent = normalize(pass_tangent);                                                     
    tangent = normalize(tangent - dot(tangent, normal) * normal);                           
    vec3 bitangent = cross(tangent, normal);  
    vec3 bumpMapNormal = texture2D(normal_map, text_coords).xyz ;                                
    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0);                              
    vec3 new_normal;                                                                         
    mat3 tbn = mat3(tangent, bitangent, normal);                                            
    new_normal = tbn * bumpMapNormal;                                                        
    new_normal = normalize(new_normal);                                                       
    return new_normal;                                                                       
}  
									
void main()									
{											
	WorldPosOut      = WorldPos0;					
	DiffuseOut       = texture(gColorMap, TexCoord0).xyz * ModelMaterial.m_Diffuse;	
	NormalOut        = UseNormalMap > .5f ?  CalcBumpedNormal(Normal0, PassTangent, NormalMap, TexCoord0) : normalize(Normal0);					
	MaterialSpecular = ModelMaterial.m_Specular;
}
