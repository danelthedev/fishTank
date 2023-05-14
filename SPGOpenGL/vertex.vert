#version 400

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vUv;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

out vec3 normal;
out vec3 pos;
out vec3 uv;

void main() 
{
	gl_Position = modelViewProjectionMatrix * vec4(vPos, 1.0);
	
	normal = vec3(normalMatrix * vec4(vNormal, 1));
	uv = vUv;
	pos = vPos;
}