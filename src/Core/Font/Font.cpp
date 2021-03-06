#include "Font.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <stdio.h>
#include <fstream>
#include "../Util/Log.hpp"
#include "../Resources.hpp"
#include "Font.vert.hpp"
#include "Font.frag.hpp"

Font::Font(const char* filename, float height) {
    this->height = height;
    
    // Read font file into buffer.
    std::ifstream fontFile;
    fontFile.open(filename, std::ios_base::binary | std::ios_base::ate);
    if (!fontFile.is_open())
        Log() << "Couldn't open font file " << filename << " for reading.\n";
    
    unsigned int size = static_cast<unsigned int>(fontFile.tellg());
    unsigned char* ttfBuffer = new unsigned char[size];
    fontFile.seekg(0, std::ios::beg);
    fontFile.read(reinterpret_cast<char*>(ttfBuffer), size);
    fontFile.close();
    
    unsigned char* tempBitmap = new unsigned char[512 * 512];
    
    stbtt_BakeFontBitmap(ttfBuffer, 0, height, tempBitmap, 512, 512, 32, 96, cdata);
    
    delete[] ttfBuffer;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tempBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    delete[] tempBitmap;
    
    // Resources.
    rectangle = Resources().CreateRectangle();
    
    vertexShader = Resources().CreateShader(FONT_VERT, FONT_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Resources().CreateShader(FONT_FRAG, FONT_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    
    color = glm::vec3(0.f, 0.f, 0.f);
}

Font::Font(const char* source, int sourceLength, float height) {
    this->height = height;
    
    unsigned char* tempBitmap = new unsigned char[512 * 512];
    
    stbtt_BakeFontBitmap(reinterpret_cast<const unsigned char*>(source), 0, height, tempBitmap, 512, 512, 32, 96, cdata);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tempBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    delete[] tempBitmap;
    
    // Resources.
    rectangle = Resources().CreateRectangle();
    
    vertexShader = Resources().CreateShader(FONT_VERT, FONT_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Resources().CreateShader(FONT_FRAG, FONT_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    
    color = glm::vec3(0.f, 0.f, 0.f);
}

Font::~Font() {
    glDeleteTextures(1, &texture);
    
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    Resources().FreeShaderProgram(shaderProgram);
    
    Resources().FreeRectangle();
}

stbtt_aligned_quad Font::BakedQuad(char character, float& x, float& y) {
    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(cdata, 512, 512, character - 32, &x, &y, &q, 1);
    return q;
}

void Font::RenderText(const char* text, const glm::vec2& position, float wrap, const glm::vec2& screenSize) {
    // Disable depth testing
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    
    shaderProgram->Use();
    
    // Blending enabled
    GLboolean blend = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindVertexArray(rectangle->VertexArray());
    
    // Texture unit 0 is for base images.
    glUniform1i(shaderProgram->UniformLocation("baseImage"), 0);
    
    // Base image texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glm::vec2 pos = position;
    pos.y += height;
    while (*text) {
        if (*text >= 32 && *text < 128) {
            pos.x = RenderCharacter(*text, pos, screenSize);
        }
        
        if (pos.x > position.x + wrap || *text == '\n') {
            pos.x = position.x;
            pos.y += height;
        }
        ++text;
    }
    
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    if (!blend)
        glDisable(GL_BLEND);
}

glm::vec3 Font::Color() const {
    return color;
}

void Font::SetColor(const glm::vec3& color) {
    this->color = color;
}

float Font::Height() const {
    return height;
}

float Font::RenderCharacter(char character, const glm::vec2& position, const glm::vec2& screenSize) {
    stbtt_aligned_quad q;
    float x = position.x;
    float y = position.y;
    q = BakedQuad(character, x, y);
    
    glm::vec2 pos = glm::vec2(q.x0, q.y0);
    glm::vec2 siz = glm::vec2(q.x1, q.y1) - glm::vec2(q.x0, q.y0);
    
    glUniform2fv(shaderProgram->UniformLocation("position"), 1, &(pos / screenSize)[0]);
    glUniform2fv(shaderProgram->UniformLocation("size"), 1, &(siz / screenSize)[0]);
    
    glm::vec2 uv1 = glm::vec2(q.s0, q.t0);
    glm::vec2 uv2 = glm::vec2(q.s1, q.t1);
    glUniform2fv(shaderProgram->UniformLocation("uv1"), 1, &uv1[0]);
    glUniform2fv(shaderProgram->UniformLocation("uv2"), 1, &uv2[0]);
    
    glUniform3fv(shaderProgram->UniformLocation("color"), 1, &color[0]);
    
    glDrawElements(GL_TRIANGLES, rectangle->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    return x;
}
