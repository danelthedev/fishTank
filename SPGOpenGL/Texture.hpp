#pragma once

#include <GL/glew.h>
#include <string>
#include "stb_image.h"
#include <iostream>
#include <vector>

class Texture {
public:
    GLuint texture;

    static int textureCount;

    void load(std::string path, GLuint shader_programme, int textureIndex) {
        glUseProgram(shader_programme);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            std::string extension = path.substr(path.find_last_of(".") + 1);

            GLenum format;
            if (extension == "jpg" || extension == "jpeg") {
                format = GL_RGB;
            }
            else {
                format = GL_RGBA;
            }

            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, width, height, 1, 0, format,
                GL_UNSIGNED_BYTE, data);

            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

            std::cout << "Loaded texture successfully" << std::endl;
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLuint texArrayLoc = glGetUniformLocation(shader_programme, "textureArray");
        glUniform1i(texArrayLoc, textureIndex);

        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    }

    void bindTexture(GLuint shader_programme, int textureIndex) {
        glUseProgram(shader_programme);

        GLuint texArrayLoc = glGetUniformLocation(shader_programme, "textureArray");
        glUniform1i(texArrayLoc, textureIndex);

        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    }
};