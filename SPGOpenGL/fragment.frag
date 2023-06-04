#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 pos;
in vec3 uv;

uniform vec3 viewPos;

uniform int textureIndex;

//uniform for array of vec3 for lights
uniform vec3 lights[10];
uniform int lightsCount;

uniform sampler2D fish_texture;
uniform sampler2D fish_texture2;
uniform sampler2D fish_texture3;
uniform sampler2D fish_texture4;
uniform sampler2D fish_texture5;

uniform sampler2D wood_texture;
uniform sampler2D GlassAndWater_texture;
uniform sampler2D wall_texture;
uniform sampler2D negru_texture;

vec3 lighting(vec3 objectColor, vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos,
				vec3 ambient, vec3 lightColor, vec3 specular, float specPower)
{
	vec3 L = normalize(lightPos - pos);
	vec3 V = normalize(viewPos - pos);
	vec3 N = normalize(normal);
	vec3 R = normalize(reflect(-L, N));
	
    float diffCoef = max(dot(L, N), 0.0);
    float specCoef = pow(max(dot(R, V), 0.0), specPower);

	vec3 ambientColor = ambient * lightColor;
	vec3 diffuseColor = diffCoef * lightColor * dot(L,N);
	vec3 specularColor = specCoef * specular * lightColor;
	vec3 col = ( ambientColor + diffuseColor + specularColor ) * objectColor; 

	return clamp(col, 0, 1);
}


void main() 
{
	vec3 objectColor = vec3(1.0, 1.0, 1.0);
	vec3 ambient = vec3(0.1);
	vec3 specular = vec3(0.8);
	float specPower = 32;

	vec3 finalColor = vec3(0,0,0);

	for(int i = 0; i < lightsCount; ++i)
	{
		vec3 lightPos = lights[i];
		vec3 lightColor = vec3(1.0, 1.0, 1.0);
		vec3 color = lighting(objectColor, pos, normal, lightPos, viewPos, ambient, lightColor, specular, specPower);

		finalColor += color;
	}

	if(textureIndex == 0)
		fragColor = texture(fish_texture, uv.xy) * vec4(finalColor, 1.0);
	else if(textureIndex == 1)
		fragColor = texture(fish_texture2, uv.xy) * vec4(finalColor, 1.0);
	else if(textureIndex == 2)
		fragColor = texture(fish_texture3, uv.xy) * vec4(finalColor, 1.0);
	else if(textureIndex == 3)
		fragColor = texture(fish_texture4, uv.xy) * vec4(finalColor, 1.0);
	else if(textureIndex == 4)
		fragColor = texture(fish_texture5, uv.xy) * vec4(finalColor, 1.0);
	else if(textureIndex == 5)
		fragColor = texture(wood_texture, uv.xy) * vec4(finalColor, 1.0);
	else if(textureIndex == 6)
		fragColor = texture(negru_texture, uv.xy) * vec4(finalColor, 1.0);
	else if(textureIndex == 7)
		fragColor = texture(GlassAndWater_texture, uv.xy) * vec4(finalColor, 1.0);
	else if(textureIndex == 8)
		fragColor = texture(wall_texture, uv.xy) * vec4(finalColor, 1.0);
}
