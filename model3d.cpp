#include "model3d.h"

void Model::loadModel(const std::string &path)
{
    Assimp::Importer import;
    //aiProcess_Triangulate将所有图元加载为三角形，aiProcess_FlipUVs翻转纹理坐标Y轴
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate|aiProcess_FlipUVs);

    if(!scene || scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // 处理节点所有的网格（如果有的话）
    for(unsigned i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back((processMesh(mesh, scene)));
    }
    // 接下来对它的子节点重复这一过程
    for(unsigned i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}
