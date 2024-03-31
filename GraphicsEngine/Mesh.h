#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GL/glew.h> 
#include<glm/glm.hpp>
#include<string>
#include<map>
#include"shader.h"

using namespace std;
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

class Mesh 
{
public:

    vector<Vertex> vertices;
    vector<unsigned int> indices;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices);

    void Draw();

    void setupMesh();


    unsigned int VAO, VBO, EBO;

    glm::vec3 Position{ 0 };
    glm::vec3 Scale{ 1 };
    glm::vec3 Rotate{ 0 };
};


#endif // !MESH_H
