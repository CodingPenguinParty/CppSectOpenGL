//
// Created by Nikolay Fedotenko on 18.12.2021.
//

#include <iostream>
#include <cmath>

#include <SOIL2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

/// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

/// Camera declarations
Camera  camera( glm::vec3(0.0f, 0.0f, 3.0f) );
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

/// Light attributes
glm::vec3 lightPos( 1.2f, 1.0f, 2.0f );

/// Time definitions (for updates in game loop)
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

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
 *  Main function, inits and runs everything
 */
int main( )
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
                                          "C++ sect: OpenGL (lecture 6)", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create window!" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    // Use callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Use Z-buffer
    glEnable(GL_DEPTH_TEST);


    // Build and compile our shader programs
    Shader lightingShader("Shaders/lighting.vert", "Shaders/lighting.frag");
    Shader lampShader("Shaders/lamp.vert", "Shaders/lamp.frag");

    // Set up vertex data (with buffers) and attribute pointers
    GLfloat vertices[] =
            {
                    // Positions            // Normals              // Texture coords
                    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f,  0.0f,
                     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f,  0.0f,
                     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f,  1.0f,
                     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f,  1.0f,
                    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f,  1.0f,
                    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f,  0.0f,

                    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,
                     0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,
                     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
                     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
                    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f,  1.0f,
                    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,

                    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
                    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
                    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
                    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
                    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
                    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

                     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
                     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
                     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
                     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
                     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
                     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

                    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f,  1.0f,
                     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f,  1.0f,
                     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f,  0.0f,
                     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f,  0.0f,
                    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f,  0.0f,
                    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f,  1.0f,

                    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f,  1.0f,
                     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f,  1.0f,
                     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,
                     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,
                    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f,  0.0f,
                    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f,  1.0f
            };

    // Positions of all containers
    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f,   0.0f,   0.0f),
            glm::vec3(2.0f,   5.0f,  -15.0f),
            glm::vec3(-1.5f, -2.2f,  -2.5f),
            glm::vec3(-3.8f, -2.0f,  -12.3f),
            glm::vec3(2.4f,  -0.4f,  -3.5f),
            glm::vec3(-1.7f,  3.0f,  -7.5f),
            glm::vec3(1.3f,  -2.0f,  -2.5f),
            glm::vec3(1.5f,   2.0f,  -2.5f),
            glm::vec3(1.5f,   0.2f,  -1.5f),
            glm::vec3(-1.3f,  1.0f,  -1.5f)
    };

    // Positions of the point lights
    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f,   0.2f,  2.0f),
            glm::vec3(2.3f,  -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3(0.0f,   0.0f, -3.0f)
    };

    // Set the container's VAO and VBO
    GLuint VBO, boxVAO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(boxVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // Set the light's VAO (VBO stays the same)
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)nullptr);  // Only position attribute
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Load textures
    GLuint diffuseMap, specularMap, emissionMap;
    glGenTextures(1, &diffuseMap);
    glGenTextures(1, &specularMap);
    glGenTextures(1, &emissionMap);

    int imageWidth, imageHeight;
    unsigned char* image;

    // Diffuse map
    image = SOIL_load_image("Images/container2.png",
                            &imageWidth, &imageHeight, nullptr, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    // Specular map
    image = SOIL_load_image("Images/container2_specular.png",
                            &imageWidth, &imageHeight, nullptr, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Set texture units
    lightingShader.use();
    glUniform1i(glGetUniformLocation(lightingShader.getProgram(), "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.getProgram(), "material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.getZoom(),
                                            (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
                                            0.1f, 100.0f);

    // Game (main) loop
    while ( !glfwWindowShouldClose(window) )
    {
        // Set frame time
        auto currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        doMovement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use corresponding shader when setting uniforms/drawing objects
        lightingShader.use();
        GLint viewPosLoc = glGetUniformLocation(lightingShader.getProgram(), "viewPos");
        glUniform3f(viewPosLoc, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "material.shininess"), 32.0f);

        // ==============================
        // Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
        // the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
        // by defining light types as classes and set their values in there.
        // ==============================

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "dirLight.ambient"),   0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "dirLight.diffuse"),   0.4f,  0.4f,  0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "dirLight.specular"),  0.5f,  0.5f,  0.5f);

        // Point light 1
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[0].position"),
                    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[0].quadratic"), 0.032f);

        // Point light 2
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[1].position"),
                    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[1].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[1].quadratic"), 0.032f);

        // Point light 3
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[2].position"),
                    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[2].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[2].quadratic"), 0.032f);

        // Point light 4
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[3].position"),
                    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[3].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "pointLights[3].quadratic"), 0.032f);

        // Spotlight
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "spotLight.position"),
                    camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "spotLight.direction"),
                    camera.getFront().x, camera.getFront().y, camera.getFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.getProgram(), "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "spotLight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.getProgram(), "spotLight.quadratic"), 0.032f);
        glUniform1f( glGetUniformLocation(lightingShader.getProgram(), "spotLight.cutOff"),
                     glm::cos(glm::radians(12.5f)) );
        glUniform1f( glGetUniformLocation(lightingShader.getProgram(), "spotLight.outerCutOff"),
                     glm::cos(glm::radians(15.0f)) );

        // Create camera transformations
        glm::mat4 view(1);
        view = camera.getViewMatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.getProgram(), "model");
        GLint viewLoc  = glGetUniformLocation(lightingShader.getProgram(), "view");
        GLint projLoc  = glGetUniformLocation(lightingShader.getProgram(), "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr(view) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr(projection) );

        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Draw 10 containers with the same VAO and VBO information; only their world space coordinates differ
        glm::mat4 model(1);
        glBindVertexArray(boxVAO);
        for (GLuint i = 0; i < 10; ++i)
        {
            model = glm::mat4(1);
            model = glm::translate( model, cubePositions[i] );
            GLfloat angle = 20.0f * (GLfloat)i;
            model = glm::rotate( model, angle, glm::vec3(1.0f, 0.3f, 0.5f) );
            glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr(model) );

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // Draw the lamp object also binding the appropriate shader
        lampShader.use();

        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(lampShader.getProgram(), "model");
        viewLoc  = glGetUniformLocation(lampShader.getProgram(), "view");
        projLoc  = glGetUniformLocation(lampShader.getProgram(), "projection");

        // Set matrices
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr(view) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr(projection) );
        model = glm::mat4(1);
        model = glm::translate(model, lightPos);
        model = glm::scale( model, glm::vec3(0.2f) );  // Make it a smaller cube
        glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr(model) );

        // Draw the light object (using light's vertex attributes)
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Draw as many light bulbs as point lights
        glBindVertexArray(lightVAO);
        for (auto& pointLightPosition : pointLightPositions)
        {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPosition);
            model = glm::scale( model, glm::vec3(0.2f) );  // Make it a smaller cube
            glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr(model) );
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &boxVAO);
    glDeleteVertexArrays(1, &lightVAO);
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
