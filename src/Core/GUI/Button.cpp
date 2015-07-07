#include "Button.hpp"
#include "SingleColor2D.vert.hpp"
#include "SingleColor2D.frag.hpp"
#include "../Resources.hpp"

namespace GUI {
    Button::Button(Widget* parent) : Widget(parent) {
        rectangle = Resources::GetInstance().CreateRectangle();
        
        vertexShader = Resources::GetInstance().CreateShader(SINGLECOLOR2D_VERT, SINGLECOLOR2D_VERT_LENGTH, GL_VERTEX_SHADER);
        fragmentShader = Resources::GetInstance().CreateShader(SINGLECOLOR2D_FRAG, SINGLECOLOR2D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
        shaderProgram = Resources::GetInstance().CreateShaderProgram({ vertexShader, fragmentShader });
        
        mouseHover = false;
        size = glm::vec2(64.f, 64.f);
    }
    
    Button::~Button() {
        Resources::GetInstance().FreeShader(SINGLECOLOR2D_VERT);
        Resources::GetInstance().FreeShader(SINGLECOLOR2D_FRAG);
        Resources::GetInstance().FreeShaderProgram(shaderProgram);
        
        Resources::GetInstance().FreeRectangle();
    }
    
    void Button::Update(GLFWwindow* window) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        mouseHover = xpos >= Position().x && xpos < Position().x + size.x && ypos >= Position().y && ypos < Position().y + size.y;
    }
    
    void Button::Render(int screenWidth, int screenHeight) {
        shaderProgram->Use();
        
        // Set color.
        glm::vec3 color(0.06666666666f, 0.06274509803f, 0.08235294117f);
        if (mouseHover)
            color = glm::vec3(0.16078431372f, 0.15686274509f, 0.17647058823f);
        glUniform3fv(shaderProgram->UniformLocation("color"), 1, &color[0]);
        
        // Set location and size.
        glUniform2fv(shaderProgram->UniformLocation("position"), 1, &Position()[0]);
        glUniform2fv(shaderProgram->UniformLocation("size"), 1, &size[0]);
        
        // Set screen size.
        glm::vec2 screenSize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
        glUniform2fv(shaderProgram->UniformLocation("screenSize"), 1, &screenSize[0]);
        
        glBindVertexArray(rectangle->VertexArray());
        
        glDrawElements(GL_TRIANGLES, rectangle->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
    
    glm::vec2 Button::Size() const {
        return size;
    }
}
