#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "mshader.h"

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };
    struct Texture
    {
        unsigned id;
        std::string type;
    };

    Mesh(const std::vector<Vertex> &vertice,
         const std::vector<unsigned> &indices,
         const std::vector<Texture> &textrue);
    ~Mesh();
    void draw(const MShader &shader, unsigned promitive, bool notLoadTex = false) const;
    void drawInstance(const MShader &shader, unsigned acount, unsigned promitive, bool notLoadTex = false) const;
    void deleteBuffer() const;
    void bindVAO() const;

    //网格数据
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

private:
    unsigned VAO, VBO, EBO;
    void setupMesh();
};

inline void Mesh::bindVAO() const
{
    glBindVertexArray(VAO);
}

#endif // MESH_H
