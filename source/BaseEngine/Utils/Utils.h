#ifndef UTIL_H
#define UTIL_H
#include <time.h>
#include <GL/glew.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <sstream>
#include <iostream>

#define M_PI  3.14159265359

//#define toRadians(x) (x*M_PI/180.0)
//#define toDegrees(x) (x*57.2957795f)
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

template<class type>
type toRadians(type a) {
	return a*M_PI / 180.0f;
}
template<class type>
type toDegrees(type a) {
	return a*57.2957795f;
}
using namespace std;
static float getTime(void) {
#ifdef MAC
	return (float)clock() * 100000.0f / (float)CLOCKS_PER_SEC;
#else
	return (float)clock() * 1000.0f / (float)CLOCKS_PER_SEC;
#endif
}
static float getMaxFromVector(glm::vec3 vector) {

	if (vector.x > vector.y && vector.x > vector.z)
		return vector.x;
	else if (vector.y > vector.z)
		return vector.y;
	else
		return vector.z;
}
static glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
	glm::mat4  matrix = glm::mat4(1.0);
	matrix *= glm::translate(translation);
	matrix *= glm::rotate(rotation.x, 1.0f, 0.0f, 0.0f);
	matrix *= glm::rotate(rotation.y, 0.0f, 1.0f, 0.0f);
	matrix *= glm::rotate(rotation.z, 0.0f, 0.0f, 1.0f);
	matrix *= glm::scale(scale.x, scale.y, scale.z);
	return matrix;
}
static glm::mat4 createTransformationMatrix(glm::vec2 translation, glm::vec2 scale) {
	glm::mat4  matrix = glm::mat4(1.0);
	matrix *= glm::translate(glm::vec3(translation, 0.0f));
	matrix *= glm::scale(glm::vec3(scale, 1.0f));
	return matrix;
}
static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}
static glm::mat4 createOffset() {
	glm::mat4 offset;
	offset *= glm::translate(glm::vec3(0.5f, 0.5f, 0.5f));
	offset *= glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
	return offset;
}
static void bindFrameBuffer(int frameBuffer, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}
static int createFrameBuffer() {
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_NONE);
	return frameBuffer;
}
static int createDepthBufferAttachment(int width, int height) {
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
static int createVAO() {
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	//vaos.push_back(vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}
static GLuint bindIndicesBuffer(vector<unsigned int> indices)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);
	//vbos.push_back(vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	return vboID;
}
static GLuint storeDataInAttributesList(int attributeNumber, int coordinateSize, vector<float>  data) {
	GLuint vboID;
	glGenBuffers(1, &vboID);
	//vbos.push_back(vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vboID;
}
static void unbindVAO() {
	glBindVertexArray(0);
}
static void enableCulling() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}
static void disableCulling() {
	glDisable(GL_CULL_FACE);
}
static void prtintMat4(glm::mat4 mat) {

	printf("\nMat:\n %f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n\n",
		mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}
template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}
static glm::vec2 toScreenSpace(glm::vec2 position, glm::vec2 windowSize) {
	return glm::vec2((position.x / windowSize.x) * 2 - 1, (1 - (position.y / windowSize.y)) * 2 - 1);
}
static string to_string(glm::vec3 v) {
	return to_string(v.x) + " " + to_string(v.y) + " " + to_string(v.z);
}
static string to_string(glm::vec2 v) {
	return to_string(v.x) + " " + to_string(v.y);
}
static string to_string(glm::mat4 m) {
	string s;
	for (int y = 0; y < 4; y++)
		s+= to_string(m[y][0]) + " " + to_string(m[y][1]) + " " + to_string(m[y][2]) + " " + to_string(m[y][3]) + "\n";
	return s;
}
static void printVector(string text, glm::vec3 v) {
	std::cout << text << v.x << " " << v.y << " " << v.z << std::endl;
}
static void printVector(string text, glm::vec2 v) {
	std::cout << text << v.x << " " << v.y  << std::endl;
}
static void printMatrix(string text, glm::mat4 m) {
	for(int y=0; y < 4 ; y++)
			std::cout << text << m[y][0] << " " << m[y][1] << " " << m[y][2] << " " << m[y][3] << std::endl;
}
#endif
