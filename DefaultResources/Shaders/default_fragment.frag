#version 450 core

#define DD 13u
#define DP 6u
#define LS 8u
#define SHADOWMAP_RESOLUTION 2048
#define DELTA 0.001

out vec4 FragColor;

in vec3 ourNormal;
in vec2 TexCoord;
in vec3 position;
in vec3 tangent;

uniform float DLight[DD];
uniform float PLights[DP*LS];

uniform uint PLightsCount;

uniform sampler2DShadow DShadowMap;
in vec4 DPos;

uniform vec3 MatAmbient;
uniform vec3 MatDiffuse;
uniform vec3 MatSpecular;
uniform vec3 MatEmissive;
uniform float MatAlpha;
uniform float MatSmoothness;
uniform float MatShininess;
uniform float MatAbsorbtion;

uniform sampler2D m_Texture;
uniform sampler2D m_NTexture;
uniform vec3 cameraPos;

vec3 normal;

float CalcShadowFactor()
{
	vec4 LightSpacePos = DPos;
    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
	float xOffset = 1.0/SHADOWMAP_RESOLUTION;
    float yOffset = 1.0/SHADOWMAP_RESOLUTION;

    float Factor = 0.0;
    for (int y = -1 ; y <= 1 ; y++) {
        for (int x = -1 ; x <= 1 ; x++) {
            vec2 Offsets = vec2(x * xOffset, y * yOffset);
            vec3 UVC = vec3(UVCoords + Offsets, z - DELTA);
            Factor += texture(DShadowMap, UVC);
        }
    }
    return (Factor / 9.0);
	/*
	vec3 UVC = vec3(UVCoords, z + DELTA);
	return texture(DShadowMap, UVC);
	*/
}

vec3 GetPointLights(vec3 textureColor)
{
	vec3 amb;
	vec3 dif;
	vec3 spe;
	for (uint i = 0u; i < PLightsCount; i++)
	{
		vec3 lposition = vec3(PLights[i*DP], PLights[i*DP+1u], PLights[i*DP+2u]);
		vec3 ldiffuse = vec3(PLights[i*DP+3u], PLights[i*DP+4u], PLights[i*DP+5u]);
		vec3 AB = lposition-position;
		float att = length(AB);
		att = clamp(1.0/(1.0+0.2*att+0.04*att*att),0.0,1.0);
		if (att < 0.01) continue;
		AB = normalize(AB);
		float deltaA = dot(AB, normal);
		if (deltaA < 0) deltaA = 0;
		vec3 halfVV = normalize(AB + normalize(cameraPos-position));
		float deltaB = pow(max(dot(normal, halfVV), 0.0), 64.0 * MatSmoothness);
		amb = amb + vec3(ldiffuse * MatAmbient) * att;
		dif = dif + vec3(ldiffuse*MatDiffuse*deltaA) * att;
		spe = spe + vec3(MatSpecular*3*ldiffuse*deltaB) * att;
	}
	amb = clamp(amb, 0, 1);
	dif = clamp(dif, 0, 1);
	spe = clamp(spe, 0, 1);
	spe = spe * MatShininess;
	return textureColor * (amb+dif+spe) + (1-MatAbsorbtion) * spe;
}

vec3 GetDirectionalLight(vec3 textureColor)
{
	float deltaD = CalcShadowFactor();
	vec3 amb;
	vec3 dif;
	vec3 spe;
	vec3 lambient = vec3(DLight[0u], DLight[1u], DLight[2u]);
	vec3 ldiffuse = vec3(DLight[3u], DLight[4u], DLight[5u]);
	vec3 lspecular = vec3(DLight[6u], DLight[7u], DLight[8u]);
	float lsmoothness = DLight[9u];
	vec3 ldirection = normalize(vec3(DLight[10u], DLight[11u], DLight[12u]));
	float deltaA = dot(-ldirection, normal);
	float deltaB = 0;
	if (deltaA < 0) deltaA = 0;
	else
	{
		vec3 halfV = normalize(-ldirection + normalize(cameraPos-position));
		deltaB = pow(max(dot(normal, halfV), 0.0), lsmoothness * MatSmoothness);
	}
	amb = vec3(lambient * MatAmbient);
	dif = vec3(ldiffuse*MatDiffuse*deltaA*deltaD);
	spe = vec3(MatSpecular*lspecular*deltaB*deltaD);
	spe = spe * MatShininess;
	return textureColor * (amb+dif+spe) + (1-MatAbsorbtion) * spe;
}

vec3 CalcBumpedNormal()
{
    vec3 Normal = normalize(ourNormal);
    vec3 Tangent = normalize(tangent);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture(m_NTexture, TexCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

void main()
{
	normal = ourNormal;
	vec4 texColor = texture(m_Texture, TexCoord);
	if (texColor.a < 0.5) discard;
	FragColor = vec4(GetDirectionalLight(texColor.xyz) + GetPointLights(texColor.xyz) + texColor.xyz*MatEmissive, texColor.w*MatAlpha);
}