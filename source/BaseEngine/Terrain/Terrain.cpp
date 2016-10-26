#include "Terrain.h"
int CTerrain::s_ID = 0;
CTerrain::CTerrain(CLoader &loader)
	: m_Loader(loader)
{
	m_Name = "No name terrain";
	m_Id = s_ID++;
	m_IsInit = false;
}
void CTerrain::Init(string name,
	float x, float z,
	string height_map,  string blend_map,
	string background_texture, string background_normal_texture,
	string rock_texture, string rock_normal_texture,
	string r_texture, string r_normal_texture,
	string g_texture, string g_normal_texture,
	string b_texture, string b_normal_texture)
{

	m_HeightMapPath = height_map;
	m_BlendMapPath = blend_map;

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

	m_Transform.position.x = x * TERRAIN_SIZE;
	m_Transform.position.y = 0;
	m_Transform.position.z = z * TERRAIN_SIZE;

	m_BlendMap = m_Loader.LoadFullTexture(blend_map, m_BlendMapData, m_BlendMapWidth, m_BlendMapHeight);

	m_BackgroundTexture[0] = m_Loader.LoadTexture(background_texture);
	m_RTexture[0] = m_Loader.LoadTexture(r_texture);
	m_GTexture[0] = m_Loader.LoadTexture(g_texture);
	m_BTexture[0] = m_Loader.LoadTexture(b_texture);
	m_BackgroundTexture[1] = m_Loader.LoadTexture(background_normal_texture);
	m_RTexture[1] = m_Loader.LoadTexture(r_normal_texture);
	m_GTexture[1] = m_Loader.LoadTexture(g_normal_texture);
	m_BTexture[1] = m_Loader.LoadTexture(b_normal_texture);

	m_RockTexture[0] = m_Loader.LoadTexture(rock_texture);
	m_RockTexture[1] = m_Loader.LoadTexture(rock_normal_texture);

	try
	{
		GenerateTerrainMap(m_Loader, height_map);
	}
	catch (const std::exception& e)
	{
		e.what();
		return;
	}

	m_Name = name;

	m_WorldCenterPosition = glm::vec3(TERRAIN_SIZE) / 2.f + m_Transform.position ;
	m_WorldCenterPosition.y = 0;

	m_IsInit = true;
	
	m_Indices.clear();
	m_Vertices.clear();
	m_Normals.clear();
	m_Tangens.clear();
	m_TextureCoords.clear();
}

void CTerrain::AddTerrainEntity(shared_ptr<CEntity> e)
{
	if (!m_IsInit)
		return;

	m_TerrainEntities.push_back(e);

	//glm::vec2 position = e->GetPositionXZ();

	//if (m_Transform.position.x < position.x && m_Transform.position.x + m_Size > position.x)
	//	if (m_Transform.position.z < position.y && m_Transform.position.z + m_Size > position.y)
	//	{
	//		position.x -= m_Transform.position.x;
	//		position.y -= m_Transform.position.z;

	//		int x = static_cast<int>(position.x);
	//		int z = static_cast<int>(position.y);

	//		int size = e->GetMaxNormalizedSize();
	//		for( int zz = -size; zz <size; zz++)
	//			for (int xx = -size; xx < size; xx++)
	//			{
	//				if (x + xx < 0) continue;
	//				if (x + xx > (int)m_Size) continue;
	//				if (z + zz < 0) continue;
	//				if (z + zz > (int)m_Size) continue;

	//				if (((xx) * (xx)+(zz) * (zz)) <= size * size)
	//				{
	//					//m_ObjectIndicesGrid[xx +x + (zz+z)*static_cast<int>(m_Size)].push_back(m_TerrainEntities.size() - 1);
	//					m_ObjectIndicesGrid[xx+x][zz+z].push_back(m_TerrainEntities.size() - 1);
	//				}
	//			}
	//	}

	//std::ofstream  file;
	//file.open("Test.txt");
	//for (int x = 0; x < m_Size; x++)
	//{		
	//	for (int y = 0; y < m_Size; y++)
	//	{
	//		//if (m_ObjectIndicesGrid[y + x*static_cast<int>(m_Size)].size() > 0)
	//			file << 'X';
	//		else
	//			file << 'o';
	//	}
	//	file << std::endl;
	//}
	//file.close();
}

void CTerrain::AddTerrainGrass(const std::vector<glm::vec3>& positions, const GLuint & texture)
{
	m_Grass.push_back(CGrass());
	m_Grass.back().Init(positions, texture);
}

vector<int> CTerrain::GetNerbyEntities(glm::vec2 position_xz, int range) const
{
	vector<int> indexes;

	if (!m_IsInit)
		return indexes;

	if (m_Transform.position.x < position_xz.x && m_Transform.position.x + TERRAIN_SIZE > position_xz.x)
		if (m_Transform.position.z < position_xz.y && m_Transform.position.z + TERRAIN_SIZE > position_xz.y)
		{
			int x = static_cast<int> (position_xz.x);
			int z = static_cast<int> (position_xz.y);

			for (int zz = -range; zz <range; zz++)
				for (int xx = -range; xx < range; xx++)
				{
					if (x + xx < 0) continue;
					if (x + xx > (int)TERRAIN_SIZE -1) continue;
					if (z + zz < 0) continue;
					if (z + zz > (int)TERRAIN_SIZE -1) continue;

					//cout << xx + x << " | " << z + zz << endl;
					//Utils::PrintVector("Camera pos: ", position_xz);

					//for (int& a : m_ObjectIndicesGrid[xx + x + (zz + z)*static_cast<int>(m_Size)])
					for(int&a : m_ObjectIndicesGrid[xx + x][zz + z])
					{
						bool was = false;
						for (int&b : indexes)
						{
							if (b == a)
							{
								was = true;
								break;
							}
						}
						if (!was)
						{
							indexes.push_back(a);
						}
					}
				}
		}
	return indexes;
}

void CTerrain::InitGrassFromFile(std::string filename, GLuint texture)
{
	vector<glm::vec3> grass_positions;
	std::ifstream file;
	file.open(filename);
	string line;
	while (std::getline(file, line))
	{
		glm::vec3 position = Get::Vector3d(line);
		grass_positions.push_back(position);
	}
	file.close();
	AddTerrainGrass(grass_positions, texture);
}

vector<glm::vec3> CTerrain::GenerateGrassPositions(const string & filename, const int & count) const
{
	vector<glm::vec3> grass_positions;
	std::ofstream file;
	file.open(filename);

	for (int i = 0; i < count; i++)
	{
		glm::vec3 position;
		position.x = m_Transform.position.x + (rand() % static_cast<int>(2 * TERRAIN_SIZE)) / 2.f;
		position.z = m_Transform.position.z + (rand() % static_cast<int>(2 * TERRAIN_SIZE)) / 2.f;
		position.y = GetHeightofTerrain(position.x, position.z) + 1.0f;

		bool is = false;
		for (glm::vec3& p : grass_positions)
		{
			float l = glm::length(position - p);
			if (abs(l) < 1.f)
				is = true;
		}
		if (!is)
		{
			file << position.x << "x" << position.y << "x" << position.z << endl;
			grass_positions.push_back(position);
		}
	}
	file.close();
	return grass_positions;
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
	
	std::ifstream file;
	file.open(heightMap);
	std::string line;
	glm::vec2 resolution;

	int k = 0;
	while (std::getline(file, line))
	{
		if (line.size() < 1)continue;
		if (line[0] == 'r')
		{
			line = line.substr(1);
			resolution = Get::Vector2d(line);


			m_HeightMapResolution = resolution.x;
			m_Heights = new float*[m_HeightMapResolution];
			for (int i = 0; i < m_HeightMapResolution; ++i)
				m_Heights[i] = new float[m_HeightMapResolution];
		}
		else
		{
			string buf; // Have a buffer string
			stringstream ss(line); // Insert the string into a stream

			vector<string> tokens; // Create vector to hold our words

			while (ss >> buf)
				tokens.push_back(buf);
			for (int x = 0; x < resolution.x; x++)
			{
				float height = std::stof(tokens[x]);
				m_Heights[x][k] = height;
				m_AvarageHeight += height;
			}
			k++;
		}
	}
	file.close();
	m_AvarageHeight /= (resolution.x*resolution.x);
	m_Transform.position.y = m_AvarageHeight;
	m_WorldCenterPosition.y = m_AvarageHeight;

	CreateTerrain();
}
void CTerrain::CreateTerrain()
{
	m_VertexCount = m_HeightMapResolution;

	CreateTerrainVertexes(0, 0, m_VertexCount, m_VertexCount);

	int pointer = 0;
	//Triaagnle strip
	for (unsigned short gz = 0; gz < m_VertexCount - 1; gz++)
	{
		if ((gz & 1) == 0) 
		{ // even rows
			for (unsigned short  gx = 0; gx < m_VertexCount; gx++)
			{
				m_Indices.push_back(gx + gz * m_VertexCount);
				m_Indices.push_back(gx + (gz + 1) * m_VertexCount);
			}
		}
		else
		{ // odd rows
			for (unsigned short  gx = m_VertexCount - 1; gx > 0; gx--)
			{
				m_Indices.push_back(gx + (gz + 1) * m_VertexCount);
				m_Indices.push_back(gx - 1 + +gz * m_VertexCount);
			}
		}
	}

	//for (unsigned short i = 0 ; i < m_Indices.size() ; i+=3)
	//{
	//	SFace tmp = CreateFace(m_Indices[i], m_Indices[i+1], m_Indices[i+2], m_Vertices, m_TextureCoords);
	//	glm::vec3 tang = CalculateTangents(tmp);
	//	m_Tangens.push_back(tang.x);
	//	m_Tangens.push_back(tang.y);
	//	m_Tangens.push_back(tang.z);
//	}
	/*for (int gz = 0; gz <m_VertexCount - 1; gz++)
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
	
	}*/
	
	SMaterial material;
	m_Model.AddMesh("Terrain", m_Vertices, m_TextureCoords, m_Normals, m_Tangens, m_Indices, material);
}
void CTerrain::CreateTerrainVertexes(int x_start, int y_start, int width, int height)
{
	int i = 0;
	for (i = y_start; i < height; i++)
	{
		for (int j = x_start; j < width; j++)
		{
			//float height = GetHeightMap(j, i, m_HeightMapFreeImage);
			//m_Heights[j][i] = height;
			float height = m_Heights[j][i];
			m_Vertices.push_back(static_cast<float>(j) / (static_cast<float>(m_VertexCount - 1)) * TERRAIN_SIZE);
			m_Vertices.push_back(height);
			m_Vertices.push_back(static_cast<float>(i) / (static_cast<float>(m_VertexCount) - 1) * TERRAIN_SIZE);

			glm::vec3 normal = CalculateNormalMap(j, i);

			m_Normals.push_back(normal.x);
			m_Normals.push_back(normal.y);
			m_Normals.push_back(normal.z);

			m_TextureCoords.push_back(static_cast<float>(j) / static_cast<float>(m_VertexCount - 1));
			m_TextureCoords.push_back(static_cast<float>(i) / static_cast<float>(m_VertexCount - 1));
		}
	}
}
glm::vec3 CTerrain::CalculateNormalMap(int x, int z)
{
	int lx = x - 1; if (lx < 0) lx = 0;
	int rx = x + 1; if (rx > m_HeightMapResolution - 1) rx = m_HeightMapResolution -1;
	int dz = z - 1; if (dz < 0) dz = 0;
	int uz = z + 1; if (uz > m_HeightMapResolution - 1) uz = m_HeightMapResolution -1;
	float heightL = GetHeightMap(lx, z);
	float heightR = GetHeightMap(rx, z);
	float heightD = GetHeightMap(x, dz);
	float heightU = GetHeightMap(x, uz);
	glm::vec3 normal(heightL -heightR, 2.0f , heightD - heightU  ) ;
	glm::normalize(normal);	
	return normal ;
}

float CTerrain::GetHeightMap(int x, int z)
{	
	if (x < 0 || x >= m_HeightMapResolution || z < 0 || z >= m_HeightMapResolution)
	{
		return 0;
	}
	//cout << m_Heights[x][z] << endl;
	return m_Heights[x][z];
}

const float CTerrain::GetHeightofTerrain(glm::vec2 posXZ) const
{
	return GetHeightofTerrain(posXZ.x, posXZ.y);
}

const float CTerrain::GetHeightofTerrain(float worldX, float worldZ) const
{
	if (!m_IsInit)
		return -1;

	float terrain_x = worldX - m_Transform.position.x ;
	float terrain_z = worldZ - m_Transform.position.z ;

	float grid_squere_size = TERRAIN_SIZE / ((float) m_VertexCount - 1) ;
	int grid_x = (int) floor(terrain_x / grid_squere_size);
	int grid_z = (int) floor(terrain_z / grid_squere_size);

	

	if(grid_x >= m_VertexCount -1 || grid_z >= m_VertexCount - 1 || grid_x < 0 || grid_z < 0 )
		return -1 ;	

	float x_coord = (fmod(terrain_x, grid_squere_size))/ grid_squere_size;
	float z_coord = (fmod(terrain_z, grid_squere_size)) / grid_squere_size;



	float answer  = -1;
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

const float& CTerrain::GetSize() const
{
	return TERRAIN_SIZE;
}

void CTerrain::RecursiveResetEnities(shared_ptr<CEntity>& entity)
{
	for (shared_ptr<CEntity>& subentity : entity->GetChildrenEntities())
	{
		RecursiveResetEnities(subentity);
		subentity.reset();
	}
}
void CTerrain::PaintBlendMap(glm::vec3 point, int m_BrushSize, glm::vec3 paint_color)
{

	float terrain_x = point.x - m_Transform.position.x;
	float terrain_z = point.z - m_Transform.position.z;

	float x = terrain_x / TERRAIN_SIZE;
	float z = terrain_z / TERRAIN_SIZE;

	int blend_x = static_cast<int>(x * (float)m_BlendMapWidth);
	int blend_y = static_cast<int>(z * (float)m_BlendMapHeight);

	GLubyte* pixels = new GLubyte[4 * 2 * m_BrushSize * 2 * m_BrushSize];
	int i = 0;
	for (int y = -m_BrushSize; y < m_BrushSize; y++)
	{
		for (int x = -m_BrushSize; x < m_BrushSize; x++)
		{
			int mx = 4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth);
			int my = 4 * (blend_x + x + (blend_y + y)*m_BlendMapWidth)+3;

			if (mx < 0) continue;
			if (my > 4 * m_BlendMapWidth * m_BlendMapHeight) continue;

		/*	if ((4 * (blend_x + x + (blend_y + y) * m_BlendMapWidth)) < 0 || (4 * (blend_x + x + (blend_y + y) * m_BlendMapWidth) + 3) >= 4 * m_BlendMapWidth * m_BlendMapHeight)
				continue;*/

			if (((x ) * (x ) + (y) * (y)) <= m_BrushSize * m_BrushSize)
			{
				glm::vec2 l = glm::vec2(blend_x + x, (blend_y + y) ) - glm::vec2(blend_x, blend_y);
				float distance = glm::length(l) / m_BrushSize;
				float r = 1.f - distance;

				GLubyte b_old = m_BlendMapData[mx + 2];
				GLubyte g_old = m_BlendMapData[mx + 1];
				GLubyte r_old = m_BlendMapData[mx + 0];

				glm::vec3 color;
				color.x = (static_cast<float>(r_old) * distance) + (paint_color.x * 255.f * r);
				color.y = (static_cast<float>(g_old) * distance) + (paint_color.y * 255.f * r);
				color.z = (static_cast<float>(b_old) * distance) + (paint_color.z * 255.f * r);

				//color.x = (paint_color.x * 2.f * r);
				//color.y = (paint_color.y * 2.f * r);
				//color.z = (paint_color.z * 255.f * r);

				if (color.x > 255) color.x = 255;
				if (color.y > 255) color.y = 255;
				if (color.z > 255) color.z = 255;

				if (color.x < 0) color.x = 0;
				if (color.y < 0) color.y = 0;
				if (color.z < 0) color.z = 0;

				pixels[i++] = m_BlendMapData[mx + 0] = static_cast<GLubyte>(color.x);
				pixels[i++] = m_BlendMapData[mx + 1] = static_cast<GLubyte>(color.y);
				pixels[i++] = m_BlendMapData[mx + 2] = static_cast<GLubyte>(color.z);
				pixels[i++] = m_BlendMapData[mx + 3];

			}
			else
			{
				pixels[i++] = m_BlendMapData[mx + 0];//r				
				pixels[i++] = m_BlendMapData[mx + 1]; //g
				pixels[i++] = m_BlendMapData[mx + 2];//b
				pixels[i++] = m_BlendMapData[mx + 3];
			}
		}
	}
	m_Loader.UpdateSubTexture(m_BlendMap, pixels, blend_x - m_BrushSize, blend_y - m_BrushSize, 2 * m_BrushSize, 2 * m_BrushSize);
	delete[] pixels;

	//m_Loader.ReloadTexture(m_BlendMapData, m_BlendMap, m_BlendMapWidth, m_BlendMapHeight);
	//m_Loader.SaveTextureToFile(m_BlendMapPath, m_BlendMapData, m_BlendMapWidth, m_BlendMapHeight);
}
void gaussBlur_1(float** scl, float** tcl, int w, int h, float r)
{
	float rs = ceil(r * 2.57);     // significant radius
	for (int i = 0; i<h; i++)
		for (int j = 0; j<w; j++)
		{
			float val = 0, wsum = 0;
			for (int iy = i - rs; iy<i + rs + 1; iy++)
				for (int ix = j - rs; ix<j + rs + 1; ix++) {
					int x = min(w - 1, max(0, ix));
					int y = min(h - 1, max(0, iy));
					int dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i);
					float wght = exp(-dsq / (2 * r*r)) / (M_PI * 2 * r*r);
					val += scl[y][x] * wght;  wsum += wght;
				}
			tcl[i][j] = round(val / wsum);
		}
}
void CTerrain::PaintHeightMapPoint(glm::vec2 point, int brush_size, float strength, bool apply_limits, float up_limit, float down_limit)
{
	if(point.x < m_Transform.position.x && point.x > m_Transform.position.x + TERRAIN_SIZE)
	{
		return;
	}
	if (point.y < m_Transform.position.z && point.y > m_Transform.position.z + TERRAIN_SIZE)
	{
		return;
	}

	float terrain_x = point.x - m_Transform.position.x;
	float terrain_z = point.y - m_Transform.position.z;

	float x = terrain_x / TERRAIN_SIZE;
	float z = terrain_z / TERRAIN_SIZE;

	int height = m_HeightMapResolution;
	int width = m_HeightMapResolution;

	int blend_x = static_cast<int>(x * (float)width);
	int blend_y = static_cast<int>(z * (float)height);

	int i = 0;

	for (int y = -brush_size; y < brush_size; y++)
	{
		for (int x = -brush_size; x < brush_size; x++)
		{
			int mx = blend_x + x;
			int my = blend_y + y;

			if (mx < 0) continue;
			if (mx > width -1) continue;
			if (my < 0) continue;
			if (my > width -1) continue;

			if (((x) * (x)+(y) * (y)) <= brush_size * brush_size)
			{
				glm::vec2 l = glm::vec2(mx, my) - glm::vec2(blend_x, blend_y);

				float distance = glm::length(l) / brush_size;

				float r = 1.f - distance;

				float& blend_height = m_Heights[blend_x + x][blend_y + y];
				//float old_h = m_Heights[mx][my];
				blend_height += (strength * r); //r	
				if (apply_limits)
				{
					if (blend_height > up_limit)
						blend_height = up_limit;
					if (blend_height < down_limit)
						blend_height = down_limit;
				}
				
			}
		}
	}
	ReloadVertex();
}

void CTerrain::ReloadVertex()
{
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
}

bool CompareColour(RGBQUAD colour,BYTE r, BYTE g, BYTE b)
{
	if(colour.rgbRed == r && colour.rgbGreen == g && colour.rgbBlue == b)
		return true ;
	else
		return false;
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
	if (!m_IsInit)
		return;

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

	for (CGrass& grass : m_Grass)
	{
		grass.CleanUp();
	}

	if (m_BlendMapData != nullptr)
		delete[] m_BlendMapData;
}