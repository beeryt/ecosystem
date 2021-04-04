#include <algorithm>
#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch) :
  Position(position),
  Front(vec3{0,0,-1}),
  Up(up),
  Right(vec3{}),
  WorldUp(up),
  Yaw(yaw),
  Pitch(pitch),
  MovementSpeed(SPEED),
  MouseSensitivity(SENSE),
  Zoom(ZOOM) {}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  if (constrainPitch) {
    Pitch = std::min(Pitch, 89.0f);
    Pitch = std::max(Pitch, -89.0f);
  }
  updateCameraVectors();
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = std::cos(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
  front.y = std::sin(glm::radians(Pitch));
  front.z = std::sin(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
  Front = glm::normalize(front);

  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}
