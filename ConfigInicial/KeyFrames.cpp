// Practica 12                           Cristina Silva Alarcon
// Fecha de Entrega: 03/05/26            319271108

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Anim variables
float rotBall = 0.0f;
float rotDog = 0.0f;
int   dogAnim = 0;

// --- Partes animables individualmente ---
float FLegL = 0.0f;
float FLegR = 0.0f;
float BLegL = 0.0f;
float BLegR = 0.0f;
float head = 0.0f;
float tail = 0.0f;
float torso = 0.0f;

// KeyFrames
float dogPosX, dogPosY, dogPosZ;

// =====================================================================
// Sistema de múltiples animaciones
// =====================================================================
#define MAX_ANIMS  5    // número máximo de animaciones guardadas
#define MAX_FRAMES 15    // keyframes por animación

int i_max_steps = 190;
int i_curr_steps = 0;

typedef struct _frame {
    // Posición
    float dogPosX, dogPosY, dogPosZ;
    float incX, incY, incZ;
    // Rotación cuerpo
    float rotDog, rotDogInc;
    // Cabeza
    float head, headInc;
    // Cola
    float tail, tailInc;
    // Torso
    float torso, torsoInc;
    // Patas individuales
    float FLegL, FLegLInc;
    float FLegR, FLegRInc;
    float BLegL, BLegLInc;
    float BLegR, BLegRInc;
} FRAME;

FRAME KeyFrame[MAX_ANIMS][MAX_FRAMES];  // [animación][frame]
int   FrameIndex[MAX_ANIMS] = { 0 };      // cuántos frames tiene cada animación
int   currentAnim = 0;                  // animación activa (grabación/reproducción)

bool play = false;
int  playIndex = 0;

// =====================================================================
void saveFrame(void)
{
    int a = currentAnim;
    int f = FrameIndex[a];
    if (f >= MAX_FRAMES) { printf("Anim %d llena (max %d frames)\n", a, MAX_FRAMES); return; }

    printf("Anim %d  ->  frame %d guardado\n", a, f);

    KeyFrame[a][f].dogPosX = dogPosX;
    KeyFrame[a][f].dogPosY = dogPosY;
    KeyFrame[a][f].dogPosZ = dogPosZ;
    KeyFrame[a][f].rotDog = rotDog;
    KeyFrame[a][f].head = head;
    KeyFrame[a][f].tail = tail;
    KeyFrame[a][f].torso = torso;
    KeyFrame[a][f].FLegL = FLegL;
    KeyFrame[a][f].FLegR = FLegR;
    KeyFrame[a][f].BLegL = BLegL;
    KeyFrame[a][f].BLegR = BLegR;

    FrameIndex[a]++;
}

// =====================================================================
void resetElements(void)
{
    int a = currentAnim;
    dogPosX = KeyFrame[a][0].dogPosX;
    dogPosY = KeyFrame[a][0].dogPosY;
    dogPosZ = KeyFrame[a][0].dogPosZ;
    rotDog = KeyFrame[a][0].rotDog;
    head = KeyFrame[a][0].head;
    tail = KeyFrame[a][0].tail;
    torso = KeyFrame[a][0].torso;
    FLegL = KeyFrame[a][0].FLegL;
    FLegR = KeyFrame[a][0].FLegR;
    BLegL = KeyFrame[a][0].BLegL;
    BLegR = KeyFrame[a][0].BLegR;
}

// =====================================================================
void interpolation(void)
{
    int a = currentAnim;
    int n = playIndex;

    KeyFrame[a][n].incX = (KeyFrame[a][n + 1].dogPosX - KeyFrame[a][n].dogPosX) / i_max_steps;
    KeyFrame[a][n].incY = (KeyFrame[a][n + 1].dogPosY - KeyFrame[a][n].dogPosY) / i_max_steps;
    KeyFrame[a][n].incZ = (KeyFrame[a][n + 1].dogPosZ - KeyFrame[a][n].dogPosZ) / i_max_steps;
    KeyFrame[a][n].rotDogInc = (KeyFrame[a][n + 1].rotDog - KeyFrame[a][n].rotDog) / i_max_steps;
    KeyFrame[a][n].headInc = (KeyFrame[a][n + 1].head - KeyFrame[a][n].head) / i_max_steps;
    KeyFrame[a][n].tailInc = (KeyFrame[a][n + 1].tail - KeyFrame[a][n].tail) / i_max_steps;
    KeyFrame[a][n].torsoInc = (KeyFrame[a][n + 1].torso - KeyFrame[a][n].torso) / i_max_steps;
    KeyFrame[a][n].FLegLInc = (KeyFrame[a][n + 1].FLegL - KeyFrame[a][n].FLegL) / i_max_steps;
    KeyFrame[a][n].FLegRInc = (KeyFrame[a][n + 1].FLegR - KeyFrame[a][n].FLegR) / i_max_steps;
    KeyFrame[a][n].BLegLInc = (KeyFrame[a][n + 1].BLegL - KeyFrame[a][n].BLegL) / i_max_steps;
    KeyFrame[a][n].BLegRInc = (KeyFrame[a][n + 1].BLegR - KeyFrame[a][n].BLegR) / i_max_steps;
}

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// =====================================================================
int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
        "Practica 12 - Cristina Silva Alarcon", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // Init KeyFrames (todas las animaciones)
    for (int a = 0; a < MAX_ANIMS; a++)
    {
        FrameIndex[a] = 0;
        for (int f = 0; f < MAX_FRAMES; f++)
        {
            KeyFrame[a][f].dogPosX = KeyFrame[a][f].dogPosY = KeyFrame[a][f].dogPosZ = 0;
            KeyFrame[a][f].incX = KeyFrame[a][f].incY = KeyFrame[a][f].incZ = 0;
            KeyFrame[a][f].rotDog = KeyFrame[a][f].rotDogInc = 0;
            KeyFrame[a][f].head = KeyFrame[a][f].headInc = 0;
            KeyFrame[a][f].tail = KeyFrame[a][f].tailInc = 0;
            KeyFrame[a][f].torso = KeyFrame[a][f].torsoInc = 0;
            KeyFrame[a][f].FLegL = KeyFrame[a][f].FLegLInc = 0;
            KeyFrame[a][f].FLegR = KeyFrame[a][f].FLegRInc = 0;
            KeyFrame[a][f].BLegL = KeyFrame[a][f].BLegLInc = 0;
            KeyFrame[a][f].BLegR = KeyFrame[a][f].BLegRInc = 0;
        }
    }

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // ===================== Game loop =====================
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        Animation();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc,
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
            lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
            glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
            glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        glm::mat4 view = camera.GetViewMatrix();

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1), modelTemp(1);

        // Piso
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        // ---- Body (con torso) ----
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
        model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(torso), glm::vec3(1.0f, 0.0f, 0.0f));
        modelTemp = model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // ---- Head ----
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // ---- Tail ----
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // ---- Front Left Leg ----
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegL), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // ---- Front Right Leg ----
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegR), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // ---- Back Left Leg ----
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(BLegL), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // ---- Back Right Leg ----
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(BLegR), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // ---- Ball ----
        model = glm::mat4(1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        // ---- Lamp ----
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// =====================================================================
void DoMovement()
{
    // --- Movimiento del perro ---
    if (keys[GLFW_KEY_H]) dogPosZ += 0.003f;
    if (keys[GLFW_KEY_Y]) dogPosZ -= 0.003f;
    if (keys[GLFW_KEY_G]) dogPosX -= 0.003f;
    if (keys[GLFW_KEY_J]) dogPosX += 0.003f;

    // --- Rotación cuerpo ---
    if (keys[GLFW_KEY_2]) rotDog += 0.3f;
    if (keys[GLFW_KEY_3]) rotDog -= 0.3f;

    // --- Cabeza ---
    if (keys[GLFW_KEY_4]) head += 0.3f;
    if (keys[GLFW_KEY_5]) head -= 0.3f;

    // --- Cola ---
    if (keys[GLFW_KEY_6]) tail += 0.3f;
    if (keys[GLFW_KEY_7]) tail -= 0.3f;

    // --- Torso (inclinación para sentarse) ---
    if (keys[GLFW_KEY_8]) torso += 0.3f;
    if (keys[GLFW_KEY_9]) torso -= 0.3f;

    // --- Pata delantera izquierda ---
    if (keys[GLFW_KEY_Z]) FLegL += 0.3f;
    if (keys[GLFW_KEY_X]) FLegL -= 0.3f;

    // --- Pata delantera derecha ---
    if (keys[GLFW_KEY_C]) FLegR += 0.3f;
    if (keys[GLFW_KEY_V]) FLegR -= 0.3f;

    // --- Pata trasera izquierda ---
    if (keys[GLFW_KEY_B]) BLegL += 0.3f;
    if (keys[GLFW_KEY_N]) BLegL -= 0.3f;

    // --- Pata trasera derecha ---
    if (keys[GLFW_KEY_M]) BLegR += 0.3f;
    if (keys[GLFW_KEY_P]) BLegR -= 0.3f;

    // --- Cámara ---
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])   camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])   camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])  camera.ProcessKeyboard(RIGHT, deltaTime);

    // --- Luz puntual ---
    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_U]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_I]) pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_O]) pointLightPositions[0].z += 0.01f;
}

// =====================================================================
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)        keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // ---- Seleccionar animación activa con teclado numérico (el de la derecha) ----
    // KP_1 a KP_5  →  animaciones 0 a 4
    if (action == GLFW_PRESS)
    {
        int newAnim = -1;
        if (key == GLFW_KEY_KP_1) newAnim = 0;
        if (key == GLFW_KEY_KP_2) newAnim = 1;
        if (key == GLFW_KEY_KP_3) newAnim = 2;
        if (key == GLFW_KEY_KP_4) newAnim = 3;
        if (key == GLFW_KEY_KP_5) newAnim = 4;

        if (newAnim >= 0)
        {
            play = false;
            currentAnim = newAnim;
            printf(">>> Animacion activa: %d  (frames grabados: %d)\n",
                currentAnim, FrameIndex[currentAnim]);
        }
    }

    // ---- Guardar keyframe en la animación activa ----
    if (keys[GLFW_KEY_K] && FrameIndex[currentAnim] < MAX_FRAMES)
        saveFrame();

    // ---- Reproducir / detener la animación activa ----
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (!play && FrameIndex[currentAnim] > 1)
        {
            resetElements();
            playIndex = 0;
            i_curr_steps = 0;
            interpolation();
            play = true;
            printf(">>> Reproduciendo animacion %d\n", currentAnim);
        }
        else
        {
            play = false;
            printf(">>> Animacion detenida\n");
        }
    }

    // ---- Luz de color dinámica ----
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        active = !active;
        Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0);
    }
}

// =====================================================================
void Animation()
{
    if (!play) return;

    if (i_curr_steps >= i_max_steps)
    {
        playIndex++;
        if (playIndex > FrameIndex[currentAnim] - 2)
        {
            printf(">>> Termina animacion %d\n", currentAnim);
            playIndex = 0;
            play = false;
        }
        else
        {
            i_curr_steps = 0;
            interpolation();
        }
    }
    else
    {
        int a = currentAnim;
        dogPosX += KeyFrame[a][playIndex].incX;
        dogPosY += KeyFrame[a][playIndex].incY;
        dogPosZ += KeyFrame[a][playIndex].incZ;

        rotDog += KeyFrame[a][playIndex].rotDogInc;
        head += KeyFrame[a][playIndex].headInc;
        tail += KeyFrame[a][playIndex].tailInc;
        torso += KeyFrame[a][playIndex].torsoInc;

        FLegL += KeyFrame[a][playIndex].FLegLInc;
        FLegR += KeyFrame[a][playIndex].FLegRInc;
        BLegL += KeyFrame[a][playIndex].BLegLInc;
        BLegR += KeyFrame[a][playIndex].BLegRInc;

        i_curr_steps++;
    }
}

// =====================================================================
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}