#pragma once
#include <glm/glm.hpp>

enum class Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class Camera {
  public:
    constexpr static float YAW = -90.0f;
    constexpr static float PITCH = 0.0f;
    constexpr static float SPEED = 2.5f;
    constexpr static float SENSE = 0.1f;
    constexpr static float ZOOM = 45.0f;
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    Camera(
        glm::vec3 position = glm::vec3{},
        glm::vec3 up = glm::vec3{0,1,0},
        float yaw = YAW,
        float pitch = PITCH);
    glm::mat4 GetViewMatrix() const;
    void ProcessKeyboard(Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
  private:
    void updateCameraVectors();
};

