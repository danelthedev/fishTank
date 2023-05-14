#include "glloggers.hpp"
#include "objloader.hpp"
#include "utils.hpp"

#include <iostream>
#include <stdio.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <stack>
#include "Camera3D.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "Fish.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PI glm::pi<float>()

GLuint shader_programme;
glm::mat4 projectionMatrix, viewMatrix, modelMatrix;
std::stack<glm::mat4> modelStack;

Camera3D camera(glm::vec3(0,5,30));

std::vector< glm::vec3 > verticesData;

GLuint vaoObj, vboObj;

Fish fish1;
Mesh mesh1;

Light light(glm::vec3(-4, 2, 5));
//Light light2(glm::vec3(4, 2, 5));

float axisRotAngle = PI / 16.0; // unghiul de rotatie in jurul propriei axe
float radius = 2;
float scaleFactor = 3;

GLuint texture;

GLuint vaoObj1, vboObj1;
GLuint vaoObj2, vboObj2;

void loadMeshInScene(Mesh _mesh, GLuint& vao, GLuint& vbo) {
	std::vector<glm::vec3> verticesData;
	for (int i = 0; i < _mesh.vertices.size(); i++)
	{
		verticesData.push_back(_mesh.vertices[i]);
		verticesData.push_back(_mesh.normals[i]);
		verticesData.push_back(_mesh.uvs[i]);
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(glm::vec3), &verticesData[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), NULL);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 6));

}

void init()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	
	mesh1 = Mesh("obj/fish.obj");

	fish1 = Fish(mesh1, glm::vec3(0,0,0));
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0);

	glewInit();

	/*
		Texture stuff
	*/

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("obj/fish_texture.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/*
		End of texture stuff
	*/

	loadMeshInScene(fish1.mesh, vaoObj1, vboObj1);

	std::string vstext = textFileRead("vertex.vert");
	std::string fstext = textFileRead("fragment.frag");
	const char* vertex_shader = vstext.c_str();
	const char* fragment_shader = fstext.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	// check for shader compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	printShaderInfoLog(fs);
	printShaderInfoLog(vs);
	printProgramInfoLog(shader_programme);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_programme);

	GLuint lightPosLoc = glGetUniformLocation(shader_programme, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.getPosition()));

	GLuint viewPosLoc = glGetUniformLocation(shader_programme, "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.getPosition()));

	modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate
	modelMatrix *= glm::rotate(axisRotAngle, glm::vec3(0, 1, 0));
	modelMatrix *= glm::scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	GLuint modelMatrixLoc = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	loadMeshInScene(fish1.mesh, vaoObj1, vboObj1);


	std::cout << fish1.positionOffset.x << " " << fish1.positionOffset.y << " " << fish1.positionOffset.z << std::endl;
	modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate
	modelMatrix *= glm::translate(fish1.positionOffset);
	modelMatrix *= glm::rotate(axisRotAngle, glm::vec3(0, 1, 0));
	modelMatrix *= glm::scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));

	glBindVertexArray(vaoObj1);
	glBindBuffer(GL_ARRAY_BUFFER, vboObj1);
	(fish1.mesh).render();

	fish1.animate();
	
	//TODO:
	//Schimba mutarea coordonatelor cu operatii pe matrici
	//Repara texturile proaste

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	projectionMatrix = glm::perspective(camera.getFov(), (float)w / h, 0.1f, 1000.0f);
	viewMatrix = glm::lookAt(camera.getPosition(), camera.getObservedPoint(), camera.getUp());
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		axisRotAngle += 0.1;
		if (axisRotAngle > 2 * PI) 
			axisRotAngle = 0;
		break;
	case 's':
		axisRotAngle -= 0.1;
		if (axisRotAngle < 0)
			axisRotAngle = 2 * PI;
		break;
	case '+':
		scaleFactor += 0.01;
		break;
	case '-':
		scaleFactor -= 0.01;
	};
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(700, 700);
	glutCreateWindow("SPG");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
