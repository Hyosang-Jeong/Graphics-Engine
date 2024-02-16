#include "Camera.h"

Camera::Camera()
{
	cameraPos = { 0,0,10 };
}

Camera::Camera(GLFWwindow* ptr_window,glm::vec3 position):ptr_window(ptr_window)
{
	cameraPos = position;
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -10.0f);
	 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	 glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	 cameraUp = glm::cross(cameraDirection, cameraRight);
	 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::Update(float dt)
{
	if (glfwGetKey(ptr_window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += 0.05f * cameraFront;
	if (glfwGetKey(ptr_window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= 0.05f * cameraFront;
	if (glfwGetKey(ptr_window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * 0.05f;
	if (glfwGetKey(ptr_window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * 0.05f;
	if (glfwGetKey(ptr_window, GLFW_KEY_E) == GLFW_PRESS)
		Yaw += 0.5f;
	if (glfwGetKey(ptr_window, GLFW_KEY_Q) == GLFW_PRESS)
		Yaw -= 0.5f;
	if (glfwGetKey(ptr_window, GLFW_KEY_Z) == GLFW_PRESS)
		Pitch += 0.5f;
	if (glfwGetKey(ptr_window, GLFW_KEY_C) == GLFW_PRESS)
		Pitch -= 0.5f;
	cameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	cameraDirection.y = sin(glm::radians(Pitch));
	cameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	cameraFront = glm::normalize(cameraDirection);



}


