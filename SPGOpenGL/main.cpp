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
#include "TextureLoader.hpp"
#include "Prop.hpp"

#include <windows.h>
#include <mmsystem.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PI glm::pi<float>()

GLuint shader_programme;
glm::mat4 projectionMatrix, viewMatrix, modelMatrix;
std::stack<glm::mat4> modelStack;

Camera3D camera(glm::vec3(0,5,30));

std::vector<Fish> fishes;
std::vector<Prop> props;
GLuint texture;

std::vector<Light> lights;

const int kTargetFPS = 60;
const int kFrameTime = 1000 / kTargetFPS;

int Texture::textureCount = 0;

void loadObjectsInScene() {

	//lights
	lights.push_back(Light(glm::vec3(-10, 10, 10)));
	lights.push_back(Light(glm::vec3(10, 10, 10)));

	//Fishes

	fishes.push_back(Fish(Mesh("obj/fish.obj"), 
		"obj/fish_texture.png", 0,
		glm::vec3(-5, 5, 0), 
		shader_programme));

	fishes.push_back(Fish(Mesh("obj/fish2.obj"), 
		"obj/fish_texture2.jpg", 1, 
		glm::vec3(5, 5, 0), 
		shader_programme));

	fishes.push_back(Fish(Mesh("obj/fish3.obj"), 
		"obj/fish_texture3.jpg", 2,
		glm::vec3(-5, -5, 0), 
		shader_programme));

	fishes.push_back(Fish(Mesh("obj/fish4.obj"), 
		"obj/fish_texture4.jpg", 3,
		glm::vec3(5, -5, 0),
		shader_programme));

	fishes.push_back(Fish(Mesh("obj/fish5.obj"), 
		"obj/fish_texture5.jpg", 4,
		glm::vec3(0, 0, 0), 
		shader_programme));

	//Props

	props.push_back(Prop(Mesh("obj/table.obj"),
		"obj/wood_texture.jpg", 5,
		glm::vec3(0,-10,0),
		shader_programme));

	props.push_back(Prop(Mesh("obj/fundAcvariu.obj"),
		"obj/negru_texture.png", 6,
		glm::vec3(0, -10, 0),
		shader_programme));

	props.push_back(Prop(Mesh("obj/acvariu.obj"),
		"obj/GlassAndWater_texture.png", 7,
		glm::vec3(0, -10, 0),
		shader_programme));

	props.push_back(Prop(Mesh("obj/wall.obj"),
		"obj/wall_texture.png", 8,
		glm::vec3(0, 0, 0),
		shader_programme));

}

void initWindow(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Fish");
}

void initOpenGL()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glewInit();

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

	///////////////////////////
	//light and position data//
	///////////////////////////

	//iterate through all lights and send data to shader
	std::vector<glm::vec3> lightPositions;
	for(int i = 0; i < lights.size(); ++ i)
		lightPositions.push_back(lights[i].getPosition());

	//send light data to shader
	GLuint lightPosLoc = glGetUniformLocation(shader_programme, "lights");
	glUniform3fv(lightPosLoc, lights.size(), glm::value_ptr(lightPositions[0]));

	//send light count to shader
	GLuint lightCountLoc = glGetUniformLocation(shader_programme, "lightsCount");
	glUniform1i(lightCountLoc, lights.size());
	
	//send view position to shader
	GLuint viewPosLoc = glGetUniformLocation(shader_programme, "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.getPosition()));

	///////////////////////////////////////////////
	//send scene information to shader and render//
	///////////////////////////////////////////////

	glm::mat4 projectionViewMatrix = projectionMatrix * viewMatrix;

	//Send world projection matrix to shader
	modelMatrix = glm::mat4();
	GLuint modelMatrixLoc = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix * modelMatrix));

	//Send normal matrix to shader
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	/////////////////////////////
	//Send actor data to shader//
	/////////////////////////////

	for(int i = 0; i < fishes.size(); ++i){
		fishes[i].texture.bindTexture(shader_programme, i);
		fishes[i].animate();
		fishes[i].render(projectionViewMatrix, shader_programme);
	}

	for (int i = 0; i < props.size(); ++i) {
		props[i].texture.bindTexture(shader_programme, i + 5);
		props[i].render(projectionViewMatrix, shader_programme);
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	projectionMatrix = glm::perspective(camera.getFov(), (float)w / h, 0.1f, 1000.0f);
	viewMatrix = glm::lookAt(camera.getPosition(), camera.getObservedPoint(), camera.getUp());
}

void keyboard(unsigned char key, int x, int y)
{

}

void idle(int value) {
	static int previous_time = glutGet(GLUT_ELAPSED_TIME);
	int current_time = glutGet(GLUT_ELAPSED_TIME);
	int elapsed_time = current_time - previous_time;

	if (elapsed_time >= kFrameTime) {
		// app logic

		glutPostRedisplay();
		previous_time = current_time;
	}

	int remaining_time = kFrameTime - elapsed_time;
	if (remaining_time > 0) {
		glutTimerFunc(remaining_time, idle, 0);
	}
	else {
		glutTimerFunc(0, idle, 0);
	}
}

void configCallbacks() {
	glutTimerFunc(0, idle, 0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
}

int main(int argc, char** argv)
{
	initWindow(argc, argv);
	initOpenGL();

	loadObjectsInScene();

	configCallbacks();
	glutMainLoop();
	return 0;
}
