#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 mvp;
uniform vec2 DeltaPos;
uniform vec2 DeltaUV;

out vec2 TexCoord;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0) + vec4(DeltaPos,1,0);
	TexCoord = aTexCoord / 16.0f * 0.98 + DeltaUV + 0.001f;
	TexCoord.y = 1.0 - TexCoord.y;
}