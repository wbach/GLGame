#include "Terrain.h"
int CTerrain::s_ID = 0;

CTerrain::CTerrain(string name, CLoader &loader,
	float x, float z,
	string height_map,  string blend_map,
	string background_texture, string background_normal_texture,
	string rock_texture, string rock_normal_texture,
	string r_texture, string r_normal_texture,
	string g_texture, string g_normal_texture,
	string b_texture, string b_normal_texture)
	: m_HeightMapPath(height_map)
	, m_BlendMapPath(blend_map)
	, m_Loader(loader)
{
	m_RockTexturePath[0] = rock_texture;
	m_RockTexturePath[1] = rock_normal_texture;

	m_BackgorungTexturePath[0] = background_texture;
	m_BackgorungTexturePath[1] = background_normal_texture;

	m_RTexturePath[0] = r_texture;
	m_RTexturePath[1] = r_normal_texture;

	m_GTexturePath[0] = g_texture;
	m_GTexturePath[1] = g_normal_texture;

	m_BTexturePath[0] = b_texture;
	m_BTexturePath[1] = b_normal_texture;

	m_Position = glm::vec3(x, 0, z);

	m_Transform.position.x = x * m_Size;
	m_Transform.position.y = .0f;
	m_Transform.position.z = z * m_Size;

	m_BlendMap = loader.LoadFullTexture(blend_map, m_BlendMapData, m_BlendMapWidth, m_BlendMapHeight);

	m_BackgroundTexture[0] = loader.LoadTexture(background_texture);
	m_RTexture[0] = loader.LoadTexture(r_texture);
	m_GTexture[0] = loader.LoadTexture(g_texture);
	m_BTexture[0] = loader.LoadTexture(b_texture);
	m_BackgroundTexture[1] = loader.LoadTexture(background_normal_texture);
	m_RTexture[1] = loader.LoadTexture(r_normal_texture);
	m_GTexture[1] = loader.LoadTexture(g_normal_texture);
	m_BTexture[1] = loader.LoadTexture(b_normal_texture);

	m_RockTexture[0] = loader.LoadTexture(rock_texture);
	m_RockTexture[1] = loader.LoadTexture(rock_normal_texture);

	try
	{
		GenerateTerrainMap(loader, height_map);
	}
	catch (int e)
	{
		
	}

	m_Name = name;
	m_Id = s_ID++;
}

CTerrain::CTerrain(CLoader &loader)
	: m_Loader(loader)
{
	m_Name = "No name terrain";
	m_Id = s_ID++;
}

glm::vec3 CalculateTangents(const SFace& f) 
{
	glm::vec3 delat_pos_1 = f.vertex[1] - f.vertex[0] ;
	glm::vec3 delat_pos_2 = f.vertex[2] - f.vertex[0] ;

	glm::vec2 delta_uv_1 = f.uv[1] - f.uv[0];
	glm::vec2 delta_uv_2 = f.uv[2] - f.uv[0] ;

	float r = 1.0f / (delta_uv_1.x * delta_uv_2.y - delta_uv_1.y * delta_uv_2.x);
	delat_pos_1 *= delta_uv_2.y ;
	delat_pos_1 *= delta_uv_1.y ;
	glm::vec3 tangent = delat_pos_1 - delat_pos_2;
	tangent *= r ;
	return tangent;
}
SFace CreateFace(const int& i1, const int& i2, const int& i3, vector<float>& vertices, std::vector<float>& texture_coords)
{ 
	SFace f1 ;
	f1.vertex[0] = glm::vec3(vertices[3*i1], vertices[3*i1 +1 ], vertices[3*i1 + 2]);
	f1.vertex[1] = glm::vec3(vertices[3*i2], vertices[3*i2 +1 ], vertices[3*i2 + 2]);
	f1.vertex[2] = glm::vec3(vertices[3*i3], vertices[3*i3 +1 ], vertices[3*i3 + 2]);

	f1.uv[0] = glm::vec2(texture_coords[2*i1], texture_coords[2*i1 +1 ]);
	f1.uv[1] = glm::vec2(texture_coords[2*i2], texture_coords[2*i2 +1 ]);
	f1.uv[2] = glm::vec2(texture_coords[2*i3], texture_coords[2*i3 +1 ]);
	return f1 ;
}
void CTerrain::GenerateTerrainMap(CLoader &loader,string heightMap)
{
	
	m_HeightMapFormat = FreeImage_GetFileType(heightMap.c_str(), 0);

	if (m_HeightMapFormat == FIF_UNKNOWN) { printf("%s : wrong image format or file does not exist.", heightMap.c_str()); throw - 1; return; }
	FIBITMAP* imagen2 = FreeImage_Load(m_HeightMapFormat, heightMap.c_str());
	if(!imagen2) {printf( "%s : wrong image format or file does not exist.",heightMap.c_str()); throw - 1; return;}
	m_HeightMapFreeImage = FreeImage_ConvertTo32Bits(imagen2);
	FreeImage_Unload(imagen2);
	m_ImageHeight =  FreeImage_GetHeight(m_HeightMapFreeImage);
	m_ImageWidth = FreeImage_GetWidth(m_HeightMapFreeImage);

	//int scale = 1;
	FreeImage_FlipHorizontal(m_HeightMapFreeImage);
	//m_HeightMapFreeImage = FreeImage_Rescale(m_HeightMapFreeImage, m_ImageWidth / scale, m_ImageHeight / scale, FILTER_BILINEAR);

	//m_ImageWidth /= scale;
	//m_ImageHeight /= scale;

	CreateTerrain();
}
void CTerrain::CreateTerrain()
{
	m_VertexCount = m_ImageHeight;

	m_Heights = new float*[m_VertexCount];
	for (int i = 0; i < m_VertexCount; ++i)
		m_Heights[i] = new float[m_VertexCount];

	CreateTerrainVertexes(0, 0, m_VertexCount, m_VertexCount);

	int pointer = 0;
	for (int gz = 0; gz <m_VertexCount - 1; gz++)
	{
		for (int gx = 0; gx <m_VertexCount - 1; gx++)
		{
			int topLeft = (gz*m_VertexCount) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*m_VertexCount) + gx;
			int bottomRight = bottomLeft + 1;

			SFace tmp = CreateFace(topLeft, bottomLeft, topRight, m_Vertices, m_TextureCoords);
			glm::vec3 tang = CalculateTangents(tmp);
			m_Tangens.push_back(tang.x);
			m_Tangens.push_back(tang.y);
			m_Tangens.push_back(tang.z);

			m_Indices.push_back(topLeft);
			m_Indices.push_back(bottomLeft);
			m_Indices.push_back(topRight);

			tmp = CreateFace(topRight, bottomLeft, bottomRight, m_Vertices, m_TextureCoords);
			tang = CalculateTangents(tmp);

			m_Tangens.push_back(tang.x);
			m_Tangens.push_back(tang.y);
			m_Tangens.push_back(tang.z);

			m_Indices.push_back(topRight);
			m_Indices.push_back(bottomLeft);
			m_Indices.push_back(bottomRight);
		}
	}

	SMaterial material;
	m_Model.AddMesh("Terrain", m_Vertices, m_TextureCoords, m_Normals, m_Tangens, m_Indices, material);
	cout << "Terrain created. Vertexses : " << m_Indices.size() << " \n";
}
void CTerrain::CreateTerrainVertexes(int x_start, int y_start, int width, int height)
{
	int i = 0;
	for (i = y_start; i < height; i++)
	{
		for (int j = x_start; j < width; j++)
		{
			float height = GetHeightMap(j, i, m_HeightMapFreeImage);
			m_Heights[j][i] = height;
			m_Vertices.push_back(static_cast<float>(j) / (static_cast<float>(m_VertexCount - 1)) * m_Size);
			m_Vertices.push_back(height);
			m_Vertices.push_back(static_cast<float>(i) / (static_cast<float>(m_VertexCount) - 1) * m_Size);

			glm::vec3 normal = CalculateNormalMap(j, i, m_HeightMapFreeImage);

			m_Normals.push_back(normal.x);
			m_Normals.push_back(normal.y);
			m_Normals.push_back(normal.z);

			m_TextureCoords.push_back(static_cast<float>(j) / static_cast<float>(m_VertexCount - 1));
			m_TextureCoords.push_back(static_cast<float>(i) / static_cast<float>(m_VertexCount - 1));
		}
	}
}
glm::vec3 CTerrain::CalculateNormalMap(int x, int z, FIBITMAP* image)
{
	int lx = x - 1; if (lx < 0) lx = 0;
	int rx = x + 1; if (rx > m_ImageHeight) rx = m_ImageHeight;
	int dz = z - 1; if (dz < 0) dz = 0;
	int uz = z + 1; if (uz > m_ImageHeight) uz = m_ImageWidth;
	float heightL = GetHeightMap(lx, z, image);
	float heightR = GetHeightMap(rx, z, image);
	float heightD = GetHeightMap(x, dz, image);
	float heightU = GetHeightMap(x, uz, image);
	glm::vec3 normal(heightL -heightR, 2.0f , heightD - heightU  ) ;
	glm::normalize(normal);
	return normal ;
}

float CTerrain::GetHeightMap(int x, int z, FIBITMAP* image){

	if (x < 0 || x >= m_ImageHeight || z < 0 || z >= m_ImageWidth) {
		return 0;
	}
	RGBQUAD color;
	FreeImage_GetPixelColor(image,x,z,&color) ;
	float r = static_cast<float>(color.rgbRed) ;
	float g = static_cast<float>(color.rgbGreen) ;
	float b = static_cast<float>(color.rgbBlue) ;
	//float height = 0.21 * r + 0.72 *g + 0.07 *b ;
	float height = r*g*b;
	height += MAX_PIXEL_COLOUR/2.0f ;
	height /= MAX_PIXEL_COLOUR/2.0f ;
	height *= m_MaxHeight ;
	return height;
}

const float CTerrain::GetHeightofTerrain(float worldX, float worldZ) const
{
	float terrain_x = worldX - m_Transform.position.x ;
	float terrain_z = worldZ - m_Transform.position.z ;

	float grid_squere_size = m_Size / ((float) m_VertexCount - 1) ;
	int grid_x = (int) floor(terrain_x / grid_squere_size);
	int grid_z = (int) floor(terrain_z / grid_squere_size);

	

	if(grid_x >= m_VertexCount -1 || grid_z >= m_VertexCount - 1 || grid_x < 0 || grid_z < 0 )
		return 0 ;	

	float x_coord = (fmod(terrain_x, grid_squere_size))/ grid_squere_size;
	float z_coord = (fmod(terrain_z, grid_squere_size)) / grid_squere_size;



	float answer  = 0;
	if (x_coord <= (1 - z_coord)) 
	{
		answer = Utils::BarryCentric(glm::vec3(0, m_Heights[grid_x][grid_z], 0), glm::vec3(1, m_Heights[grid_x + 1][grid_z], 0),
			glm::vec3(0, m_Heights[grid_x][grid_z + 1], 1),glm::vec2(x_coord, z_coord));
	
	} else 
	{
		answer = Utils::BarryCentric(glm::vec3(1, m_Heights[grid_x + 1][grid_z], 0), glm::vec3(1, m_Heights[grid_x + 1][grid_z + 1], 1),
			glm::vec3(0, m_Heights[grid_x][grid_z + 1], 1),glm::vec2(x_coord, z_coord));
	}
	return answer;
}

void CTerrain::FiltrElementOffTerrain()
{
	vector<SElement>::iterator iter;
	for(iter = m_ElementsMap.begin(); iter != m_ElementsMap.end() ; iter++)
			iter->filtr();
}

const float& CTerrain::GetSize() const
{
	return m_Size;
}

void CTerrain::RecursiveResetEnities(shared_ptr<CEntity>& entity)
{
	for (shared_ptr<CEntity>& subentity : entity->GetChildrenEntities())
	{
		RecursiveResetEnities(subentity);
		subentity.reset();
	}
}
void CTerrain::PaintBlendMap(glm::vec3 point)
{
	float x = point.x / m_Size;
	float z = point.z / m_Size;

	int blend_x = static_cast<int>(x * (float)m_BlendMapWidth);
	int blend_y = static_cast<int>(z * (float)m_BlendMapHeight);

	if (blend_y < 0) blend_y = 0;
	if (blend_x < 0) blend_x = 0;

	if (blend_y > m_BlendMapHeight) blend_y = m_BlendMapHeight -1;
	if (blend_x > m_BlendMapWidth) blend_x = m_BlendMapWidth -1;

	GLubyte* pixels = new GLubyte[4 * 2 * m_BrushSize * 2 * m_BrushSize];
	int i = 0;
	for (int y = -m_BrushSize; y < m_BrushSize; y++)
	{
		for (int x = -m_BrushSize; x < m_BrushSize; x++)
		{
			if ((4 * (blend_x + x + (blend_y + y) * m_BlendMapWidth)) < 0 || (4 * (blend_x + x + (blend_y + y) * m_BlendMapWidth) + 3) >= 4 * m_BlendMapWidth * m_BlendMapHeight)
				continue;

			if (((x ) * (x ) + (y) * (y)) <= m_BrushSize * m_BrushSize)
			{
				glm::vec2 l = glm::vec2(blend_x + x, (blend_y + y) ) - glm::vec2(blend_x, blend_y);
				float distance = glm::length(l) / m_BrushSize;
				float r = 1.f - distance;

				GLubyte b_old = m_BlendMapData[4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth) + 2];
				GLubyte g_old = m_BlendMapData[4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth) + 1];
				GLubyte r_old = m_BlendMapData[4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth) + 0];


				pixels[i++] = m_BlendMapData[4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth) + 0] = static_cast<GLubyte>(static_cast<float>(r_old) * distance) + static_cast<GLubyte>(m_PaintColor.x * 255.f * r); //r				
				pixels[i++] = m_BlendMapData[4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth) + 1] = static_cast<GLubyte>(static_cast<float>(g_old) * distance) + static_cast<GLubyte>(m_PaintColor.y * 255.f * r); //g
				pixels[i++] = m_BlendMapData[4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth) + 2] = static_cast<GLubyte>(static_cast<float>(b_old) * distance) + static_cast<GLubyte>(m_PaintColor.z * 255.f * r); //b
				pixels[i++] = m_BlendMapData[4 * (blend_x + blend_y*m_BlendMapWidth) + 3];
			}
			else
			{
				pixels[i++] = m_BlendMapData[4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth) + 0];//r				
				pixels[i++] = m_BlendMapData[4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth) + 1]; //g
				pixels[i++] = m_BlendMapData[4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth) + 2];//b
				pixels[i++] = m_BlendMapData[4 * (blend_x + blend_y*m_BlendMapWidth) + 3];
			}
		}
	}
	m_Loader.UpdateSubTexture(m_BlendMap, pixels, blend_x - m_BrushSize, blend_y - m_BrushSize, 2 * m_BrushSize, 2 * m_BrushSize);
	delete[] pixels;

	//m_Loader.ReloadTexture(m_BlendMapData, m_BlendMap, m_BlendMapWidth, m_BlendMapHeight);
	//m_Loader.SaveTextureToFile(m_BlendMapPath, m_BlendMapData, m_BlendMapWidth, m_BlendMapHeight);
}

void CTerrain::PaintHeightMap(glm::vec3 point)
{
	float x = point.x / m_Size;
	float z = point.z / m_Size;

	int height = FreeImage_GetHeight(m_HeightMapFreeImage);
	int width = FreeImage_GetWidth(m_HeightMapFreeImage);

	int blend_x = static_cast<int>(x * (float)width);
	int blend_y = static_cast<int>(z * (float)height);

	if (blend_y < 0) blend_y = 0;
	if (blend_x < 0) blend_x = 0;

	if (blend_y > height) blend_y = height - 1;
	if (blend_x > width) blend_x = width - 1;
	m_BrushSize = 5;
	int i = 0;
	for (int y = -m_BrushSize; y < m_BrushSize; y++)
	{
		for (int x = -m_BrushSize; x < m_BrushSize; x++)
		{
			//if ((4 * (blend_x + x + (blend_y + y) * width)) < 0 || (4 * (blend_x + x + (blend_y + y) * width) + 3) >= 4 * width * height)
				//continue;

			if (((x) * (x)+(y) * (y)) <= m_BrushSize * m_BrushSize)
			{
				RGBQUAD color;
				FreeImage_GetPixelColor(m_HeightMapFreeImage, x + blend_x, y + blend_y, &color);
				color.rgbBlue += m_HeightPaint.z;
				if (color.rgbBlue > 250) color.rgbBlue = 250;
				color.rgbGreen += m_HeightPaint.y;
				if (color.rgbGreen > 250) color.rgbGreen = 250;
				color.rgbRed += m_HeightPaint.x;
				if (color.rgbRed > 250) color.rgbRed = 250;
				FreeImage_SetPixelColor(m_HeightMapFreeImage, x + blend_x, y + blend_y, &color);
			}			
		}
	}

	m_Vertices.clear();
	m_Normals.clear();
	m_TextureCoords.clear();
	CreateTerrainVertexes(0, 0, m_VertexCount, m_VertexCount);
	GLuint vbo = m_Model.GetMeshes()[0].GetVbo(VertexBufferObjects::POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), &m_Vertices[0], GL_DYNAMIC_DRAW);

	GLuint vbon = m_Model.GetMeshes()[0].GetVbo(VertexBufferObjects::NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, vbon);
	glBufferData(GL_ARRAY_BUFFER, m_Normals.size() * sizeof(float), &m_Normals[0], GL_DYNAMIC_DRAW);

	//FreeImage_Save(m_HeightMapFormat, m_HeightMapFreeImage, "h.bmp", 0);
	//cout << "Paint height" << endl;
}


bool CompareColour(RGBQUAD colour,BYTE r, BYTE g, BYTE b)
{
	if(colour.rgbRed == r && colour.rgbGreen == g && colour.rgbBlue == b)
		return true ;
	else
		return false;
}
void CTerrain::LoadFloora(string flooraMap)
{
	m_FlooraMap = flooraMap;
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(flooraMap.c_str(), 0);

	if(formato  == FIF_UNKNOWN) { printf( "%s : wrong image format or file does not exist.",flooraMap.c_str());return;}
	FIBITMAP* imagen2 = FreeImage_Load(formato, flooraMap.c_str());
	if(!imagen2) {printf( "%s : wrong image format or file does not exist.",flooraMap.c_str());return;}
	FIBITMAP* imagen = FreeImage_ConvertTo32Bits(imagen2);
	FreeImage_Unload(imagen2);
	int floraHeight =  FreeImage_GetHeight(imagen);
	int floraWidth = FreeImage_GetWidth(imagen);
	m_FloraSize = static_cast<float>(floraHeight);

	for (int i = 0; i < floraWidth; i++)
	{
		for (int j = 0; j < floraHeight; j++)
		{
			RGBQUAD colour;
			FreeImage_GetPixelColor(imagen,i,j,&colour) ;

			if(CompareColour(colour,0,0,0)) continue;
			if(CompareColour(colour,255,0,0)){
				AddElementToList(glm::vec3(255.0f, .0f, .0f), 0, static_cast<float>(floraHeight), i, j, 25);
			 }
			if(CompareColour(colour,0,0,255)){
				AddElementToList(glm::vec3(.0f, .0f, 255.0f), 1, static_cast<float>(floraHeight), i, j, 30);
			}
			if(CompareColour(colour,0,255,0)){
				AddElementToList(glm::vec3(0.0f, 255.0f, .0f), 2, static_cast<float>(floraHeight), i, j, 10);
			}
		}
	}
	FreeImage_Unload(imagen);
	return ;
}



vector<SElement> CTerrain::GetElementsMap()
{
	return m_ElementsMap;
}

void CTerrain::AddElementToList(glm::vec3 colorOnMap,int value, float floraSize, int i, int j, float distanceFilter)
{
	float xx = i * ((float)m_Size/floraSize) ;
	float zz = j * ((float)m_Size/floraSize) ;
	float height = GetHeightofTerrain(xx,zz);
	bool isElement = false ;
	SElement *a = FindElement(value,isElement);
	if (!isElement)
	{
		SElement nowy;
		nowy.flitrDistance  = distanceFilter ;
		nowy.value = value ;
		nowy.colorOnMap = colorOnMap ;
		nowy.positions.push_back(glm::vec3(xx,height,zz));
		m_ElementsMap.push_back(nowy);
	}
	else
		a->positions.push_back(glm::vec3(xx,height,zz));
}

SElement* CTerrain::FindElement(int value ,bool &finded)
{
	SElement* element = NULL;
	finded = false;
	vector<SElement>::iterator it;
	for (it = m_ElementsMap.begin() ; it != m_ElementsMap.end() ; ++it)
	{
		if ((*it).value == value)
		{
			element = &*it;
			finded = true ;
			return element;
		}
	}
	return element;
}

void CTerrain::SaveCorrectedFloraMap()
{
	FIBITMAP *bitmap = FreeImage_Allocate(static_cast<int>(m_FloraSize), static_cast<int>(m_FloraSize), 24);
	for ( SElement element : m_ElementsMap)
	{
		for( glm::vec3 pos : element.positions)
		{
			int i = static_cast<int>( floor( pos.x / ((float)m_Size / m_FloraSize) ) );
			int j = static_cast<int>( floor( pos.z / ((float)m_Size / m_FloraSize) ) );

			RGBQUAD color;
			color.rgbRed = static_cast<BYTE>(element.colorOnMap.x) ;
			color.rgbGreen = static_cast<BYTE>(element.colorOnMap.y);
			color.rgbBlue = static_cast<BYTE>(element.colorOnMap.z);
			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}
	FreeImage_Save(FIF_BMP, bitmap, m_FlooraMap.c_str(), 0);
}

void CTerrain::SetName(std::string name) 
{ 
	m_Name = name;
}

const string CTerrain::GetName() const
{ 
	return m_Name;
}

const string CTerrain::GetNameWithId() const
{
	string name = m_Name + "__id_t" + std::to_string(m_Id);
	return name;
}

const glm::vec3 CTerrain::GetPosition() const 
{ 
	return m_Position;
}
void CTerrain::CleanUp()
{
	for (int i = 0; i < m_VertexCount; ++i)
	{
		delete[] m_Heights[i];
	}
	delete[] m_Heights;
	// tekstury usuwa loader
	m_Model.CleanUp();

	for (shared_ptr<CEntity>& entity : m_TerrainEntities)
	{
		RecursiveResetEnities(entity);
		entity.reset();
	}

	if (m_BlendMapData != nullptr)
		delete[] m_BlendMapData;

	FreeImage_Unload(m_HeightMapFreeImage);
}