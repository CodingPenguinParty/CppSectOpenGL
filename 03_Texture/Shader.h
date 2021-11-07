//
// Created by Nikolay Fedotenko on 29.10.2021.
//

#pragma once

/*! \file
 *  This header declares Shader class
 */

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*! \class
 *  Wrapper class for using GLSL shaders
 */
class Shader
{
public:
    /*! \brief
     *  Parameterized constructor
     *  \param Path to vertex shader
     *  \param Path to fragment shader
     *  \throws std::ifstream::failure
     */
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    /*! \brief
     *  Getter for shader's program
     *  \see m_program
     */
    [[nodiscard]] GLuint getProgram() const;

    /*! \brief
     *  Constant method to call glUseProgram
     */
    void use() const;

private:
    GLuint m_program;
};
