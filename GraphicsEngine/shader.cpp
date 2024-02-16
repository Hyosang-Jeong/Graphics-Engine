#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <cstdlib>
#include <cstring>

#include <GL/glew.h>

#include "shader.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


Shader::Shader(const char* vertexPath_, const char* fragmentPath_)
{
    ID = setup_shdrpgm(vertexPath_, fragmentPath_);
    vertexPath = vertexPath_;
    fragmentPath = fragmentPath_;
}

Shader::Shader(const char* computePath_)
{
    ID = setup_shdrpgm(computePath_);
    computePath = computePath_;
}

void Shader::active()
{
    glUseProgram(ID);
}

void Shader::reload()
{
    if (vertexPath != nullptr && fragmentPath != nullptr)
    {
        ID = setup_shdrpgm(vertexPath, fragmentPath);
        return;
    }
    else
    {
        ID = setup_shdrpgm(computePath);
        return;
    }
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 m) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &m[0].x);
}
void Shader::setVec3(const std::string& name, glm::vec3 v) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &v.x);
}

GLuint setup_shdrpgm(const char* vertexPath, const char* fragmentPath)
{
        std::string vert = "../shaders/";
        std::string frag = "../shaders/";
        vert = vert + vertexPath;
        frag = frag + fragmentPath;

        return LoadShaders(vert.c_str(), frag.c_str());
}

GLuint setup_shdrpgm(const char* computePath)
{
    std::string compute = "../shaders/";
    compute = compute + computePath;

    return LoadShaders(compute.c_str());
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line;
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line;
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}


	// Compile Fragment Shader
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}


	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint LoadShaders(const char* compute_file_path)
{
    // Create the shaders
    GLuint ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);

    // Read the Vertex Shader code from the file
    std::string ComputeShaderCode;
    std::ifstream ComputeShaderStream(compute_file_path, std::ios::in);
    if (ComputeShaderStream.is_open()) {
        std::string Line;
        while (getline(ComputeShaderStream, Line))
            ComputeShaderCode += "\n" + Line;
        ComputeShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", compute_file_path);
        getchar();
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Compute Shader
    char const* ComputeSourcePointer = ComputeShaderCode.c_str();
    glShaderSource(ComputeShaderID, 1, &ComputeSourcePointer, nullptr);
    glCompileShader(ComputeShaderID);

    // Check Compute Shader
    glGetShaderiv(ComputeShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(ComputeShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ComputeShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(ComputeShaderID, InfoLogLength, nullptr, &ComputeShaderErrorMessage[0]);
        printf("%s\n", &ComputeShaderErrorMessage[0]);
    }

    // Link the program
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, ComputeShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    glDetachShader(ProgramID, ComputeShaderID);
    glDeleteShader(ComputeShaderID);

    return ProgramID;
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

GLuint LoadPipeline(const char * vertex_file_path,const char * fragment_file_path, GLuint * programIDs )
{
    // Hardcoding the values to 2
    
    // Vertex Shader + Program
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line;
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    programIDs[0] = glCreateProgram();
    glAttachShader(programIDs[0], VertexShaderID);
    
    // IMP: Do this BEFORE linking the program
    glProgramParameteri(programIDs[0],
                        GL_PROGRAM_SEPARABLE,
                        GL_TRUE );
    
    glLinkProgram(programIDs[0]);
    
    // Check the program
    glGetProgramiv(programIDs[0], GL_LINK_STATUS, &Result);
    glGetProgramiv(programIDs[0], GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(programIDs[0], InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
    
    
    
    
    
    // Fragment Shader + Program
    
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line;
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    
    Result = GL_FALSE;
    InfoLogLength = 0;
    
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , nullptr);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    
    // Link the program
    printf("Linking program\n");
    programIDs[1] = glCreateProgram();
    glAttachShader(programIDs[1], FragmentShaderID);
    
    // IMP: Do this BEFORE linking the progam
    glProgramParameteri(programIDs[1],
                        GL_PROGRAM_SEPARABLE,
                        GL_TRUE );
    
    glLinkProgram(programIDs[1]);
    
    GLuint pipeLineID;
    glGenProgramPipelines( 1, &pipeLineID );

    glUseProgramStages( pipeLineID, GL_VERTEX_SHADER_BIT, programIDs[0]);
    glUseProgramStages( pipeLineID, GL_FRAGMENT_SHADER_BIT, programIDs[1]);

    return pipeLineID;
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////

GLuint LoadMultiShaders(const char *vertex_file_path, const char *fragment_file_path,
                        const char *geom_file_path,
                        Primitive_Enum out_primitive_type)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;

    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::string Line;
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    } else
    {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n",
               vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
        std::string Line;
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }


    // array of char * to hold the strings
    std::vector<const char *> shaderStrings;
    shaderStrings.clear();

    // First string
    std::string geometryShaderHeader("#version 450 core\n"
                                     "\n"
                                     "const int maxVerts = 4;");
    shaderStrings.push_back(geometryShaderHeader.c_str());

    // Second string
    std::string geometryInOut = "// Type of primitive input\n  // from VS (CPU)\n layout (points) in;";

    std::string temp;
    switch (out_primitive_type)
    {
        case TriangleStrip:
            temp = "triangle_strip";
            break;

        case LineStrip:
            temp = "line_strip";
            break;

        case Points:
            temp = "points";
            break;
    };

    geometryInOut += "layout (" + temp + ", max_vertices = maxVerts) out;\n";
    shaderStrings.push_back(geometryInOut.c_str());

    // Third string
    // Read the Geometry Shader from the file
    std::string GeometryShaderCode;
    std::ifstream GeometryShaderStream(geom_file_path, std::ios::in);
    if (GeometryShaderStream.is_open())
    {
        std::string Line;
        while (getline(GeometryShaderStream, Line))
            GeometryShaderCode += "\n" + Line;
        GeometryShaderStream.close();
    }

    shaderStrings.push_back(GeometryShaderCode.c_str());
    //////////////////////////////////////////////////////////


    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> VertexShaderErrorMessage(static_cast<unsigned long>(InfoLogLength + 1));
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }


    // Compile Geometry Shader
    printf("Compiling shader : %s\n", geom_file_path);
    char const **GeometrySourcePointer = shaderStrings.data(); //GeometryShaderCode.c_str();
    glShaderSource(GeometryShaderID, 3, GeometrySourcePointer, nullptr);
    glCompileShader(GeometryShaderID);

    // Check Fragment Shader
    glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> GeometryShaderErrorMessage((unsigned long) (InfoLogLength + 1));
        glGetShaderInfoLog(GeometryShaderID, InfoLogLength, nullptr, &GeometryShaderErrorMessage[0]);
        printf("%s\n", &GeometryShaderErrorMessage[0]);
    }


    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> FragmentShaderErrorMessage(static_cast<unsigned long>(InfoLogLength + 1));
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, GeometryShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(static_cast<unsigned long>(InfoLogLength + 1));
        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, GeometryShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(GeometryShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}


