//
// Created by Nikolay Fedotenko on 28.11.2021.
//

#pragma once

/*! \file
 *  This header declares Camera class
 */

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*! \enum
 *  Defines several possible options for camera movement.
 *  Used as abstraction to stay away from window-system specific input methods.
 */
enum ECameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/*! \brief
 *  Default camera values
 */
const GLfloat YAW         = -90.0f;
const GLfloat PITCH       =  0.0f;
const GLfloat SPEED       =  6.0f;
const GLfloat SENSITIVITY =  0.25f;
const GLfloat ZOOM        =  45.0f;


/*! \class
 *  An abstract camera class that processes input and calculates the corresponding
 *  Eular Angles, Vectors and Matrices for use in OpenGL.
 */
class Camera
{
public:
    /*! \brief
     *  Constructor with vectors
     *  \param position
     *  \param up
     *  \param yaw
     *  \param pitch
     */
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat   yaw      = YAW,
           GLfloat   pitch    = PITCH);

    /*! \brief
     *  Constructor with scalar values
     *  \param posX
     *  \param posY
     *  \param posZ
     *  \param upX
     *  \param upY
     *  \param upZ
     *  \param yaw
     *  \param pitch
     *  \note May be unused
     */
    [[maybe_unused]] Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
                            GLfloat upX,  GLfloat upY,  GLfloat upZ,
                            GLfloat yaw,
                            GLfloat pitch);

    /*! \brief
     *  View matrix getter
     *  \return The view matrix calculated using Eular Angles and the LookAt Matrix
     *  \see glm::lookAt
     */
    glm::mat4 getViewMatrix();

    /*! \brief
     *  Processes input received from any keyboard-like input system
     *  \param direction Camera defined enum
     *  \param deltaTime Time passed
     */
    void processKeyboard(ECameraMovement direction, GLfloat deltaTime);

    /*! \brief
     *  Processes input received from a mouse input system
     *  \param xOffset Offset value in X-axis
     *  \param yOffset Offset value in Y-axis
     *  \param constrainPitch Should constrain pitch? True by default
     */
    void processMouseMovement(GLfloat xOffset,
                              GLfloat yOffset,
                              GLboolean constrainPitch = true);

    /*! \brief
     *  Processes input received from a mouse scroll-wheel event
     *  \param yOffset Vertical wheel-axis
     */
    void processMouseScroll(GLfloat yOffset);

    /*! \brief
     *  Getter for the zoom
     *  \see m_zoom
     */
    [[nodiscard]] GLfloat getZoom() const;

private:
    /*! \brief
     *  Camera Attributes
     */
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    /*! \brief
     *  Eular Angles
     */
    GLfloat m_yaw;
    GLfloat m_pitch;

    /*! \brief
     *  Camera options
     */
    GLfloat m_movementSpeed;
    GLfloat m_mouseSensitivity;
    GLfloat m_zoom;

    /*! \brief
     *  Method to calculate the front vector from the Camera's (updated) Eular Angles
     */
    void updateCameraVectors();
};
