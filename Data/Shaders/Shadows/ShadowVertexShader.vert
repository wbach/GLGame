#version 330 core

layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;   
layout (location = 4) in mat4 TransformationMatrixes;

out vec2 textureCoords ;

uniform mat4 TransformationMatrix ;
uniform mat4 ProjectionMatrix ;
uniform mat4 ViewMatrix ;
uniform float IsInstancedRender ;

void main(void)
{
	mat4 transform_matrix = IsInstancedRender > 0.5f ? TransformationMatrixes : TransformationMatrix;
	gl_Position =  ProjectionMatrix * transform_matrix * vec4(Position, 1.0);
    textureCoords = TexCoord;
}