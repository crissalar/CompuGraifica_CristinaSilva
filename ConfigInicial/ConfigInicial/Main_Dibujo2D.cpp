// Cristina Silva Alarcón

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D - Cristina Silva Alarcon", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    float vertices[] = {
        // Posición (x, y, z)      Color (r, g, b)

        // TRIÁNGULO 1 - pico
        -0.7f,  0.1f, 0.0f,       0.2f, 0.8f, 0.6f,  // 0
        -0.55f,  0.35f, 0.0f,     0.2f, 0.8f, 0.6f,  // 1
        -0.5f,  0.3f, 0.0f,       0.2f, 0.8f, 0.6f,  // 2

        // TRIÁNGULO 2 - cabeza baja
        -0.55f, 0.35f, 0.0f,      0.2f, 0.8f, 0.6f,  // 3
        -0.5f, 0.3f, 0.0f,        0.2f, 0.8f, 0.6f,  // 4
        -0.4f, 0.4f, 0.0f,        0.2f, 0.8f, 0.6f,  // 5

        // TRIÁNGULO 3 - cabeza media
        -0.55f, 0.35f, 0.0f,         0.2f, 0.8f, 0.6f,  // 6
        -0.55f, 0.45f, 0.0f,         0.2f, 0.8f, 0.6f,  // 7
        -0.4f, 0.4f, 0.0f,         0.2f, 0.8f, 0.6f,  // 8

        // TRIÁNGULO 4 - cabeza alta
        -0.55f, 0.45f, 0.0f,         0.2f, 0.8f, 0.6f,  // 9
        -0.5f, 0.5f, 0.0f,         0.2f, 0.8f, 0.6f,  // 10
        -0.4f, 0.4f, 0.0f,         0.2f, 0.8f, 0.6f,  // 11

        // TRIÁNGULO 5 - pecho
        -0.4f, 0.1f, 0.0f,         0.902f, 0.847f, 0.263f,  // 12
        -0.4f, 0.4f, 0.0f,         0.902f, 0.847f, 0.263f,  // 13
        0.2f, -0.2f, 0.0f,         0.902f, 0.847f, 0.263f,  // 14

        // TRIÁNGULO 6 - espalda alta
        -0.5f, 0.5f, 0.0f,         0.2f, 0.8f, 0.6f,  // 15
        -0.2f, 0.2f, 0.0f,         0.2f, 0.8f, 0.6f,  // 16
        -0.2f, 0.5f, 0.0f,         0.2f, 0.8f, 0.6f,  // 17

        // TRIÁNGULO 7 - espalda media
        -0.2f, 0.2f, 0.0f,         0.2f, 0.8f, 0.6f,  // 18
        -0.2f, 0.5f, 0.0f,         0.2f, 0.8f, 0.6f,  // 19
        0.1f, 0.2f, 0.0f,         0.2f, 0.8f, 0.6f,  // 20

        // TRIÁNGULO 8 - espalda baja
        -0.2f, 0.2f, 0.0f,         0.2f, 0.8f, 0.6f,  // 21
        0.1f, 0.2f, 0.0f,         0.2f, 0.8f, 0.6f,  // 22
        0.2f, -0.2f, 0.0f,         0.2f, 0.8f, 0.6f,  // 23

        // TRIÁNGULO 9 - ala atrás
        -0.2f, 0.5f, 0.0f,         0.2f, 0.8f, 0.6f,  // 24
        0.1f, 0.2f, 0.0f,         0.2f, 0.8f, 0.6f,  // 25
        0.4f, 0.8f, 0.0f,         0.2f, 0.8f, 0.6f,  // 26

        // TRIÁNGULO 10 - ala frente
        -0.2f, 0.5f, 0.0f,         0.2f, 0.8f, 0.6f,  // 27
        0.1f, 0.2f, 0.0f,         0.2f, 0.8f, 0.6f,  // 28
        0.6f, 0.7f, 0.0f,         0.2f, 0.8f, 0.6f,  // 29

        // TRIÁNGULO 11 - cola alta
        0.1f, -0.1f, 0.0f,         0.2f, 0.8f, 0.6f,  // 30
        0.3f, -0.2f, 0.0f,         0.2f, 0.8f, 0.6f,  // 31
        0.3f, 0.1f, 0.0f,         0.2f, 0.8f, 0.6f,  // 32

        // TRIÁNGULO 12 - cola 2
        0.1f, -0.1f, 0.0f,         0.2f, 0.8f, 0.6f,  // 33
        0.3f, -0.4f, 0.0f,         0.2f, 0.8f, 0.6f,  // 34
        0.4f, -0.1f, 0.0f,         0.2f, 0.8f, 0.6f,  // 35
    };

    unsigned int indices[] = {
        0, 1, 2,        // Triángulo 1
        3, 4, 5,        // Triángulo 2
        6, 7, 8,        // Triángulo 3
        9, 10, 11,      // Triángulo 4
        12, 13, 14,     // Triángulo 5
        15, 16, 17,     // Triángulo 6
        18, 19, 20,     // Triángulo 7
        21, 22, 23,     // Triángulo 8
        24, 25, 26,     // Triángulo 9
        27, 28, 29,     // Triángulo 10
        30, 31, 32,     // Triángulo 11
        33, 34, 35,     // Triángulo 12
    };



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);


        //glPointSize(10);
        //glDrawArrays(GL_POINTS,0,4);
        
        //glDrawArrays(GL_LINES,4,2);
        //glDrawArrays(GL_LINE_LOOP,0,4);
        
        //glDrawArrays(GL_TRIANGLES,0,3);
        //glDrawElements(GL_TRIANGLES, 3,GL_UNSIGNED_INT,0);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}