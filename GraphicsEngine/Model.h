#ifndef MODEL_H
#define MODEL_H
#include"shader.h"
#include"Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model() = default;
    Model(const char* path)
    {
        loadModel(path);
    }
    void Rendering();
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void Calculate_uv_spherical(vector<Vertex>& vertices);
};

class Transform {
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotate;

    Transform() : position(0.0f), scale(1.0f), rotate(0.0f) {}

    glm::mat4 GetModelMatrix() const;
};
#endif