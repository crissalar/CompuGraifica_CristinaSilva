// Previo 12                           Cristina Silva Alarcon
// Fecha de Entrega: 27/04/26          319271108


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
float FLegL = 0.0f;   // Pata delantera izquierda 
float FLegR = 0.0f;   // Pata delantera derecha 
float BLegL = 0.0f;   // Pata trasera izquierda  
float BLegR = 0.0f;   // Pata trasera derecha  
float head = 0.0f;   // Cabeza
float tail = 0.0f;   // Cola
float torso = 0.0f;   // Inclinación del torso (eje X, para sentarse)

// KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 9
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

FRAME KeyFrame[MAX_FRAMES];
int  FrameIndex = 0;
bool play = false;
int  playIndex = 0;

void saveFrame(void)
{
    printf("frameindex %d\n", FrameIndex);

    KeyFrame[FrameIndex].dogPosX = dogPosX;
    KeyFrame[FrameIndex].dogPosY = dogPosY;
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;

    KeyFrame[FrameIndex].rotDog = rotDog;
    KeyFrame[FrameIndex].head = head;
    KeyFrame[FrameIndex].tail = tail;
    KeyFrame[FrameIndex].torso = torso;

    KeyFrame[FrameIndex].FLegL = FLegL;
    KeyFrame[FrameIndex].FLegR = FLegR;
    KeyFrame[FrameIndex].BLegL = BLegL;
    KeyFrame[FrameIndex].BLegR = BLegR;

    FrameIndex++;
}

void resetElements(void)
{
    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;

    rotDog = KeyFrame[0].rotDog;
    head = KeyFrame[0].head;
    tail = KeyFrame[0].tail;
    torso = KeyFrame[0].torso;

    FLegL = KeyFrame[0].FLegL;
    FLegR = KeyFrame[0].FLegR;
    BLegL = KeyFrame[0].BLegL;
    BLegR = KeyFrame[0].BLegR;
}

void interpolation(void)
{
    int n = playIndex;

    KeyFrame[n].incX = (KeyFrame[n + 1].dogPosX - KeyFrame[n].dogPosX) / i_max_steps;
    KeyFrame[n].incY = (KeyFrame[n + 1].dogPosY - KeyFrame[n].dogPosY) / i_max_steps;
    KeyFrame[n].incZ = (KeyFrame[n + 1].dogPosZ - KeyFrame[n].dogPosZ) / i_max_steps;

    KeyFrame[n].rotDogInc = (KeyFrame[n + 1].rotDog - KeyFrame[n].rotDog) / i_max_steps;
    KeyFrame[n].headInc = (KeyFrame[n + 1].head - KeyFrame[n].head) / i_max_steps;
    KeyFrame[n].tailInc = (KeyFrame[n + 1].tail - KeyFrame[n].tail) / i_max_steps;
    KeyFrame[n].torsoInc = (KeyFrame[n + 1].torso - KeyFrame[n].torso) / i_max_steps;

    KeyFrame[n].FLegLInc = (KeyFrame[n + 1].FLegL - KeyFrame[n].FLegL) / i_max_steps;
    KeyFrame[n].FLegRInc = (KeyFrame[n + 1].FLegR - KeyFrame[n].FLegR) / i_max_steps;
    KeyFrame[n].BLegLInc = (KeyFrame[n + 1].BLegL - KeyFrame[n].BLegL) / i_max_steps;
    KeyFrame[n].BLegRInc = (KeyFrame[n + 1].BLegR - KeyFrame[n].BLegR) / i_max_steps;
}

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
        "Animacion maquina de estados - Cristina Silva Alarcon", nullptr, nullptr);

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

    // Init KeyFrames
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        KeyFrame[i].dogPosX = KeyFrame[i].dogPosY = KeyFrame[i].dogPosZ = 0;
        KeyFrame[i].incX = KeyFrame[i].incY = KeyFrame[i].incZ = 0;
        KeyFrame[i].rotDog = KeyFrame[i].rotDogInc = 0;
        KeyFrame[i].head = KeyFrame[i].headInc = 0;
        KeyFrame[i].tail = KeyFrame[i].tailInc = 0;
        KeyFrame[i].torso = KeyFrame[i].torsoInc = 0;
        KeyFrame[i].FLegL = KeyFrame[i].FLegLInc = 0;
        KeyFrame[i].FLegR = KeyFrame[i].FLegRInc = 0;
        KeyFrame[i].BLegL = KeyFrame[i].BLegLInc = 0;
        KeyFrame[i].BLegR = KeyFrame[i].BLegRInc = 0;
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
        model = glm::rotate(model, glm::radians(torso), glm::vec3(1.0f, 0.0f, 0.0f)); // inclinación torso
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

    // --- Pata delantera izquierda (individual) ---
    if (keys[GLFW_KEY_Z]) FLegL += 0.3f;
    if (keys[GLFW_KEY_X]) FLegL -= 0.3f;

    // --- Pata delantera derecha (individual) ---
    if (keys[GLFW_KEY_C]) FLegR += 0.3f;
    if (keys[GLFW_KEY_V]) FLegR -= 0.3f;

    // --- Pata trasera izquierda (individual) ---
    if (keys[GLFW_KEY_B]) BLegL += 0.3f;
    if (keys[GLFW_KEY_N]) BLegL -= 0.3f;

    // --- Pata trasera derecha (individual) ---
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
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // Guardar keyframe
    if (keys[GLFW_KEY_K] && FrameIndex < MAX_FRAMES)
        saveFrame();

    // Reproducir / detener animación
    if (keys[GLFW_KEY_L])
    {
        if (!play && FrameIndex > 1)
        {
            resetElements();
            interpolation();
            play = true;
            playIndex = 0;
            i_curr_steps = 0;
        }
        else
        {
            play = false;
        }
    }

    // Luz de color dinámica
    if (keys[GLFW_KEY_SPACE])
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
        if (playIndex > FrameIndex - 2)
        {
            printf("termina anim\n");
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
        dogPosX += KeyFrame[playIndex].incX;
        dogPosY += KeyFrame[playIndex].incY;
        dogPosZ += KeyFrame[playIndex].incZ;

        rotDog += KeyFrame[playIndex].rotDogInc;
        head += KeyFrame[playIndex].headInc;
        tail += KeyFrame[playIndex].tailInc;
        torso += KeyFrame[playIndex].torsoInc;

        FLegL += KeyFrame[playIndex].FLegLInc;
        FLegR += KeyFrame[playIndex].FLegRInc;
        BLegL += KeyFrame[playIndex].BLegLInc;
        BLegR += KeyFrame[playIndex].BLegRInc;

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