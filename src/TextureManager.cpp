#include "TextureManager.h"
#include "Config.h"
#include <SOIL/SOIL.h>
#include <iostream>
#include <stdexcept>

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
    releaseAllTextures();
}

GLuint TextureManager::loadTexture(const std::string& name, const std::string& filename) {
    GLuint textureID = SOIL_load_OGL_texture(
        filename.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if (textureID == 0) {
        std::cerr << "[TextureManager] Failed to load texture '" << name
                  << "' from file: " << filename << "\nSOIL Error: " << SOIL_last_result() << std::endl;
        throw std::runtime_error("Failed to load texture: " + filename);
    }

    textures[name] = textureID;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "[TextureManager] Loaded texture '" << name << "' (ID: " << textureID << ") from " << filename << std::endl;
    return textureID;
}

GLuint TextureManager::getTexture(const std::string& name) const {
    auto it = textures.find(name);
    if (it == textures.end()) {
        std::cerr << "[TextureManager] Texture '" << name << "' not found." << std::endl;
        throw std::out_of_range("Texture not found: " + name);
    }
    return it->second;
}

void TextureManager::bindTexture(const std::string& name, GLenum textureUnit) const {
    try {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, getTexture(name));
    } catch (const std::out_of_range&) {
        glBindTexture(GL_TEXTURE_2D, 0);
        std::cerr << "[TextureManager] Warning: Attempted to bind missing texture '" << name << "'. Bound default instead.\n";
    }
}

void TextureManager::releaseAllTextures() {
    for (const auto& [name, id] : textures) {
        glDeleteTextures(1, &id);
    }
    textures.clear();
    std::cout << "[TextureManager] Released all textures." << std::endl;
}
