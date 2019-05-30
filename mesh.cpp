#include "mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<Mesh::Vertex> &vertice,
           const std::vector<unsigned> &indices,
           const std::vector<Mesh::Texture> &textrue):
    vertices(vertice),
    indices(indices),
    textures(textrue)
{
    setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::draw(const MShader &shader) const
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    if(!shader.notDrawTex)
    {
        if(textures.size() == 0)
        {
            shader.setUniform3F("OneMaterial.objColor", glm::vec3(0.5));
        }
        else {
            for(unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
                // 获取纹理序号（diffuse_textureN 中的 N）
                std::string number;
                std::string name = textures[i].type;
                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = std::to_string(specularNr++);

                shader.setUniform1F(("OneMaterial." + name + number).c_str(), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            shader.setUniform3F("OneMaterial.objColor", glm::vec3(0.0));
        }
    }
    glActiveTexture(GL_TEXTURE0);

    // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::deleteBuffer() const
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VAO);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned),
                 &indices[0], GL_STATIC_DRAW);

    //顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));
    //顶点法线
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    //顶点纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
    glEnableVertexAttribArray(2);
    //offset(t, d)预处理指令，参数1是一个结构体，参数2是其变量的名字，宏会返回变量距结构体头部的字节偏移量

    glBindVertexArray(0);
}
