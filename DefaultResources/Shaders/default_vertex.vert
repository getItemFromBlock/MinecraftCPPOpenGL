#version 450 core

#define LS 8u

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out vec3 ourNormal;
out vec2 TexCoord;
out vec3 position;
out vec3 tangent;
out vec4 DPos;

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 Dmvp;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0);
	ourNormal = vec3(m * vec4(aNormal, 0.0));
	TexCoord = aTexCoord;
	position = vec3(m * vec4(aPos, 1.0));
	tangent = vec3(m * vec4(aTangent, 0.0));
	DPos = Dmvp * vec4(position, 1.0);
}