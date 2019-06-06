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
        void deleteBuffer();
        void draw(MShader shader, unsigned primitive, bool notLoadTex = false);
        void drawInstance(MShader shader, unsigned acount, unsigned primitive, bool notLoadTex = false);
        std::vector<Mesh>::size_type meshesSize() const;
        const Mesh& getMeshe(std::vector<Mesh>::size_type index) const;

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

inline std::vector<Mesh>::size_type Model3d::meshesSize() const
{
    return meshes.size();
}

inline const Mesh &Model3d::getMeshe(std::vector<Mesh>::size_type index) const
{
    return meshes[index];
}

#endif // MODEL3D_H
