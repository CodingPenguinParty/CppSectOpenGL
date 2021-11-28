//
// Created by Nikolay Fedotenko on 28.11.2021.
//

#include <iostream>

#include <SOIL2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

const GLuint WINDOW_WIDTH  = 800;
const GLuint WINDOW_HEIGHT = 600;

/// Camera declarations
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f) );
GLfloat lastX = WINDOW_WIDTH / 2.0;
GLfloat lastY = WINDOW_HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

/// Time definitions (for updates in game loop)
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

/// Function declarations

/*! \brief
 *  A function that moves/alters the camera positions based on user input
 */
void doMovement();

/*! \brief
 *  Keyboard callback function
 *  Is called whenever a key is pressed/released via GLFW
 *  \param window Pointer to the current GLFW window
 *  \param key Keyboard key
 *  \param scancode Platform-specific scancode
 *  \param action Key action
 *  \param mode Modifier bits
 */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

/*! \brief
 *  Mouse cursor callback function
 *  \param window Pointer to the current GLFW window
 *  \param xPos Cursor position on X-axis
 *  \param yPos Cursor position on Y-axis
 */
void mouseCallback(GLFWwindow* window, double xPos, double yPos);

/*! \brief
 *  Mouse scroll callback function
 *  \param window Pointer to the current GLFW window
 *  \param xOffset Offset value in X-axis
 *  \param yOffset Offset value in Y-axis
 */
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);


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
                                          "C++ sect: OpenGL (lecture 5)", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create window!" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    // Use callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Use Z-buffer
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader ourShader("Shaders/basic.vert", "Shaders/basic.frag");

    // Set up vertex data (with buffers) and attribute pointers
    GLfloat vertices[] =
            {
                    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
                     0.5f, -0.5f, 0.5f,  1.0f, 0.0f,
                     0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
                     0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
                    -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,
                    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffers and attribute pointers.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

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
        // Set frame time
        auto currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        doMovement();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        // Activate texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(ourShader.getProgram(), "ourTexture"), 0);

        glm::mat4 projection;
        projection = glm::perspective(camera.getZoom(),
                                      (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);

        // Create camera transformation
        glm::mat4 view;
        view = camera.getViewMatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(ourShader.getProgram(), "model");
        GLint viewLoc  = glGetUniformLocation(ourShader.getProgram(), "view");
        GLint projLoc  = glGetUniformLocation(ourShader.getProgram(), "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr(view) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr(projection) );

        glBindVertexArray(VAO);

        // Calculate the model matrix for the object and pass it to shader before drawing
        glm::mat4 model(1);
        model = glm::translate( model, glm::vec3(0.0f, 0.0f, 0.0f) );
        GLfloat angle = 3.0f * (GLfloat)glfwGetTime();
        model = glm::rotate( model, angle, glm::vec3(0.0f, 1.0f, 0.0f) );
        glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr(model) );

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}

/// Function definitions

void doMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.processKeyboard(FORWARD, deltaTime);
    else if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.processKeyboard(BACKWARD, deltaTime);
    else if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.processKeyboard(LEFT, deltaTime);
    else if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.processKeyboard(RIGHT, deltaTime);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key >= 0 && key < 1024)
        keys[key] = action == GLFW_PRESS;
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.processMouseScroll( (GLfloat)yOffset );
}
