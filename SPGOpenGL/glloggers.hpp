#ifndef GLLOGGERS_H
#define GLLOGGERS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>


void printShaderInfoLog(GLuint obj);
void printProgramInfoLog(GLuint obj);

#endif