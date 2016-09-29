#version 330
#define MAX_LIGHTS 10
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2
struct SLight
{
    int	  m_Type;
	vec3  m_Position;
	vec3  m_Colour;
	vec3  m_Attenuation;
	float m_CutOff;
};
                                                              
uniform sampler2D PositionMap;
uniform sampler2D ColorMap;
uniform sampler2D NormalMap;

uniform vec2	ScreenSize;

uniform int		NumberOfLights;
uniform SLight  lights[MAX_LIGHTS];										             

out vec4 FragColor;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / ScreenSize;
}
vec4 CalculateBaseLight(vec3 light_direction, vec3 world_pos, vec3 unit_normal, vec3 light_color)
{
	float diffuse_factor = dot(unit_normal, light_direction);

	vec3 diffuse_color   = vec3(0.f);
	vec3 specular_color  = vec3(0.f);

	if (diffuse_factor > 0.f) 
	{
		diffuse_color = light_color * diffuse_factor;
	}
	//ambient color
	diffuse_color = max(diffuse_color, .2f);

	return vec4(diffuse_color, 1.f) + vec4(specular_color, 1.f);

}
vec4 CalcDirectionalLight(SLight light, vec3 world_pos, vec3 unit_normal)
{
	vec3 to_light_vector = light.m_Position - world_pos;
	vec3 light_direction = normalize(to_light_vector);

	return CalculateBaseLight(light_direction, world_pos, unit_normal, light.m_Colour);
}
vec4 CalculatePointLight(SLight light, vec3 world_pos, vec3 unit_normal)
{
	
	vec3 to_light_vector    = light.m_Position - world_pos;
	float distance_to_light = length(to_light_vector);
	vec3 light_direction	= normalize(to_light_vector);

	vec4 color = CalculateBaseLight(light_direction, world_pos, unit_normal, light.m_Colour);


	float att_factor =	light.m_Attenuation.x + 
						light.m_Attenuation.y * distance_to_light + 
						light.m_Attenuation.z * distance_to_light * distance_to_light;

	return color / att_factor; 
}
// not end 
vec4 CalcSpotLight(SLight light, vec3 world_pos, vec3 unit_normal)                                                
{ 
	vec3 to_light_vector = light.m_Position - world_pos;
	vec3 light_direction = normalize(to_light_vector); 
                                                                                         
  //  vec3 light_to_pixel = normalize(WorldPos0 - l.Base.Position);                             
    float spot_factor = dot(light_direction, light_direction);                                      
                                                                                            
    if (spot_factor > light.m_CutOff) 
	{                                                            
        vec4 color = CalculatePointLight(light, world_pos, unit_normal);                                        
        return color * (1.f - (1.f - spot_factor) * 1.f/(1.f - light.m_CutOff));                   
    }                                                                                       
    else 
	{                                                                                  
        return vec4(0.f, 0.f, 0.f, 1.f);                                                               
    }                                                                                       
} 

vec4 CalculateColor(vec3 world_pos, vec3 unit_normal)
{
	vec4 total_color = vec4(0.f, 0.f, 0.f, 1.f);


	for (int i = 0; i < NumberOfLights; i++)
	{	
		switch(lights[i].m_Type)
		{
			case LIGHT_TYPE_DIRECTIONAL:
				total_color += CalcDirectionalLight(lights[i], world_pos, unit_normal);
				break;
			case LIGHT_TYPE_POINT:
				total_color += CalculatePointLight(lights[i], world_pos, unit_normal);
				break;
			case LIGHT_TYPE_SPOT:
				total_color += CalcSpotLight(lights[i], world_pos, unit_normal);
				break;
		}
	}

	return total_color;
}
										
void main()									
{											
	vec2 tex_coord	= CalcTexCoord();
	vec3 world_pos	= texture(PositionMap, tex_coord).xyz;
	vec4 color		= texture(ColorMap, tex_coord);
	vec3 normal		= texture(NormalMap, tex_coord).xyz;
	normal			= normalize(normal);		

	const float gamma = 2.2f;
	const float exposure = 0.0f; 
	vec3 final_color = (CalculateColor(world_pos, normal)* color).rgb;
	//vec3 mapped = vec3(1.0) - exp(-final_color * exposure);
	final_color = pow(final_color, vec3(1.f / gamma));
	FragColor = vec4(final_color, 1.f);

}
