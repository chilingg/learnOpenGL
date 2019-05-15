#ifndef MODEL3D_H
#define MODEL3D_H

#include "mshader.h"
#include "mesh.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int textureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model3d
{
    struct TextureGloble
    {
        unsigned int id;
        std::string type;
        aiString path;  // 我们储存纹理的路径用于与其它纹理进行比较
    };

    public:
        /*  函数   */
        Model3d(const char *path) { loadModel(path); }
        void draw(MShader shader);

    private:
        /*  模型数据  */
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<TextureGloble> textures_loaded;
        /*  函数   */
        void loadModel(const std::string &path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                        std::string typeName);
};

inline void Model3d::draw(MShader shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);
}

#endif // MODEL3D_H
