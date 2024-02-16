#include "Model.h"
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include<iostream>
void Model::Rendering()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw();
}


void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    glm::vec3 min{ vertices[0].Position };
    glm::vec3 max{ vertices[0].Position };
    for (auto i = 1; i < vertices.size(); i++)
    {
        if (vertices[i].Position.x < min.x)
        {
            min.x = vertices[i].Position.x;
        }
        if (vertices[i].Position.y < min.y)
        {
            min.y = vertices[i].Position.y;
        }
        if (vertices[i].Position.z < min.z)
        {
            min.z = vertices[i].Position.z;
        }
        if (vertices[i].Position.x > max.x)
        {
            max.x = vertices[i].Position.x;
        }
        if (vertices[i].Position.y > max.y)
        {
            max.y = vertices[i].Position.y;
        }
        if (vertices[i].Position.z > max.z)
        {
            max.z = vertices[i].Position.z;
        }
    }
    for (auto i = 0; i < vertices.size(); i++)
    {
        vertices[i].Position.x = max.x == min.x ? 0 : (2.f * ((vertices[i].Position.x - min.x) / (max.x - min.x))) - 1.f;
        vertices[i].Position.y = max.y == min.y ? 0 : (2.f * ((vertices[i].Position.y - min.y) / (max.y - min.y))) - 1.f;
        vertices[i].Position.z = max.z == min.z ? 0 : (2.f * (( vertices[i].Position.z - min.z) / (max.z - min.z))) - 1.f;
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; 
    Calculate_uv_spherical(vertices);
    return Mesh(vertices, indices, textures);
}


void Model::Calculate_uv_spherical(vector<Vertex>& vertices)
{
    for (auto& vertex : vertices)
    {
        glm::vec3 v = vertex.Position;

        // Calculate azimuth angle (ux) and normalize to [0, 1] range
        float azimuth = glm::degrees(atan2(v.z, v.x)) + 180.f;
        float normalizedAzimuth = azimuth / 360.f;

        // Calculate polar angle (uy) and normalize to [0, 1] range
        float polar = (v.y + 1.f) / 2.f;

        // Set normalized UV coordinates
        vertex.TexCoords.x = normalizedAzimuth;
        vertex.TexCoords.y = polar;
    }
}


glm::mat4 Transform::GetModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    return model;
}
