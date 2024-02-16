#pragma once

#include<gl/glew.h>
#include<string>
#include<glm/glm.hpp>
class Shader
{
public:
    // the program ID
    GLuint  ID;
    Shader() = default;
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const char* computePath);
    // use/activate the shader
    void active();
    void reload();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, glm::mat4 m) const;
    void setVec3(const std::string& name, glm::vec3 v) const;
private:
    const char* vertexPath{ nullptr };
    const char* fragmentPath{ nullptr };
    const char* computePath{ nullptr };
};


enum Primitive_Enum
{
    TriangleStrip = 0,
    Points,
    LineStrip
};
GLuint setup_shdrpgm(const char* vertexPath, const char* fragmentPath);
GLuint setup_shdrpgm(const char* computePath);

///////////////////
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

/// ////////////////
GLuint LoadShaders(const char * compute_file_path);


///////////////////
GLuint LoadPipeline(const char * vertex_file_path,const char * fragment_file_path, GLuint * programIDs );

// Load shaders where multiple shader files == one complete shader
// Show how code can be reused across shaders
GLuint LoadMultiShaders(const char *vertex_file_path, const char *fragment_file_path,
                        const char *geom_file_path, Primitive_Enum  out_primitive_type );


