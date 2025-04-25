#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

// TextureManager handles loading, storing, and binding OpenGL textures by name
class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    // Loads a texture from a file and stores its OpenGL ID under a given name
    // Throws std::runtime_error if loading fails
    GLuint loadTexture(const std::string& name, const std::string& filename);

    // Retrieves the OpenGL texture ID by name
    // Throws std::out_of_range if the name is not found
    GLuint getTexture(const std::string& name) const;

    // Binds a texture by name to a specific texture unit (default is GL_TEXTURE0)
    void bindTexture(const std::string& name, GLenum textureUnit = GL_TEXTURE0) const;

    // Deletes all loaded textures and clears internal map
    void releaseAllTextures();

private:
    std::unordered_map<std::string, GLuint> textures;
};
