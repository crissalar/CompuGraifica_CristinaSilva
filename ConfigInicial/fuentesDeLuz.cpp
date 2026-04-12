// Practica 9                          Cristina Silva Alarcon
// Fecha de Entrega: 12/04/26          319271108

#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL2/SOIL2.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

Camera camera(glm::vec3(0.0f, 1.5f, 6.0f));
GLfloat lastX = 400, lastY = 300;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// ─────────────────────────────────────────────────────────────────────────────
// POSICIÓN Y ESCALA DE LAS TRES LÁMPARAS
// ─────────────────────────────────────────────────────────────────────────────
// x /z/y
// Lámpara japonesa
glm::vec3 posJapan = glm::vec3(-30.0f, 0.0f, 0.0f);
glm::vec3 scaleJapan = glm::vec3(2.5f);
glm::vec3 lightJapan = glm::vec3(-30.0f, 15.0f, 0.0f);  // 

// Lámpara de escritorio (metal)
glm::vec3 posMetal = glm::vec3(0.0f, 20.5f, -1.0f);
glm::vec3 scaleMetal = glm::vec3(0.5f);
glm::vec3 lightMetal = glm::vec3(-4.0f, 24.7f, 0.5f);  // 
glm::vec3 spotDir = glm::normalize(glm::vec3(0.0f, -3.0f, 1.5f));

// Lámpara de lava
glm::vec3 posLava = glm::vec3(36.0f, 32.0f, -10.0f);
glm::vec3 scaleLava = glm::vec3(1.5f);
glm::vec3 lightLava = glm::vec3(35.0f, 35.0f, -5.0f); // 

// ─────────────────────────────────────────────────────────────────────────────

float vertices[] = {
    -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,

    -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,

     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,

    -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f
};

bool active = false;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
        "Fuentes de luz - Cristina Silva Alarcon", nullptr, nullptr);
    if (!window) { glfwTerminate(); return EXIT_FAILURE; }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) { std::cout << "Failed to initialize GLEW\n"; return EXIT_FAILURE; }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // ── Shaders ───────────────────────────────────────────────────────────────
    // Usa el lighting.frag del Previo 9 (soporta dirLight, pointLights[], spotLight)
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // ── Escenario ─────────────────────────────────────────────────────────────
    Model scenary((char*)"Models/modelo_final/final_scenary.obj");

    // ── Tres lámparas (fuentes de luz) ────────────────────────────────────────
    Model LamparaJapan((char*)"Models/lamps/japan_lamp.obj");
    Model LamparaMetal((char*)"Models/lamps/lampara_metal.obj");
    Model LamparaLava((char*)"Models/lamps/lampara_lava.obj");

    // ── VAO/VBO (cubos indicadores de posición de luz) ─────────────────────────
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

    glm::mat4 projection = glm::perspective(
        camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
        0.1f, 200.0f
    );

    // ─────────────────────────────────────────────────────────────────────────
    // GAME LOOP
    // ─────────────────────────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // =====================================================================
        // ESCENARIO + LÁMPARAS  ← lightingShader
        // =====================================================================
        lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));

        // ── Luz direccional global ────────────────────────────────────────────
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Point light 0 — japonesa
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            lightJapan.x, lightJapan.y, lightJapan.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.2f, 0.05f, 0.01f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.2f, 0.6f, 0.08f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.5f, 0.5f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.005f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0001f);

        // ── Point light 1: LÁMPARA DE LAVA  →  verde ─────────────────────────
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"),
            lightLava.x, lightLava.y, lightLava.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.0f, 0.12f, 0.01f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.05f, 0.85f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.8f, 0.1f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

        // ── Point lights 2 y 3: apagadas ─────────────────────────────────────
        for (int i = 2; i <= 3; i++)
        {
            char buf[64];
            sprintf_s(buf, "pointLights[%d].position", i); glUniform3f(glGetUniformLocation(lightingShader.Program, buf), 0, 0, 0);
            sprintf_s(buf, "pointLights[%d].ambient", i); glUniform3f(glGetUniformLocation(lightingShader.Program, buf), 0, 0, 0);
            sprintf_s(buf, "pointLights[%d].diffuse", i); glUniform3f(glGetUniformLocation(lightingShader.Program, buf), 0, 0, 0);
            sprintf_s(buf, "pointLights[%d].specular", i); glUniform3f(glGetUniformLocation(lightingShader.Program, buf), 0, 0, 0);
            sprintf_s(buf, "pointLights[%d].constant", i); glUniform1f(glGetUniformLocation(lightingShader.Program, buf), 1.0f);
            sprintf_s(buf, "pointLights[%d].linear", i); glUniform1f(glGetUniformLocation(lightingShader.Program, buf), 0.0f);
            sprintf_s(buf, "pointLights[%d].quadratic", i); glUniform1f(glGetUniformLocation(lightingShader.Program, buf), 0.0f);
        }

        // ── SpotLight: LÁMPARA DE ESCRITORIO  →  blanco ───────────────────────
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            lightMetal.x, lightMetal.y, lightMetal.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            spotDir.x, spotDir.y, spotDir.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 5.0f, 5.0f, 5.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 5.0f, 5.0f, 5.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.014f);  // <── menos atenuación
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0007f); // <── menos atenuación
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
            glm::cos(glm::radians(25.0f)));   // <── más abierto
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
            glm::cos(glm::radians(35.0f)));   // <── borde más suave

        // ── Material ──────────────────────────────────────────────────────────
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");

        // ── Escenario ─────────────────────────────────────────────────────────
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        scenary.Draw(lightingShader);

        // ── LÁMPARA JAPONESA ──────────────────────────────────────────────────
        model = glm::mat4(1.0f);
        model = glm::translate(model, posJapan);
        model = glm::scale(model, scaleJapan);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        LamparaJapan.Draw(lightingShader);

        // ── LÁMPARA DE LAVA ───────────────────────────────────────────────────
        model = glm::mat4(1.0f);
        model = glm::translate(model, posLava);
        model = glm::scale(model, scaleLava);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        LamparaLava.Draw(lightingShader);

        // ── LÁMPARA DE ESCRITORIO ─────────────────────────────────────────────
        model = glm::mat4(1.0f);
        model = glm::translate(model, posMetal);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, scaleMetal);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        LamparaMetal.Draw(lightingShader);

        // =====================================================================
        // CUBITOS INDICADORES DE LUZ  ← lampShader
        // =====================================================================
        lampShader.Use();
        GLint modelLocL = glGetUniformLocation(lampShader.Program, "model");
        GLint viewLocL = glGetUniformLocation(lampShader.Program, "view");
        GLint projLocL = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLocL, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLocL, 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(VAO);

        // Indicador japonesa (rojo-amarillo)
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightJapan);
        model = glm::scale(model, glm::vec3(0.1f));
        glUniformMatrix4fv(modelLocL, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 0.9f, 0.3f, 0.05f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Indicador lava (verde)
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightLava);
        model = glm::scale(model, glm::vec3(0.1f));
        glUniformMatrix4fv(modelLocL, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 0.05f, 0.9f, 0.1f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Indicador escritorio (blanco)
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightMetal);
        model = glm::scale(model, glm::vec3(0.1f));
        glUniformMatrix4fv(modelLocL, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])   camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])   camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])  camera.ProcessKeyboard(RIGHT, deltaTime);

    // Mover la lámpara japonesa con T/G/Y/H/U/J
    if (keys[GLFW_KEY_T]) posJapan.x += 0.01f;
    if (keys[GLFW_KEY_G]) posJapan.x -= 0.01f;
    if (keys[GLFW_KEY_Y]) posJapan.y += 0.01f;
    if (keys[GLFW_KEY_H]) posJapan.y -= 0.01f;
    if (keys[GLFW_KEY_U]) posJapan.z -= 0.01f;
    if (keys[GLFW_KEY_J]) posJapan.z += 0.01f;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos; lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}