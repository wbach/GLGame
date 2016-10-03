#version 330 core

layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;   

out vec2 textureCoords ;

uniform mat4 TransformationMatrix ;
uniform mat4 ProjectionMatrix ;
uniform mat4 ViewMatrix ;

void main(void)
{

	gl_Position =  ProjectionMatrix * TransformationMatrix * vec4(Position, 1.0);
    textureCoords = TexCoord;
}