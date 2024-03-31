#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GL/glew.h> 
#include<glm/glm.hpp>
#include<string>
#include<map>
#include"shader.h"

using namespace std;
#define MAX_BONE_INFLUENCE 4
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};


struct Texture {
    Texture() = default;
    Texture(unsigned id, string type) :id(id),type(type) {}
    unsigned int id;
    string type;
    string path;  
};

class Mesh 
{
public:

    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw();

    void setupMesh();


    unsigned int VAO, VBO, EBO;

    glm::vec3 Position{ 0 };
    glm::vec3 Scale{ 1 };
    glm::vec3 Rotate{ 0 };
};


#endif // !MESH_H
