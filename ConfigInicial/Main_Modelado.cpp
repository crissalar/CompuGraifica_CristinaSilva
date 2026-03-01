//Práctica 4                                      Cristina Silva Alarcón
// Fecha de Entrega: 1 de marzo de 2026          319271108


#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado geometrico - Cristina Silva Alarcon", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// CAMBIO: Se eliminaron los colores del arreglo de vértices.
	// Antes cada vértice tenía 6 floats (x,y,z + r,g,b).
	// Ahora solo tiene 3 floats (x,y,z) porque el color
	// se manejará por figura desde C++ usando un uniform.
	float vertices[] = {
		-0.5f, -0.5f,  0.5f,  // Front
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,  // Back
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		 0.5f, -0.5f,  0.5f,  // Right
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,  // Left
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,  // Bottom
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,  // Top
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// CAMBIO: Se actualizó el stride de 6*sizeof(GLfloat) a 3*sizeof(GLfloat)
	// porque ahora cada vértice solo tiene posición (x,y,z), sin color.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// CAMBIO: Se eliminó el glVertexAttribPointer del color (location 1)
	// ya que el color ya no viene de los vértices sino del uniform figureColor.

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 projection = glm::mat4(1);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	// CAMBIO: Se obtiene la ubicación del uniform "figureColor" del fragment shader.
	// Esto permite mandar un color distinto antes de dibujar cada figura.
	ourShader.Use();
	GLint colorLoc = glGetUniformLocation(ourShader.Program, "figureColor");

	while (!glfwWindowShouldClose(window))
	{
		Inputs(window);
		glfwPollEvents();

		glClearColor(0.529f, 0.839f, 0.820f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);

		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);

		// -------------------------------------------------------
		// COLORES DISPONIBLES (hex -> normalizado 0.0-1.0):
		// Crema claro  #EBE7C1 -> (0.922f, 0.906f, 0.757f)
		// Café claro   #CC9C5E -> (0.800f, 0.612f, 0.369f)
		// Marrón       #754539 -> (0.459f, 0.271f, 0.224f)
		// Amarillo     #F3FF21 -> (0.953f, 1.000f, 0.129f)
		// Rojo claro   #FF4124 -> (1.000f, 0.255f, 0.141f)
		// Rojo fuerte  #9C2714 -> (0.612f, 0.153f, 0.078f)
		// -------------------------------------------------------

		////////////////////////////////////////////////////////
		//  CAPA 1
		///////////////////////////////////////////////////////

		// Figura 1 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 2 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.35f, 0.55f, -0.1f));
		model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 3 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.35f, 0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.3f, 0.3f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 4 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.45f, 0.45f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 5 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.45f, 0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 6 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.45f, 0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 7 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.3f, 0.1f, -0.1f));
		model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 8 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f, -0.05f, -0.1f));
		model = glm::scale(model, glm::vec3(0.4f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 9 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.1f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 10 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.15f, -0.05f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 11 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.05f, -0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 12 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.15f, 0.0f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 13 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.3f, 0.1f, -0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 14 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, 0.2f, -0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 15 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.45f, 0.0f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.3f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 16 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.35f, -0.1f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.3f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 17 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, -0.05f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 18 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.15f, -0.1f));
		model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 19 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.35f, -0.15f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 20 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.45f, -0.15f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 21 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.55f, -0.15f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 22 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.1f, -0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.4f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 23 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, -0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 24 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.35f, -0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 25 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.45f, -0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 26 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.55f, -0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 27 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.15f, -0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 28 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.15f, -0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.1f, 0.3f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 29 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.15f, -0.45f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 30 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, -0.45f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 31 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.15f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 32 - Amarillo #F3FF21
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.65f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.953f, 1.000f, 0.129f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////
		//  CAPA 2
		///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////

		// Figura 33 - Rojo fuerte #9C2714
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.15f, 0.75f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.612f, 0.153f, 0.078f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 34 - Rojo claro #FF4124
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.3f, 0.75f, -0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 1.000f, 0.255f, 0.141f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 35 - Rojo claro #FF4124
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.35f, 0.65f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 1.000f, 0.255f, 0.141f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 36 - Rojo fuerte #9C2714
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.45f, 0.65f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.612f, 0.153f, 0.078f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 37 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.15f, 0.55f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 38 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.55f, 0.5f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 39 - Amarillo #F3FF21
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.6f, 0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.953f, 1.000f, 0.129f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 40 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.05f, 0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 41 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.05f, 0.1f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 42 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.15f, 0.15f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 43 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.35f, 0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 44 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.55f, 0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 45 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, 0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 46 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, 0.15f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 47 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.55f, 0.05f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 48 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, -0.05f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 49 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.55f, -0.05f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 50 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.55f, -0.2f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 51 - Crema claro #EBE7C1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.45f, -0.15f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.922f, 0.906f, 0.757f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 52 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.45f, -0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 53 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.35f, -0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 54 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, -0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 55 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, 0.05f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 56 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.25f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 57 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.45f, -0.35f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////
		//  EXCLUSIVAS CAPA 3
		///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////

		// Figura 58 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.25f, -0.45f, -0.2f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 59 - Marrón #754539
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, -0.45f, -0.2f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.459f, 0.271f, 0.224f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 60 - Café claro #CC9C5E
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.15f, -0.6f, -0.2f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniform3f(colorLoc, 0.800f, 0.612f, 0.369f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Figura 61 - Amarillo #F3FF21
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.3f, -0.65f, -0.2f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.1f));
		glUniform3f(colorLoc, 0.953f, 1.000f, 0.129f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

glBindVertexArray(0);

glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.4f;
}