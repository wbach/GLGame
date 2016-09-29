#version 330
#define MAX_LIGHTS				10
#define LIGHT_TYPE_DIRECTIONAL	0
#define LIGHT_TYPE_POINT		1
#define LIGHT_TYPE_SPOT			2
struct SLight
{
    int	  m_Type;
	vec3  m_Position;
	vec3  m_Colour;
	vec3  m_Attenuation;
	float m_CutOff;	
};
struct SMaterial
{
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	float m_ShineDamper;
};                                                  
uniform sampler2D PositionMap;
uniform sampler2D ColorMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform vec3	  CameraPosition;
uniform vec2	  ScreenSize;

uniform int		NumberOfLights;
uniform SLight  lights[MAX_LIGHTS];										             

out vec4 FragColor;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / ScreenSize;
}
vec4 CalculateBaseLight(SMaterial material, vec3 light_direction, vec3 world_pos, vec3 unit_normal, vec3 light_color)
{
	float diffuse_factor = dot(unit_normal, light_direction);

	vec3 ambient_color   = vec3(0.f);
	vec3 diffuse_color   = vec3(0.f);
	vec3 specular_color  = vec3(0.f);

	if (diffuse_factor > 0.f) 
	{
		diffuse_color = light_color * diffuse_factor;
	}
	//ambient color
	diffuse_color = diffuse_color * material.m_Diffuse;
	ambient_color =  material.m_Ambient;

	if (material.m_ShineDamper > .0f)
	{
		vec3	vertex_to_camera	= normalize(CameraPosition - world_pos);
		vec3	light_reflect		= normalize(reflect(light_direction, unit_normal));
		float	specular_factor		= dot(vertex_to_camera, light_reflect);
				specular_factor		= pow(specular_factor, material.m_ShineDamper);
		if (specular_factor > .0f) 
		{
            specular_color = light_color * material.m_Specular * specular_factor;
        }
    }

	return vec4(ambient_color, 1.f) + vec4(diffuse_color, 1.f) + vec4(specular_color, 1.f);

}

vec4 CalcDirectionalLight(SMaterial material, SLight light, vec3 world_pos, vec3 unit_normal)
{
	vec3 to_light_vector = light.m_Position - world_pos;
	vec3 light_direction = normalize(to_light_vector);

	return CalculateBaseLight(material, light_direction, world_pos, unit_normal, light.m_Colour);
}

vec4 CalculatePointLight(SMaterial material, SLight light, vec3 world_pos, vec3 unit_normal)
{
	
	vec3 to_light_vector    = light.m_Position - world_pos;
	float distance_to_light = length(to_light_vector);
	vec3 light_direction	= normalize(to_light_vector);

	vec4 color = CalculateBaseLight(material, light_direction, world_pos, unit_normal, light.m_Colour);


	float att_factor =	light.m_Attenuation.x + 
						light.m_Attenuation.y * distance_to_light + 
						light.m_Attenuation.z * distance_to_light * distance_to_light;

	return color / att_factor; 
}
// not end 
vec4 CalcSpotLight(SMaterial material, SLight light, vec3 world_pos, vec3 unit_normal)                                                
{ 
	vec3 to_light_vector = light.m_Position - world_pos;
	vec3 light_direction = normalize(to_light_vector); 
                                                                                         
  //  vec3 light_to_pixel = normalize(WorldPos0 - l.Base.Position);                             
    float spot_factor = dot(light_direction, light_direction);                                      
                                                                                            
    if (spot_factor > light.m_CutOff) 
	{                                                            
        vec4 color = CalculatePointLight(material, light, world_pos, unit_normal);                                        
        return color * (1.f - (1.f - spot_factor) * 1.f/(1.f - light.m_CutOff));                   
    }                                                                                       
    else 
	{                                                                                  
        return vec4(0.f, 0.f, 0.f, 1.f);                                                               
    }                                                                                       
} 

vec4 CalculateColor(SMaterial material, vec3 world_pos, vec3 unit_normal)
{
	vec4 total_color = vec4(0.f, 0.f, 0.f, 1.f);

	for (int i = 0; i < NumberOfLights; i++)
	{	
		switch(lights[i].m_Type)
		{
			case LIGHT_TYPE_DIRECTIONAL:
				total_color += CalcDirectionalLight(material, lights[i], world_pos, unit_normal);
				break;
			case LIGHT_TYPE_POINT:
				total_color += CalculatePointLight(material, lights[i], world_pos, unit_normal);
				break;
			case LIGHT_TYPE_SPOT:
				total_color += CalcSpotLight(material, lights[i], world_pos, unit_normal);
				break;
		}
	}

	return total_color;
}
										
void main()									
{											
	vec2 tex_coord	= CalcTexCoord();
	vec3 world_pos	= texture(PositionMap, tex_coord).xyz;
	vec3 color		= texture(ColorMap, tex_coord).xyz;
	vec3 normal		= texture(NormalMap, tex_coord).xyz;
	vec3 specular	= texture(SpecularMap, tex_coord).xyz;
	normal			= normalize(normal);		

	SMaterial material;
	material.m_Ambient = color * 0.1f;
	material.m_Diffuse = color;
	material.m_Specular = specular;
	material.m_ShineDamper = 20.f;

	const float gamma = 2.2f;
	const float exposure = 0.0f; 
	vec3 final_color = CalculateColor(material, world_pos, normal).rgb;
	//vec3 mapped = vec3(1.0) - exp(-final_color * exposure);
	final_color = pow(final_color, vec3(1.f / gamma));
	FragColor = vec4(final_color, 1.f);

}
