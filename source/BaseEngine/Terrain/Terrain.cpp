#include "Terrain.h"


glm::vec3 calculateTangents(Face f) {
						   glm::vec3 delatPos1 = f.vertex[1] - f.vertex[0] ;
						   glm::vec3 delatPos2 = f.vertex[2] - f.vertex[0] ;

						   glm::vec2 deltaUv1 = f.uv[1] - f.uv[0];
						   glm::vec2 deltaUv2 =f.uv[2] - f.uv[0] ;

						   float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
						   delatPos1*= deltaUv2.y ;
						   delatPos2*= deltaUv1.y ;
						  // delatPos1.scale(deltaUv2.y);
						  // delatPos2.scale(deltaUv1.y);
						   glm::vec3 tangent = delatPos1 - delatPos2;
						   tangent *= r ;
						   return tangent;
}
Face createFace(int i1,int i2, int i3, vector<float> &vertices,std::vector<float> &textureCoords){ //,std::vector<float> normals
	Face f1 ;
	f1.vertex[0] = glm::vec3(vertices[3*i1],vertices[3*i1 +1 ],vertices[3*i1 + 2]);
	f1.vertex[1] = glm::vec3(vertices[3*i2],vertices[3*i2 +1 ],vertices[3*i2 + 2]);
	f1.vertex[2] = glm::vec3(vertices[3*i3],vertices[3*i3 +1 ],vertices[3*i3 + 2]);

	f1.uv[0] = glm::vec2(textureCoords[2*i1],textureCoords[2*i1 +1 ]);
	f1.uv[1] = glm::vec2(textureCoords[2*i2],textureCoords[2*i2 +1 ]);
	f1.uv[2] = glm::vec2(textureCoords[2*i3],textureCoords[2*i3 +1 ]);

	return f1 ;

}
void CTerrain::generateTerrainMap(CLoader &loader,string heightMap){
	std::vector<unsigned int> indices;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> tangens;
	std::vector<float> textureCoords;


	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(heightMap.c_str(), 0);

	if(formato  == FIF_UNKNOWN) { printf( "%s : wrong image format or file does not exist.",heightMap.c_str());return;}
	FIBITMAP* imagen2 = FreeImage_Load(formato, heightMap.c_str());
	if(!imagen2) {printf( "%s : wrong image format or file does not exist.",heightMap.c_str());return;}
	FIBITMAP* imagen = FreeImage_ConvertTo32Bits(imagen2);
	FreeImage_Unload(imagen2);
	imageHeight =  FreeImage_GetHeight(imagen);
	imageWidth = FreeImage_GetWidth(imagen);

	int scale = 4;
	FreeImage_FlipHorizontal(imagen);
	imagen = FreeImage_Rescale(imagen, imageWidth / scale, imageHeight / scale, FILTER_BILINEAR);

	imageWidth /= scale;
	imageHeight /= scale;

	VERTEX_COUNT = imageHeight;
	cout << "Creating terrain size: " << VERTEX_COUNT << endl;
	Uint32 start;
	start = SDL_GetTicks();


	heights = new float*[VERTEX_COUNT];
	for(int i = 0; i < VERTEX_COUNT; ++i)
		heights[i] = new float[VERTEX_COUNT];
		int i=0;
		cout << "Time 1: " << SDL_GetTicks() - start;
		start = SDL_GetTicks();
		
		
		for(i=0;i<VERTEX_COUNT;i++){
			for(int j=0;j<VERTEX_COUNT;j++){
				float height = getHeightMap(j, i, imagen);
				heights[j][i] = height;
				vertices.push_back(static_cast<float>(j)/(static_cast<float>(VERTEX_COUNT - 1)) * SIZE) ;
				vertices.push_back(height);
				vertices.push_back(static_cast<float>(i)/(static_cast<float>(VERTEX_COUNT) - 1) * SIZE);

				glm::vec3 normal = calculateNormalMap(i,j,imagen);

				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
				textureCoords.push_back(static_cast<float>(j)/ static_cast<float>(VERTEX_COUNT - 1));
				textureCoords.push_back(static_cast<float>(i)/static_cast<float>(VERTEX_COUNT - 1));
			}
		}

		cout << "Time 2: " << SDL_GetTicks() - start <<endl ;
		start = SDL_GetTicks();


		int pointer = 0;
		for(int gz=0;gz<VERTEX_COUNT-1;gz++){
			for(int gx=0;gx<VERTEX_COUNT-1;gx++){
				int topLeft = (gz*VERTEX_COUNT)+gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz+1)*VERTEX_COUNT)+gx;
				int bottomRight = bottomLeft + 1;

				Face tmp = createFace(topLeft,bottomLeft,topRight,vertices,textureCoords);
				glm::vec3 tang = calculateTangents(tmp);
				tangens.push_back(tang.x);
				tangens.push_back(tang.y);
				tangens.push_back(tang.z);

				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);

				//system("cls"); int t1 =  glutGet(GLUT_ELAPSED_TIME) ;
				tmp = createFace(topRight,bottomLeft,bottomRight,vertices,textureCoords);
				//printf("End crate Face, time : %i\n",  glutGet(GLUT_ELAPSED_TIME) - t1);
				tang = calculateTangents(tmp);

				tangens.push_back(tang.x);
				tangens.push_back(tang.y);
				tangens.push_back(tang.z);

				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}

		cout << "Time 3: " << SDL_GetTicks() - start << endl;
		start = SDL_GetTicks();

		FreeImage_Unload(imagen);
		glm::vec3 diffuse(1.0),specular(0.0);
		float shineDamper =0,reflectivity = 0 ;
        Material material;
        model.addMesh("Terrain",vertices,textureCoords,normals, tangens,indices,material);
		model.meshes[0].vertexCount = indices.size();
		cout << "Terrain created. Vertexses : "<< indices .size() <<" \n" ;

        //	model.addMesh(filepath, postions, textCoords, normals, tangents,indices, material);
		//this->model = new CMeshModel(false,&vertices,&textureCoords,&normals,&tangens,&diffuse,&specular,&shineDamper,&reflectivity,&indices,1,0) ;
		//CMeshModel(vector<float>*positions,vector<float>*textCoords,vector<float>*normals,vector<float>*tangents,/*vector<float>*diffuse,vector<float>*specular,*/vector<unsigned int>* indices,vector<textureData> *tData =NULL ) {
		return;

}

glm::vec3 CTerrain::calculateNormalMap(int x, int z, FIBITMAP* image)
{
	float heightL = getHeightMap(x-1, z, image);
	float heightR = getHeightMap(x+1, z, image);
	float heightD = getHeightMap(x, z-1, image);
	float heightU = getHeightMap(x, z+1, image);
	glm::vec3 normal(heightL -heightR, 8.0f,heightD-heightU  ) ;
	glm::normalize(normal);
	return normal ;
}

float CTerrain::getHeightMap(int x, int z, FIBITMAP* image){

	if (x < 0 || x>= imageHeight || z < 0 || z >= imageWidth) {
		return 0;
	}
	RGBQUAD color;
	FreeImage_GetPixelColor(image,x,z,&color) ;
	BYTE r = color.rgbRed ;
	BYTE g = color.rgbGreen ;
	BYTE b = color.rgbBlue ;
	//float gray = 0.21 * r + 0.72 *g + 0.07 *b ;
	float height = r*g*b;
	height += MAX_PIXEL_COLOUR/2.0f ;
	height /= MAX_PIXEL_COLOUR/2.0f ;
	height *= MAX_HEIGHT ;
	return height;
}

float CTerrain::getHeightofTerrain(float worldX,float worldZ)
{
	float terrainX = worldX - this->transform.position.x ;
	float terrainZ = worldZ - this->transform.position.z ;
	//float heightsLength = sizeof(heights)/sizeof(heights[0]) ;
	float gridSquereSize = SIZE / ((float) VERTEX_COUNT-1) ;
	int gridX=(int) floor(terrainX/gridSquereSize);
	int gridZ=(int) floor(terrainZ/gridSquereSize);
	if(gridX >= VERTEX_COUNT-1 || gridZ >= VERTEX_COUNT-1 || gridX < 0 || gridZ < 0 ){
		return 0 ;
	}
	float xCoord = (fmod(terrainX,gridSquereSize))/ gridSquereSize;
	float zCoord = (fmod(terrainZ,gridSquereSize)) /gridSquereSize;
	float answer ;
	if (xCoord <= (1-zCoord)) {
		answer = barryCentric(glm::vec3(0,heights[gridX][gridZ] , 0), glm::vec3(1,heights[gridX + 1][gridZ], 0),
			glm::vec3(0,heights[gridX][gridZ + 1], 1),glm::vec2(xCoord, zCoord));
	} else {
		answer = barryCentric(glm::vec3(1, heights[gridX + 1][gridZ], 0),glm::vec3(1,heights[gridX + 1][gridZ + 1], 1),
			glm::vec3(0,heights[gridX][gridZ + 1], 1),glm::vec2(xCoord, zCoord));
	}
	return answer;
}

void CTerrain::filtrElementOffTerrain()
{
	vector<Element>::iterator iter;
	for(iter = elementsMap.begin(); iter != elementsMap.end() ; iter++){
			iter->filtr();
	}

}

float CTerrain::getSIZE()
{
	return SIZE;
}



CTerrain::CTerrain(CLoader &loader, string heightMap, float x, float z, GLuint blendMap, GLuint backgroundTexture, GLuint backgroundNormalTexture,
	GLuint rTexture, GLuint rnormalTexture, GLuint gTexture, GLuint gnormalTexture, GLuint bTexture, GLuint bnormalTexture)
{
	this->transform.position.x = x * SIZE ;
	this->transform.position.z = z * SIZE ;
	this->blendMap = blendMap;
	this->backgroundTexture[0] = backgroundTexture;
	this->rTexture[0] = rTexture;
	this->gTexture[0] = gTexture;
	this->bTexture[0] = bTexture;
	this->backgroundTexture[1] = backgroundNormalTexture;
	this->rTexture[1] = rnormalTexture;
	this->gTexture[1] = gnormalTexture;
	this->bTexture[1] = bnormalTexture;
	generateTerrainMap(loader,heightMap);
}

CTerrain::CTerrain()
{

}

void CTerrain::cleanUp()
{
	for(int i = 0; i < VERTEX_COUNT; ++i) {
		delete [] heights[i];
	}
	delete [] heights;
	// usuwa loader
//	rTexture.cleanUp();
//	bTexture.cleanUp();
//	gTexture.cleanUp();
//	blendMap.cleanUp();
//	backgroundTexture.cleanUp();
	model.cleanUp() ;
}


bool compareColour(RGBQUAD colour,BYTE r, BYTE g, BYTE b){
	if(colour.rgbRed == r && colour.rgbGreen == g && colour.rgbBlue == b)
		return true ;
	else
		return false;
}
void CTerrain::loadFloora(string flooraMap)
{
	this->flooraMap = flooraMap;
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(flooraMap.c_str(), 0);

	if(formato  == FIF_UNKNOWN) { printf( "%s : wrong image format or file does not exist.",flooraMap.c_str());return;}
	FIBITMAP* imagen2 = FreeImage_Load(formato, flooraMap.c_str());
	if(!imagen2) {printf( "%s : wrong image format or file does not exist.",flooraMap.c_str());return;}
	FIBITMAP* imagen = FreeImage_ConvertTo32Bits(imagen2);
	FreeImage_Unload(imagen2);
	int floraHeight =  FreeImage_GetHeight(imagen);
	int floraWidth = FreeImage_GetWidth(imagen);
	floraSize = floraHeight ;
	//FreeImage_FlipVertical(imagen);
	//FreeImage_FlipHorizontal(imagen);
	for (int i = 0; i < floraWidth; i++)
	{
		for (int j = 0; j < floraHeight; j++)
		{
			RGBQUAD colour;
			FreeImage_GetPixelColor(imagen,i,j,&colour) ;
			//printf("%d",(int)colour.rgbRed);
			//system("pause");
			if(compareColour(colour,0,0,0)) continue;
			if(compareColour(colour,255,0,0)){
				addElementToList(glm::vec3(255,0,0),0,floraHeight,i,j,25);
			 }
			if(compareColour(colour,0,0,255)){
				addElementToList(glm::vec3(0,0,255),1,floraHeight,i,j,30);
			}
			if(compareColour(colour,0,255,0)){
				addElementToList(glm::vec3(0,255,0),2,floraHeight,i,j,10);
			}
		}
	}
	FreeImage_Unload(imagen);
	// first time create, flora map with high resolution can have multiple objects in small distance, so flitr that, and save new one
	//filtrElementOffTerrain() ;
	//saveCorrectedFloraMap() ;
	return ;
	//wypisywanie elementow
	//vector<Element>::iterator it;
	//for (it = elementsMap.begin() ; it != elementsMap.end() ; ++it){
	//	printf("VALUE: %i\n",it->value);
	//	vector<glm::vec3>::iterator iter;
	//	for (iter = it->positions.begin() ; iter !=  it->positions.end() ; ++iter){
	//		printf("POS: %.2f, %.2f, %.2f\n",iter->x, iter->y,iter->z );
	//

	//		//printf("POS: %.2f, %.2f\n",xx,zz);
	//	}
	//}

}

vector<Element> CTerrain::getElementsMap()
{
	return elementsMap;
}

void CTerrain::addElementToList(glm::vec3 colorOnMap,int value, float floraSize, int i, int j, float distanceFilter)
{
	float xx = i * ((float)SIZE/floraSize) ;
	float zz = j * ((float)SIZE/floraSize) ;
	float height = getHeightofTerrain(xx,zz);
	bool isElement = false ;
	Element *a = findElement(value,isElement);
	if (!isElement){
		//	printf("%d",(int)colour.rgbRed);
		Element nowy;
		nowy.flitrDistance  = distanceFilter ;
		nowy.value = value ;
		nowy.colorOnMap = colorOnMap ;
		nowy.positions.push_back(glm::vec3(xx,height,zz));
		elementsMap.push_back(nowy);
	}
	else
		a->positions.push_back(glm::vec3(xx,height,zz));
}

Element* CTerrain::findElement(int value ,bool &finded)
{
	Element* element = NULL;
	finded = false;
	vector<Element>::iterator it;
	for (it = elementsMap.begin() ; it != elementsMap.end() ; ++it){

		if ((*it).value == value){
			element = &*it;
			finded = true ;
			return element;
		}
	}
	return element;
}

void CTerrain::saveCorrectedFloraMap()
{
	FIBITMAP *bitmap = FreeImage_Allocate(floraSize,floraSize, 24);
	for ( Element element : elementsMap){
		for( glm::vec3 pos : element.positions){
			int i = floor( pos.x / ((float)SIZE/floraSize) );
			int j = floor( pos.z / ((float)SIZE/floraSize) );



				RGBQUAD color;
				color.rgbRed = element.colorOnMap.x ;
				color.rgbGreen = element.colorOnMap.y ;
				color.rgbBlue = element.colorOnMap.z ;
				FreeImage_SetPixelColor(bitmap, i, j, &color);



		}
	}
	FreeImage_Save(FIF_BMP, bitmap, flooraMap.c_str(), 0);
}
