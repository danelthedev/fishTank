#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 pos;
in vec3 uv;

uniform vec3 viewPos;

uniform int textureIndex;

uniform sampler2DArray textureArray;

uniform vec3 lights[10];
uniform int lightsCount;

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
        vec3 lightColor = vec3(0.75, 0.75, 0.75);
        vec3 color = lighting(objectColor, pos, normal, lightPos, viewPos, ambient, lightColor, specular, specPower);

        finalColor += color;
    }

    fragColor = texture(textureArray, vec3(uv.xy, textureIndex)) * vec4(finalColor, 1.0);
}