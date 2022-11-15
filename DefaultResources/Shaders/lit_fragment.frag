#version 450 core

out vec4 FragColor;

in vec3 ourNormal;
in vec2 TexCoord;
in vec3 position;

uniform vec3 MatAmbient;

uniform sampler2D m_Texture;

void main()
{
	vec4 texColor = texture(m_Texture, TexCoord);
	if (texColor.a < 0.5) discard;
	FragColor = texColor * vec4(MatAmbient, 1.0);
}