#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include<GLFW/glfw3.h>

class Camera
{
public:
	Camera();
    Camera(GLFWwindow* ,glm::vec3 position);
    glm::mat4 GetView();
    void Update(float dt);
    glm::vec3 GetPosition() { return cameraPos; }
private:
    GLFWwindow* ptr_window;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraDirection{ 0 };
    glm::vec3 cameraRight{ 0 };
    glm::vec3 cameraUp{ 0 };
    glm::vec3 cameraFront{ 0 };
    // euler Angles
    float Yaw{ -90.0f};
    float Pitch{ 0 };
};