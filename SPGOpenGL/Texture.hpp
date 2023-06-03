#pragma once

#include <GL/glew.h>
#include <string>
#include "stb_image.h"
#include <iostream>
#include <vector>

class Texture{

public:
	
	GLuint texture, shader_programme;
	std::string textureName;
	static int textureCount;
	int textureIndex;

	void load(std::string path, GLuint shader_programme) {
		this->shader_programme = shader_programme;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		int width, height, nrChannels;
		//stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			std::string extension = path.substr(path.find_last_of(".") + 1);

			if (extension == "jpg" || extension == "jpeg")
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
					GL_UNSIGNED_BYTE, data);
			}else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, data);
			}
			
			glGenerateMipmap(GL_TEXTURE_2D);

			std::cout << "Loaded texture successfully" << std::endl;
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

		textureName = path.substr(path.find_last_of("/") + 1);
		textureName = textureName.substr(0, textureName.find_last_of("."));
		GLuint fishTexID = glGetUniformLocation(shader_programme, textureName.c_str());
		glUniform1i(fishTexID, textureIndex);

		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, texture);

		textureIndex = textureCount;
		textureCount++;
	}

	void bindTexture() {
		GLuint fishTexID = glGetUniformLocation(shader_programme, textureName.c_str());
		glUniform1i(fishTexID, textureIndex);

		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, texture);

		GLuint textureIndexPos = glGetUniformLocation(shader_programme, "textureIndex");
		glUniform1i(textureIndexPos, textureIndex);
	}

};