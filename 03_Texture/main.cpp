#include <iostream>

#include <SOIL2/SOIL2.h>

#include "Shader.h"

const GLuint WINDOW_WIDTH  = 800;
const GLuint WINDOW_HEIGHT = 600;

/*! \brief
 *  Main function, inits and runs everything
 */
int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "C++ sect: OpenGL (lecture 3)", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create window!" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Build and compile our shader program
    Shader ourShader("Shaders/basic.vert", "Shaders/basic.frag");

    // Set up vertex data (with buffers) and attribute pointers
    GLfloat vertices[] =
            {
                    // Positions          // Colors           // Texture coords
                    0.5f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // Top Right
                    0.5f,  -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // Bottom Right
                    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Bottom Left
                    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // Top Left
            };

    GLuint indices[] =
            {
                    0, 1, 3,  // First triangle
                    1, 2, 3   // Second triangle
            };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffers and attribute pointers.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);  // Unbind VAO

    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;

    // Load, create texture and generate mipmaps
    unsigned char* image = SOIL_load_image("Images/monkey.jpg",
                                           &width, &height, nullptr, SOIL_LOAD_RGBA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Game (main) loop
    while ( !glfwWindowShouldClose(window) )
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        // Activate texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(ourShader.getProgram(), "ourTexture"), 0);

        // Draw triangles
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}
