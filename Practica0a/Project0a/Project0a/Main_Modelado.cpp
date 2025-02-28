#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Agregar 1/5
#include <vector>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=0.0f;
float movZ=-5.0f;
float rot = 0.0f;
float rotx = 0.0f;
float rotz= 0.0f;

int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado geometrico", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	 //float vertices[] = {
		// // Positions          // Colors (Gradients mejorados)
		//  // Front (Gradiente de rojo a naranja)
		//  -0.1f, -0.1f,  0.1f,  1.0f, 0.2f, 0.2f, // Rojo oscuro
		//   0.1f, -0.1f,  0.1f,  1.0f, 0.5f, 0.2f, // Naranja oscuro
		//   0.1f,  0.1f,  0.1f,  1.0f, 0.8f, 0.2f, // Naranja claro
		//   0.1f,  0.1f,  0.1f,  1.0f, 0.8f, 0.2f,
		//  -0.1f,  0.1f,  0.1f,  1.0f, 0.5f, 0.2f,
		//  -0.1f, -0.1f,  0.1f,  1.0f, 0.2f, 0.2f,

		//  // Back (Gradiente de azul a morado)
		//  -0.1f, -0.1f, -0.1f,  0.2f, 0.2f, 1.0f, // Azul oscuro
		//   0.1f, -0.1f, -0.1f,  0.5f, 0.2f, 1.0f, // Morado oscuro
		//   0.1f,  0.1f, -0.1f,  0.8f, 0.2f, 1.0f, // Morado claro
		//   0.1f,  0.1f, -0.1f,  0.8f, 0.2f, 1.0f,
		//  -0.1f,  0.1f, -0.1f,  0.5f, 0.2f, 1.0f,
		//  -0.1f, -0.1f, -0.1f,  0.2f, 0.2f, 1.0f,

		//  // Right (Gradiente de verde a amarillo)
		//   0.1f, -0.1f,  0.1f,  0.2f, 1.0f, 0.2f, // Verde oscuro
		//   0.1f, -0.1f, -0.1f,  0.5f, 1.0f, 0.2f, // Verde amarillento
		//   0.1f,  0.1f, -0.1f,  0.8f, 1.0f, 0.2f, // Amarillo verdoso
		//   0.1f,  0.1f, -0.1f,  0.8f, 1.0f, 0.2f,
		//   0.1f,  0.1f,  0.1f,  0.5f, 1.0f, 0.2f,
		//   0.1f, -0.1f,  0.1f,  0.2f, 1.0f, 0.2f,

		//   // Left (Gradiente de morado a rosa)
		//   -0.1f,  0.1f,  0.1f,  1.0f, 0.2f, 1.0f, // Morado oscuro
		//   -0.1f,  0.1f, -0.1f,  1.0f, 0.5f, 1.0f, // Rosa oscuro
		//   -0.1f, -0.1f, -0.1f,  1.0f, 0.8f, 1.0f, // Rosa claro
		//   -0.1f, -0.1f, -0.1f,  1.0f, 0.8f, 1.0f,
		//   -0.1f, -0.1f,  0.1f,  1.0f, 0.5f, 1.0f,
		//   -0.1f,  0.1f,  0.1f,  1.0f, 0.2f, 1.0f,

		//   // Bottom (Gradiente de amarillo a blanco)
		//   -0.1f, -0.1f, -0.1f,  1.0f, 1.0f, 0.2f, // Amarillo oscuro
		//	0.1f, -0.1f, -0.1f,  1.0f, 1.0f, 0.5f, // Amarillo claro
		//	0.1f, -0.1f,  0.1f,  1.0f, 1.0f, 0.8f, // Blanco amarillento
		//	0.1f, -0.1f,  0.1f,  1.0f, 1.0f, 0.8f,
		//   -0.1f, -0.1f,  0.1f,  1.0f, 1.0f, 0.5f,
		//   -0.1f, -0.1f, -0.1f,  1.0f, 1.0f, 0.2f,

		//   // Top (Gradiente de cian a azul claro)
		//   -0.1f,  0.1f, -0.1f,  0.2f, 1.0f, 1.0f, // Cian oscuro
		//	0.1f,  0.1f, -0.1f,  0.5f, 1.0f, 1.0f, // Cian claro
		//	0.1f,  0.1f,  0.1f,  0.8f, 1.0f, 1.0f, // Azul claro
		//	0.1f,  0.1f,  0.1f,  0.8f, 1.0f, 1.0f,
		//   -0.1f,  0.1f,  0.1f,  0.5f, 1.0f, 1.0f,
		//   -0.1f,  0.1f, -0.1f,  0.2f, 1.0f, 1.0f,
  //  };
	 //
	// 
	// 
	 float vertices[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
		 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,
      
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	};


	 // Agregar 2/5
	//------------------------------------------------INICIO RETICULA Y EJES.--------------
	// 
	
	float axisVertices[] = {
		// Ejes coordenados
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Origen a X (rojo)
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Origen a Y (azul)
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Origen a Z (verde)
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	};


	// Crear un VAO y un VBO para los ejes
	GLuint axisVBO, axisVAO;
	glGenVertexArrays(1, &axisVAO);
	glGenBuffers(1, &axisVBO);

	// Vincular el VAO y el VBO para los ejes
	glBindVertexArray(axisVAO);
	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

	// Configurar el atributo de posición para los ejes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Configurar el atributo de color para los ejes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Desvincular el VBO y el VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Inicio de la retícula--------------
	// Definir los vértices de la retícula (una cuadrícula en el plano XZ)
	std::vector<float> gridVertices;
	float gridSize = 1.0f;       // Tamaño de la retícula (de -1 a 1 en X y Z)
	float gridStep = 0.0999f;    // Separación entre líneas
	float gridColor = 0.7f;      // Color gris para la retícula

	// Líneas paralelas al eje X
	for (float z = -gridSize; z <= gridSize; z += gridStep) {
		gridVertices.push_back(-gridSize); gridVertices.push_back(0.0f); gridVertices.push_back(z);
		gridVertices.push_back(gridColor); gridVertices.push_back(gridColor); gridVertices.push_back(gridColor);
		gridVertices.push_back(gridSize); gridVertices.push_back(0.0f); gridVertices.push_back(z);
		gridVertices.push_back(gridColor); gridVertices.push_back(gridColor); gridVertices.push_back(gridColor);
	}

	// Líneas paralelas al eje Z
	for (float x = -gridSize; x <= gridSize; x += gridStep) {
		gridVertices.push_back(x); gridVertices.push_back(0.0f); gridVertices.push_back(-gridSize);
		gridVertices.push_back(gridColor); gridVertices.push_back(gridColor); gridVertices.push_back(gridColor);
		gridVertices.push_back(x); gridVertices.push_back(0.0f); gridVertices.push_back(gridSize);
		gridVertices.push_back(gridColor); gridVertices.push_back(gridColor); gridVertices.push_back(gridColor);
	}

	// Crear un VAO y un VBO para la retícula
	GLuint gridVBO, gridVAO;
	glGenVertexArrays(1, &gridVAO);
	glGenBuffers(1, &gridVBO);

	// Vincular el VAO y el VBO para la retícula
	glBindVertexArray(gridVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

	// Configurar el atributo de posición para la retícula
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Configurar el atributo de color para la retícula
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Desvincular el VBO y el VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//------------------------------------------------------- Final de la retícula Y EJES--------------

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Balanced grayish-black


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 view=glm::mat4(1);
		glm::mat4 model=glm::mat4(1);
	
		// Agregar 3/4
		//VISTA PARA PODER ENTENDER RETICULA Y EJES.-----------------
		//Por alguna razón desconocida, esto conviene hacerlo en este orden. 
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f)); // Mover la cámara hacia atrás en el eje Z
		//view = glm::rotate(view, (float)glfwGetTime() * glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar el cubo
		// FIN VISTA para entender.-----------------------------------

		view = glm::translate(view, glm::vec3(movX/10.0f,movY /1.0f, movZ/10.0f-2.0f));
		view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotar la cámara 30 grados hacia arriba
		view = glm::rotate(view, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar la cámara 45 grados a la derecha
		//
		//view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		//view = glm::translate(view, glm::vec3(movX/10.f, movY/10.f, movZ/10.f-2.0f));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::rotate(view, glm::radians(rotx), glm::vec3(1.0f, 0.0f, 0.0f));
		//view = glm::rotate(view, glm::radians(rotz), glm::vec3(0.0f, 0.0f, 1.0f));


	/*	view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));*/



		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO);
	
	    model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 0.05f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//DIBJUAR PATAS:

	

		glm::mat4 pata1 = glm::mat4(1.0f);
		pata1 = glm::scale(pata1, glm::vec3(0.1f, 0.5f, 0.1f));
		
		pata1 = glm::translate(pata1, glm::vec3(7.0f, 0.0f, 4.5f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pata1));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);



		glm::mat4 pata2 = glm::mat4(1.0f);
		pata2 = glm::scale(pata2, glm::vec3(0.1f, 0.5f, 0.1f));

		pata2 = glm::translate(pata2, glm::vec3(-7.0f, 0.0f, 4.5f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pata2));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);



		glm::mat4 pata3 = glm::mat4(1.0f);
		pata3 = glm::scale(pata3, glm::vec3(0.1f, 0.5f, 0.1f));
		pata3 = glm::translate(pata3, glm::vec3(7.0f, 0.0f, -4.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pata3));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glm::mat4 pata4 = glm::mat4(1.0f);
		pata4 = glm::scale(pata4, glm::vec3(0.1f, 0.5f, 0.1f));
		pata4 = glm::translate(pata4, glm::vec3(-7.0f, 0.0f, -4.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pata4));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// Agregar 4/5
		//-------------------------DIUBJAR EJES Y RETICULA
		// Dibujar los ejes sin rotación (matriz de modelo identidad)
		glm::mat4 axisModel = glm::mat4(1.0f); // Matriz de modelo sin rotación
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(axisModel));

		// Dibujar los ejes
		glBindVertexArray(axisVAO);
		glDrawArrays(GL_LINES, 0, 6); // 6 vértices para los 3 ejes
		glBindVertexArray(0);


		// Dibujar LA RETICULA

		glm::mat4 gridModel = glm::mat4(1.0f); // Matriz de modelo para la retícula (sin transformación)
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(gridModel));
		glBindVertexArray(gridVAO);
		glDrawArrays(GL_LINES, 0, gridVertices.size() / 6); // Dibujar líneas
		glBindVertexArray(0);


		//-------------------------------FIN  EJES Y RETICULA


				

		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	// Agregar 5/5
	glDeleteVertexArrays(1, &axisVAO);
	glDeleteBuffers(1, &axisVBO);
	glDeleteVertexArrays(1, &gridVAO);
	glDeleteBuffers(1, &gridVAO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		 movY += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		 movY -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		 rotx += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		 rotz -= 0.4f;
 }


