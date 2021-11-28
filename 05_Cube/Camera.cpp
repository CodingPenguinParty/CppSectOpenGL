//
// Created by Nikolay Fedotenko on 28.11.2021.
//

#include "Camera.h"

/// Public methods

Camera::Camera(glm::vec3 position,
               glm::vec3 up,
               GLfloat   yaw,
               GLfloat   pitch)
    : m_front           ( glm::vec3(0.0f, 0.0f, -1.0f) )
    , m_movementSpeed   ( SPEED )
    , m_mouseSensitivity( SENSITIVITY )
    , m_zoom            ( ZOOM )
{
    m_position = position;
    m_worldUp  = up;
    m_yaw      = yaw;
    m_pitch    = pitch;

    updateCameraVectors();
}

[[maybe_unused]] Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
               GLfloat upX,  GLfloat upY,  GLfloat upZ,
               GLfloat yaw,
               GLfloat pitch)
    : m_front           ( glm::vec3(0.0f, 0.0f, -1.0f) )
    , m_movementSpeed   ( SPEED )
    , m_mouseSensitivity( SENSITIVITY )
    , m_zoom            ( ZOOM )
{
    m_position = glm::vec3(posX, posY, posZ);
    m_worldUp  = glm::vec3(upX, upY, upZ);
    m_yaw      = yaw;
    m_pitch    = pitch;

    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() { return glm::lookAt(m_position, m_position + m_front, m_up); }

void Camera::processKeyboard(ECameraMovement direction, GLfloat deltaTime)
{
    GLfloat velocity = m_movementSpeed * deltaTime;

    switch (direction)
    {
        case FORWARD:
            m_position += m_front * velocity;
            break;
        case BACKWARD:
            m_position -= m_front * velocity;
            break;
        case LEFT:
            m_position -= m_right * velocity;
            break;
        case RIGHT:
            m_position += m_right * velocity;
            break;
        default:
            break;
    }
}

void Camera::processMouseMovement(GLfloat xOffset,
                                  GLfloat yOffset,
                                  GLboolean constrainPitch)
{
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw   += xOffset;
    m_pitch += yOffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;

        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yOffset)
{
    if (m_zoom >= 1.0f && m_zoom <= 45.0f)
        m_zoom -= yOffset;

    if (m_zoom <= 1.0f)
        m_zoom = 1.0f;

    if (m_zoom >= 45.0f)
        m_zoom = 45.0f;
}

GLfloat Camera::getZoom() const { return m_zoom; }

/// Private methods

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos( glm::radians(m_yaw) ) * cos(glm::radians(m_pitch) );
    front.y = sin( glm::radians(m_pitch) );
    front.z = sin( glm::radians(m_yaw) ) * cos(glm::radians(m_pitch) );
    m_front = glm::normalize(front);
    // Also re-calculate the Right and Up vector.
    // Normalize the vectors, because their length gets closer to 0 the more
    // you look up or down which results in slower movement.
    m_right = glm::normalize(glm::cross(m_front, m_worldUp) );
    m_up    = glm::normalize(glm::cross(m_right, m_front) );
}
