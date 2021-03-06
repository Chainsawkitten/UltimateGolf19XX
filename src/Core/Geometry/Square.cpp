#include "Square.hpp"

namespace Geometry {
    Square::Square() {
        // Vertices
        vertexNr = 4;
        vertexData = new Vertex[vertexNr];
    
        vertexData[0] = {
            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
        vertexData[1] = {
            glm::vec3(-0.5f, -0.5f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
        vertexData[2] = {
            glm::vec3(0.5f, -0.5f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
        vertexData[3] = {
            glm::vec3(-0.5f, 0.5f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        };
    
        // Vertexindices
        indexNr = 6;
        indexData = new unsigned int[indexNr];
    
        indexData[0] = 0;
        indexData[1] = 1;
        indexData[2] = 2;
        indexData[3] = 0;
        indexData[4] = 3;
        indexData[5] = 1;
    
        GenerateBuffers();
        GenerateVertexArray();
    }
    
    Square::~Square() {
        delete[] vertexData;
        delete[] indexData;
    }
    
    Geometry3D::Vertex* Square::Vertices() const {
        return vertexData;
    }
    
    unsigned int Square::VertexCount() const {
        return vertexNr;
    }
    
    unsigned int* Square::Indices() const {
        return indexData;
    }
    
    unsigned int Square::IndexCount() const {
        return indexNr;
    }
}
