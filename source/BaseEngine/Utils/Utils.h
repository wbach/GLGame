#pragma once
#include <time.h>
#include <GL/glew.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <sstream>
#include <iostream>

#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288   /* pi */
#endif

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

namespace Utils{
	template<class type>
	type ToRadians(type a)
	{
		return a * static_cast<float>(M_PI) / 180.0f;
	}
	template<class type>
	type ToDegrees(type a)
	{
		return a * 180.0f / static_cast<float>(M_PI);
	}
	template<class type>
	type KmToMs(type a)
	{
		return a * 0.277777778f;
	}
	static void PrintVector(const std::string& text, const glm::vec3& v)
	{
		std::cout << text << v.x << " " << v.y << " " << v.z << std::endl;
	}
	static void PrintVector(const std::string& text, const glm::vec2& v)
	{
		std::cout << text << v.x << " " << v.y << std::endl;
	}
	static void PrintMatrix(const std::string& text, const glm::mat4& m)
	{
		for (int y = 0; y < 4; y++)
			std::cout << text << m[y][0] << " " << m[y][1] << " " << m[y][2] << " " << m[y][3] << std::endl;
	}
	static void PrtintMat4(const glm::mat4& mat)
	{
		printf("\nMat:\n %f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n\n",
			mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	}
	static float GetMaxFromVector(const glm::vec3& vector)
	{
		if (vector.x > vector.y && vector.x > vector.z)
			return vector.x;
		else if (vector.y > vector.z)
			return vector.y;
		else
			return vector.z;
	}
	static glm::mat4 CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
	{
		glm::mat4  matrix = glm::mat4(1.0);
		matrix *= glm::translate(translation);
		matrix *= glm::rotate(rotation.x, 1.0f, 0.0f, 0.0f);
		matrix *= glm::rotate(rotation.y, 0.0f, 1.0f, 0.0f);
		matrix *= glm::rotate(rotation.z, 0.0f, 0.0f, 1.0f);
		matrix *= glm::scale(scale.x, scale.y, scale.z);
		return matrix;
	}
	static glm::mat4 CreateTransformationMatrix(const glm::vec2& translation, const glm::vec2& scale)
	{
		glm::mat4  matrix = glm::mat4(1.0);
		matrix *= glm::translate(glm::vec3(translation, 0.0f));
		matrix *= glm::scale(glm::vec3(scale, 1.0f));
		return matrix;
	}
	static void CalculateBoudnigBox(const std::vector<float>& positions, glm::vec3& bounding_min, glm::vec3& bounding_max, glm::vec3& bounding_size, glm::vec3& bounding_center)
	{
		if (positions.size() == 0)
			return;

		bounding_min.x = bounding_max.x = positions[0];
		bounding_min.y = bounding_max.y = positions[1];
		bounding_min.z = bounding_max.z = positions[2];
		for (unsigned int i = 0; i < positions.size(); i += 3)
		{
			if (positions[i] < bounding_min.x) bounding_min.x = positions[i];
			if (positions[i] > bounding_max.x) bounding_max.x = positions[i];
			if (positions[i + 1] < bounding_min.y) bounding_min.y = positions[i + 1];
			if (positions[i + 1] > bounding_max.y) bounding_max.y = positions[i + 1];
			if (positions[i + 2] < bounding_min.z) bounding_min.z = positions[i + 2];
			if (positions[i + 2] > bounding_max.z) bounding_max.z = positions[i + 2];
		}
		bounding_size = bounding_max - bounding_min;
		bounding_center = (bounding_max + bounding_min) / 2.f;
	}
	static glm::vec3 CalculateMinimumVector(const glm::vec3& a, const glm::vec3& b)
	{
		glm::vec3 result;
		result.x = a.x < b.x ? a.x : b.x;
		result.y = a.y < b.y ? a.y : b.y;
		result.z = a.z < b.z ? a.z : b.z;
		return result;
	}
	static float BarryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}
	static glm::mat4 CreateOffset()
	{
		glm::mat4 offset;
		offset *= glm::translate(glm::vec3(0.5f, 0.5f, 0.5f));
		offset *= glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
		return offset;
	}
	static void BindFrameBuffer(const int& frameBuffer, const int& width, const int& height)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, width, height);
	}
	static int CreateFrameBuffer()
	{
		GLuint frame_buffer;
		glGenFramebuffers(1, &frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		glDrawBuffer(GL_NONE);
		return frame_buffer;
	}
	static int CreateDepthBufferAttachment(const int& width, const int& height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
		return texture;
	}
	static int CreateVao()
	{
		GLuint vao_id;
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);
		return vao_id;
	}
	static GLuint BindIndicesBuffer(const std::vector<unsigned int>& indices)
	{
		GLuint vbo_id;
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		return vbo_id;
	}
	static GLuint StoreDataInAttributesList(const int& attributeNumber, const int& coordinateSize, const std::vector<float>& data)
	{
		GLuint vbo_id;
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return vbo_id;
	}
	static void UnbindVao()
	{
		glBindVertexArray(0);
	}
	static void EnableCulling()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	static void DisableCulling()
	{
		glDisable(GL_CULL_FACE);
	}
	static void CreateQuad(GLuint &vao, GLuint& vbo_indices, GLuint& vbo_vertex, GLuint& vbo_text_coord, int &indices_size)
	{
		//std::vector<float> vertex = { -0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0 };
		std::vector<float> vertex = {	-1,  1, 0,
										-1, -1, 0,
										 1, -1, 0,
										 1,  1, 0 };
		std::vector<float> text_coords = {
			0, 0,
			0, 1,
			1, 1,
			1, 0 };
		std::vector<unsigned int> indices = { 0, 1, 3, 3, 1, 2 };
		indices_size = indices.size();
		vao = CreateVao();
		vbo_indices = BindIndicesBuffer(indices);
		vbo_vertex = StoreDataInAttributesList(0, 3, vertex);
		vbo_text_coord = StoreDataInAttributesList(1, 2, text_coords);
		UnbindVao();
	}
	static void SimpleRenderVao(const GLuint &vao, int indices, int attributes = 1)
	{
		glBindVertexArray(vao);
		for (int x = 0; x < attributes; x++)
			glEnableVertexAttribArray(x);

		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);

		for (int x = attributes; x > 0; x--)
			glDisableVertexAttribArray(x);
		glBindVertexArray(0);
	}
	static void DeleteQuad(GLuint &vao, GLuint& vbo_indices, GLuint& vbo_vertex, GLuint& vbo_text_coord)
	{
		glDeleteBuffers(1, &vbo_indices);
		glDeleteBuffers(1, &vbo_vertex);
		glDeleteBuffers(1, &vbo_text_coord);
		glDeleteVertexArrays(1, &vao);
	}

	template <typename T> std::string ToStr(const T& t)
	{
		std::ostringstream os;
		os << t;
		return os.str();
	}
	static int NextP2(int a)
	{
		int rval = 1;
		while (rval<a) rval <<= 1;
		return rval;
	}

	static glm::vec2 toScreenSpace(glm::vec2& position, glm::vec2& window_size)
	{
		return glm::vec2((position.x / window_size.x) * 2 - 1, (1 - (position.y / window_size.y)) * 2 - 1);
	}
	static std::string ToString(const glm::vec3& v)
	{
		return std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z);
	}
	static std::string ToString(const glm::vec2& v)
	{
		return std::to_string(v.x) + " " + std::to_string(v.y);
	}
	static std::string ToString(const glm::mat4& m)
	{
		std::string s;
		for (int y = 0; y < 4; y++)
			s += std::to_string(m[y][0]) + " " + std::to_string(m[y][1]) + " " + std::to_string(m[y][2]) + " " + std::to_string(m[y][3]) + "\n";
		return s;
	}
	
}