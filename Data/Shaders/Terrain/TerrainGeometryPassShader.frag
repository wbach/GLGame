#version 330
                                                                        
in vec2 TexCoord0;                                                                  
in vec3 Normal0;                                                                    
in vec3 WorldPos0;                                                                  
in vec4 ShadowCoords ;

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 SpecularOut;    

uniform sampler2D BlendMap ;									
uniform sampler2D BackgroundTexture ;
uniform sampler2D rTexture ;
uniform sampler2D gTexture ;
uniform sampler2D bTexture ;
uniform sampler2D BackgroundTextureNormal;
uniform sampler2D rTextureNormal ;
uniform sampler2D gTextureNormal ;
uniform sampler2D bTextureNormal ;
uniform sampler2D ShadowMap ;           


float CalculateShadowFactorSimple(){
    float objectNearestLight = texture(ShadowMap, ShadowCoords.xy).r ;
    float lightFactor = 1.0 ;
    if (ShadowCoords.z > objectNearestLight){
        lightFactor = 1.0f - (ShadowCoords.w * 0.4f);
    }
    return lightFactor ;

}
float CalculateShadowFactor(){
    float lightFactor = 1.0 ;
    float shadowFactor = 0 ;
    float a = 0 ;
    for(float x=-0.0005;x<=0.0005;x+=0.0001)
		for(float y=-0.0005;y<=0.0005;y+=0.0001)
		{
			float objectNearestLight = texture(ShadowMap, ShadowCoords.xy +vec2(x, y)).r ;   			 
            if (ShadowCoords.z > objectNearestLight + 0.001)
			{
                shadowFactor += (ShadowCoords.w * 0.4f);
            }	
            a++ ;
		}
	lightFactor = 1.0f - ( shadowFactor / a) ;

    return lightFactor ;
}
vec4 CalculateTerrainColor()
{
	vec4 blend_map_colour = texture(BlendMap, TexCoord0) ;
		
	float back_texture_amount = 1 - (blend_map_colour.r + blend_map_colour.g + blend_map_colour.b) ;
	vec2 tiled_coords = TexCoord0 * 40.0f ;

	vec4 backgorund_texture_colour = texture(BackgroundTexture, tiled_coords) * back_texture_amount;

	vec4 r_texture_colour = texture(rTexture, tiled_coords) * blend_map_colour.r;
	vec4 g_texture_colour = texture(gTexture, tiled_coords) * blend_map_colour.g;
	vec4 b_texture_colour = texture(bTexture, tiled_coords) * blend_map_colour.b;

	return backgorund_texture_colour + r_texture_colour + g_texture_colour + b_texture_colour ;
}
											
void main()									
{											
	WorldPosOut     = WorldPos0;					
	DiffuseOut      = CalculateTerrainColor().xyz *  CalculateShadowFactor();	
	NormalOut       = normalize(Normal0);					
	SpecularOut     = vec3(0.f);				
}