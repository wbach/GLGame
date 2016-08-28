#version 430 core
#define MAX_LIGHTS 10

in float visibility; 		//fog
in float nLights ;
in vec2 textCoords ;
in vec3 surfaceNormal ;
in vec3 toCameraVector ;
in vec3 toLightVector[MAX_LIGHTS] ;
in vec3 pass_tangent ;
in float useNormalMapping ;
in float isElement;
in vec4 shadowCoords ;

uniform sampler2D backgroundTexture ;
uniform sampler2D rTexture ;
uniform sampler2D gTexture ;
uniform sampler2D bTexture ;
uniform sampler2D blendMap ;
uniform sampler2D backgroundTextureNormal;
uniform sampler2D rTextureNormal ;
uniform sampler2D gTextureNormal ;
uniform sampler2D bTextureNormal ;
uniform sampler2D shadowMap ;

uniform vec3 lightColour[MAX_LIGHTS] ;
uniform vec3 attenuation[MAX_LIGHTS] ;
uniform vec3 skyColour ;

out vec4 outColour ;

const float shineDamper = 20.0f;
const float reflectivity = 0.6f;
float calculateShadowFactorSimple(){
    float objectNearestLight = texture(shadowMap, shadowCoords.xy).r ;
    float lightFactor = 1.0 ;
    if (shadowCoords.z > objectNearestLight){
        lightFactor = 1.0f - (shadowCoords.w * 0.4f);
    }
    return lightFactor ;

}


vec3 CalcBumpedNormal(vec3 surfaceNormal, vec3 pass_tangent, vec3 BumpMap,vec2 textCoords)                                                                     
{                                                                                           
    vec3 Normal = normalize(surfaceNormal);                                                       
    vec3 Tangent = normalize(pass_tangent);                                                     
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);                           
    vec3 Bitangent = cross(Tangent, Normal);  
    vec3 BumpMapNormal = BumpMap ;                               
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);                              
    vec3 NewNormal;                                                                         
    mat3 TBN = mat3(Tangent, Bitangent, Normal);                                            
    NewNormal = TBN * BumpMapNormal;                                                        
    NewNormal = normalize(NewNormal);                                                       
    return NewNormal;                                                                       
}  
float calculateShadowFactor(){
    float lightFactor = 1.0 ;
    float shadowFactor = 0 ;
    float a = 0 ;
    for(float x=-0.0005;x<=0.0005;x+=0.0001)
		for(float y=-0.0005;y<=0.0005;y+=0.0001)
		{
			float objectNearestLight = texture(shadowMap, shadowCoords.xy +vec2(x,y)).r ;    
                            if (shadowCoords.z > objectNearestLight){
                                shadowFactor += (shadowCoords.w * 0.4f);
                            }	
                            a++ ;
		}
	lightFactor = 1.0f - ( shadowFactor / a) ;

        return lightFactor ;
}

void main()
{
	float lightFactor = 1.0f ;
	if ( shadowCoords.x > -0.5f ){
		lightFactor = calculateShadowFactor();
	}
	
	vec3 normal = surfaceNormal ;
	vec4 totalColour = vec4(1,1,1,1);
	if (isElement < 0.5){
		vec4 blendMapColour = texture(blendMap,textCoords) ;
		
		float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b) ;
		vec2 tiledCoords = textCoords * 40.0f ;
		vec4 backgorundTextureColour = texture(backgroundTexture , tiledCoords) * backTextureAmount;
		vec4 rTextureColour = texture(rTexture , tiledCoords) * blendMapColour.r;
		vec4 gTextureColour = texture(gTexture , tiledCoords) * blendMapColour.g;
		vec4 bTextureColour = texture(bTexture , tiledCoords) * blendMapColour.b;

		totalColour = backgorundTextureColour + rTextureColour + gTextureColour +bTextureColour ;


		
		if ( useNormalMapping > 0.5f) {
			 vec4 NormalbackgorundTexture = texture(backgroundTextureNormal , tiledCoords) * backTextureAmount;
			vec4 NormalrTexture = texture(rTextureNormal , tiledCoords) * blendMapColour.r;
			vec4 NormalgTexture = texture(gTextureNormal , tiledCoords) * blendMapColour.g;
			vec4 NormalbTexture = texture(bTextureNormal , tiledCoords) * blendMapColour.b;
			vec4 totalNormalTexture = NormalbackgorundTexture + NormalrTexture + NormalgTexture + NormalbTexture ;
			vec3 BumpMap = totalNormalTexture.xyz ; //texture2D(normalMap, textCoords).xyz ; 
			normal = CalcBumpedNormal(normal,pass_tangent,BumpMap,tiledCoords) ;
		
		}
	}
	else
		totalColour =  texture(backgroundTexture , textCoords) ;
	
	vec3 unitNormal = normalize(normal) ;
	vec3 uintVectorToCamera = normalize(toCameraVector) ;
    vec3 lightDirection = -uintVectorToCamera ;
	
	vec3 totalDiffiuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);
	for (int i=0 ; i<nLights ; i++){
		float distance = length(toLightVector[i]) ;
        float attFactor = attenuation[i].x + (attenuation[i].y *distance) + (attenuation[i].z*distance*distance);
		if (distance > 100 && attFactor != 1 ) continue;
		vec3 unitLightVector = normalize(toLightVector[i]) ;
        float nDotL = dot(unitNormal,unitLightVector) ;
        float brightness = max(nDotL, 0.0) ;  
		totalDiffiuse = totalDiffiuse + (brightness * lightColour[i])/attFactor;
		/*
		vec3 finalSpecular = vec3(0,0,0) ;
        if (shineDamper> 0.0f){
			vec3 reflectedLightDirecection = reflect(lightDirection,unitNormal) ;
            float specularFactor = dot(reflectedLightDirecection, uintVectorToCamera) ;
            specularFactor = max(specularFactor,0.0) ;
            float dampedFactor = pow(specularFactor,shineDamper) ;
            finalSpecular = dampedFactor * lightColour[i] ;
        }
        totalSpecular = totalSpecular + (finalSpecular)/attFactor ; */
		
	}
	if(totalColour.a < 0.5)
    {   
        discard ;
    }
	
	
	totalDiffiuse = max(totalDiffiuse,0.1);
	vec4 outDiffuse = vec4(totalDiffiuse,1.0f);
	outDiffuse *= totalColour * lightFactor;

	outColour =  outDiffuse + vec4(totalSpecular,1.0);;
	outColour*= 2;
	/*
	float xx = 1.0 ;
	float a = pow(outColour.x,xx);
	float b = pow(outColour.y,xx);
	float c = pow(outColour.z,xx);
	
	outColour = vec4(a,b,c,outColour.a)*2 ;*/
	//outColour = mix(vec4(skyColour,1.0),outColour,visibility);
	
	//totalDiffiuse = max(totalDiffiuse,0.1);
	//outColour = vec4(totalDiffiuse,1.0f) * totalColour ; //  + vec4(totalSpecular,1.0);
	//outColour = mix(vec4(skyColour,1.0),outColour,visibility);
	//outColour = vec4(totalDiffiuse,1.0f) ;
}