// Practica 8                              Silva Alarcón Cristina
// Fecha de Entrega: 29/03/26              319271108

#include <string>
#include <vector>
#include <windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL2/SOIL2.h"
#include "stb_image.h"

const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

Camera camera(glm::vec3(0.0f, 1.5f, 6.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Rotación del sol
bool rotateSun = false;
float sunAngle = 0.0f;

// Esferas sol/luna
GLuint sphereVAO, sphereVBO, sphereEBO;
int sphereIndexCount = 0;

void generateSphere(float radius, int sectors, int stacks)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= stacks; ++i)
    {
        float stackAngle = glm::pi<float>() / 2 - i * (glm::pi<float>() / stacks);
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * (2 * glm::pi<float>() / sectors);
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
            vertices.push_back(x / radius); vertices.push_back(y / radius); vertices.push_back(z / radius);
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;
        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            if (i != 0) { indices.push_back(k1); indices.push_back(k2); indices.push_back(k1 + 1); }
            if (i != stacks - 1) { indices.push_back(k1 + 1); indices.push_back(k2); indices.push_back(k2 + 1); }
        }
    }

    sphereIndexCount = (int)indices.size();

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

int main()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
        "Escenario con sol y luna - Cristina Silva Alarcon", nullptr, nullptr);
    if (!window) { glfwTerminate(); return EXIT_FAILURE; }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return EXIT_FAILURE;

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Escenario
    Model scenary((char*)"Models/modelo_final/final_scenary.obj");

    // Generar esfera (radio 0.4, 36 sectores, 18 stacks)
    generateSphere(0.4f, 36, 18);

    glm::mat4 projection = glm::perspective(
        camera.GetZoom(),
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
        0.1f, 200.0f
    );

    while (!glfwWindowShouldClose(window))
    {
        // --- Tiempo ---
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // --- Rotación del sol ---
        if (rotateSun)
        {
            sunAngle += deltaTime * 0.4f;
            if (sunAngle > 2.0f * glm::pi<float>())
                sunAngle -= 2.0f * glm::pi<float>();
        }

        // --- Dirección de la luz (sol y luna opuestos) ---
        glm::vec3 sunDir(
            sin(sunAngle),
            -cos(sunAngle),
            0.2f
        );
        sunDir = glm::normalize(sunDir);
        glm::vec3 moonDir = -sunDir;

        // --- Posición visual de las esferas ---
        float orbitRadius = 50.0f;
        glm::vec3 sunPos = -sunDir * orbitRadius;
        glm::vec3 moonPos = -moonDir * orbitRadius;

        // --- Limpiar buffer ---
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // ======================================================
        // ESCENARIO — lightingShader
        // ======================================================
        lightingShader.Use();

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"),
            1, GL_FALSE, glm::value_ptr(view));

        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Luz 1: Sol (cálido)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.direction"),
            sunDir.x, sunDir.y, sunDir.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.15f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.9f, 0.75f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 0.9f, 0.6f);

        // Luz 2: Luna (frío)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.direction"),
            moonDir.x, moonDir.y, moonDir.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.0f, 0.03f, 0.20f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.15f, 0.80f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.2f, 1.0f);

        // Material del escenario
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        scenary.Draw(lightingShader);

        // ======================================================
        // ESFERAS SOL Y LUNA — lampShader
        // ======================================================
        lampShader.Use();

        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"),
            1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(sphereVAO);

        // Sol — amarillo cálido
        model = glm::mat4(1.0f);
        model = glm::translate(model, sunPos);
		model = glm::scale(model, glm::vec3(7.0f)); // <-- escala uniforme, aquí se ajusta el valor para que el sol sea más grande que la luna
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 0.9f, 0.2f);
        glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

        // Luna — azul frío
        model = glm::mat4(1.0f);
        model = glm::translate(model, moonPos);
        model = glm::scale(model, glm::vec3(5.4f)); // <-- luna un poco más pequeña que el sol
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 0.6f, 0.75f, 1.0f);
        glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // ======================================================
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &sphereEBO);
    glfwTerminate();
    return 0;
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])   camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])   camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])  camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // R = activar/detener rotación del sol (y la luna se mueve opuesta)
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        rotateSun = !rotateSun;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos; lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}