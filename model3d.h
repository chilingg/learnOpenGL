#ifndef MODEL3D_H
#define MODEL3D_H

#include "mshader.h"
#include "mesh.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
    public:
        /*  函数   */
        Model(char *path) { loadModel(path); }
        void draw(MShader shader);

    private:
        /*  模型数据  */
        std::vector<Mesh> meshes;
        std::string directory;
        /*  函数   */
        void loadModel(const std::string &path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                        std::string typeName);
};

inline void Model::draw(MShader shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);
}

#endif // MODEL3D_H
