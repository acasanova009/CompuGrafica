#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Agregar 1/5
#include <vector>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);


float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float roty = 0.0f;
float rotx = 0.0f;
float rotz = 0.0f;


const GLint WIDTH = 1200, HEIGHT = 800;

bool ctrlPressed = false; // Track if Ctrl is pressed
bool rightMousePressed = false; // Track if right mouse button is pressed
bool leftMousePressed = false; // Track if right mouse button is pressed
double lastX = WIDTH / 2.0; // Last X position of the mouse
double lastY = HEIGHT / 2.0; // Last Y position of the mouse
double lastZ = WIDTH / 2.0; // Last Y position of the mouse




//For Keyboard
float	movX_fig = 0.0f,
movY_fig = 0.0f,
movZ_fig = -5.0f;
//
//For model
float	hombro = 0.0f;
float	codo = 0.0f;
float	mano = 0.0f;
float	dedo1 = 45.0f;
float	falanje1 = -30.0f;

float	dedo2 = 0.0f;
float	falanje2 = 0.0f;

float	dedo3 = -45.0f;
float	falanje3 = 30.0f;

int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado jerarquico", nullptr, nullptr);

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
	// use with Orthographic Projection


	

	// use with Perspective Projection
	float vertices[] = {
	-0.1f, -0.1f,  0.1f,
	 0.1f, -0.1f,  0.1f,
	 0.1f,  0.1f,  0.1f,
	 0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f,

	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	-0.1f,  0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,

	 0.1f, -0.1f,  0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f,
	 0.1f, -0.1f,  0.1f,

	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,

	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f,
	 0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
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
	

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color= glm::vec3(0.0f, 0.0f, 1.0f);


	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);



	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		
		ourShader.Use();
		glm::mat4 view=glm::mat4(1);
		glm::mat4 model=glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp (Mainbrazo)
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp (Upper brazo)
		glm::mat4 modelTemp3 = glm::mat4(1.0f); //Mano (Muneca)
		glm::mat4 modelTemp4 = glm::mat4(1.0f); //Temp  (Dedito1)
		glm::mat4 modelTemp5 = glm::mat4(1.0f); //Temp  (Falange1)

		glm::mat4 modelTemp6 = glm::mat4(1.0f); //Temp  (Dedito2)
		glm::mat4 modelTemp7 = glm::mat4(1.0f); //Temp  (Falange2)

		glm::mat4 modelTemp8 = glm::mat4(1.0f); //Temp  (Dedito3)
		glm::mat4 modelTemp9 = glm::mat4(1.0f); //Temp  (Falange3)

		//View set up 
		//view = glm::translate(view, glm::vec3(movX,movY, movZ));
		//view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
	
		view = glm::translate(view, glm::vec3(movX, movY, movZ + 1.0f));
		view = glm::rotate(view, glm::radians(roty + -40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, glm::radians(-rotx + 30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, glm::radians(rotz + -20.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	

		glBindVertexArray(VAO);

		////----------------------------------------------HOMBRO 1------------------//
// Modelado del Hombro (Shoulder)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f)); // Centrar el objeto en el pivote del hombro
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación del hombro
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // Desplazamiento basado en la escala y tamaño
		modelTemp = model; // Guardar la matriz de transformación
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); // Escalar el hombro

		color = glm::vec3(0.55f, 0.55f, 0.55f); // Gris medio
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Dibujar el brazo superior

		////----------------------------------------------CODO 1------------------//
		// Modelado del Codo (Elbow)
		model = modelTemp; // Restaurar transformación del hombro
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // Posicionar en la articulación del codo
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación del codo
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // Desplazamiento al antebrazo
		modelTemp2 = model; // Guardar transformación del codo
		model = glm::scale(model, glm::vec3(2.0f, 0.75f, 0.75f)); // Escalar el antebrazo

		color = glm::vec3(0.40f, 0.40f, 0.40f); // Gris oscuro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Dibujar el antebrazo

		////----------------------------------------------MUÑECA 1------------------//
		// Modelado de la Mano (Hand)
		model = modelTemp2; // Restaurar transformación del codo
		model = glm::translate(model, glm::vec3(0.05f, 0.0f, 0.0f)); // Posicionar en la muñeca
		model = glm::rotate(model, glm::radians(mano), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la muñeca
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // Desplazamiento a la mano
		modelTemp3 = model; // Guardar transformación de la muñeca
		model = glm::scale(model, glm::vec3(0.5f, 1.2f, 1.2f)); // Escalar la mano

		color = glm::vec3(0.55f, 0.55f, 0.55f); // Gris medio
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Dibujar la mano

		////----------------------------------------------DEDITO 1------------------//
		// Modelado del Dedo 1
		model = modelTemp3; // Restaurar transformación de la muñeca
		model = glm::translate(model, glm::vec3(0.05f, 0.08f, 0.08f)); // Posicionar el dedo
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar el dedo
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f)); // Desplazamiento final
		modelTemp4 = model; // Guardar transformación del dedo
		model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.1f)); // Escalar el dedo

		color = glm::vec3(0.70f, 0.70f, 0.70f); // Gris claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Dibujar el dedo

		////----------------------------------------------FALANJE 1------------------//
		// Modelado de la Falange 1
		model = modelTemp4; // Restaurar transformación del dedo
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f)); // Posicionar la falange
		model = glm::rotate(model, glm::radians(falanje1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación de la falange
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f)); // Desplazamiento final
		model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.1f)); // Escalar la falange

		color = glm::vec3(0.85f, 0.85f, 0.85f); // Gris más claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Dibujar la falange


		////----------------------------------------------DEDITO 2------------------//

		// Model - Mano (Hand)
		model = modelTemp3; // Reset to elbow transformation
		model = glm::translate(model, glm::vec3(0.05f, 0.08f, -0.08f)); // Move to wrist position (end of lower arm)
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate wrist
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f)); // Move to hand position
		modelTemp4 = model; // Store elbow for tranformation
		model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.1f)); // Scale for hand


		color = glm::vec3(0.70f, 0.70f, 0.70f); // Lighter Grey 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Draw hand

		////----------------------------------------------FALANJE 2------------------//

		// Model - Mano (Hand)
		model = modelTemp4; // Reset to elbow transformation
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f)); // Move to wrist position (end of lower arm)
		model = glm::rotate(model, glm::radians(falanje1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate wrist
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f)); // Move to hand position
		//modelTemp4 = model; // Store elbow for tranformation
		model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.1f)); // Scale for hand

		color = glm::vec3(0.85f, 0.85f, 0.85f); // Lightest Grey
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Draw hand


		////----------------------------------------------DEDITO 3------------------//

		// Model - Mano (Hand)
		model = modelTemp3; // Reset to elbow transformation
		model = glm::translate(model, glm::vec3(0.05f, -0.08f, 0.0f)); // Move to wrist position (end of lower arm)
		model = glm::rotate(model, glm::radians(dedo3), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate wrist
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f)); // Move to hand position
		modelTemp4 = model; // Store elbow for tranformation
		model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.1f)); // Scale for hand


		color = glm::vec3(0.70f, 0.70f, 0.70f); // Lighter Grey
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Draw hand

		////----------------------------------------------FALANJE 3------------------//

		// Model - Mano (Hand)
		model = modelTemp4; // Reset to elbow transformation
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f)); // Move to wrist position (end of lower arm)
		model = glm::rotate(model, glm::radians(falanje3), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate wrist
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f)); // Move to hand position
		//modelTemp4 = model; // Store elbow for tranformation
		model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.1f)); // Scale for hand

		color = glm::vec3(0.85f, 0.85f, 0.85f); // Lightest Grey 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Draw hand

		



		// Agregar 4/5
	   //-------------------------DIUBJAR EJES Y RETICULA
	   // Dibujar los ejes sin rotación (matriz de modelo identidad)
		glm::mat4 axisModel = glm::mat4(1.0f); // Matriz de modelo sin rotación
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(axisModel));

		//R
			//A
			//V
		// Dibujar los ejes
		glBindVertexArray(axisVAO);
		color = glm::vec3(1.0f, 0.0f, 0.0f); //ROJO
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glDrawArrays(GL_LINES, 0, 2); // 6 vértices para los 3 ejes

		color = glm::vec3(0.0f, 0.0f, 1.0f); // BLUE
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glDrawArrays(GL_LINES, 2, 2); // 6 vértices para los 3 ejes

		color = glm::vec3(0.0f, 1.0f, 0.0f); // GREEN
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glDrawArrays(GL_LINES, 4, 2); // 6 vértices para los 3 ejes

		glBindVertexArray(0);




		// Dibujar LA RETICULA

		glm::mat4 gridModel = glm::mat4(1.0f); // Matriz de modelo para la retícula (sin transformación)
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(gridModel));
		glBindVertexArray(gridVAO);

		
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glDrawArrays(GL_LINES, 0, gridVertices.size() / 6); // Dibujar líneas
		glBindVertexArray(0);


		//-------------------------------FIN  EJES Y RETICULA



		glBindVertexArray(0);

		
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
	 
	 
	 if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		 hombro += 1.212f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 hombro -= 1.212f;
	
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 codo += 1.212f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 codo -= 1.212f;


	 if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		 mano += 1.212f;
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 mano -= 1.212f;


	 if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {

		 dedo1 -= 1.212f;
		 falanje1 -= 1.212f;

		 dedo2 -= 1.212f;
		 falanje2 -= 1.212f;
		 dedo3 += 1.212f;
		 falanje3 += 1.212f;
	 }

	 if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		 dedo1 += 1.212f;
		 falanje1 += 1.212f;

		 dedo2 += 1.212f;
		 falanje2 += 1.212f;
		 dedo3 -= 1.212f;
		 falanje3 -= 1.212f;
	 }



	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		 glfwSetWindowShouldClose(window, true);

	 // Track Ctrl key state
	 if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		 ctrlPressed = true;
	 else
		 ctrlPressed = false;
 }



 void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	 double xoffset = xpos - lastX;
	 double yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

	 lastX = xpos;
	 lastY = ypos;

	 float sensitivity = 0.5f; // Adjust sensitivity as needed
	 xoffset *= sensitivity;
	 yoffset *= sensitivity;
	 if (ctrlPressed && rightMousePressed) {


		 // Rotate around Z axis (Ctrl + Left Click)
		 movX += xoffset / 100.0f; // Adjust the factor as needed
		 movY += yoffset / 100.0f; // Adjust the factor as needed
		 //std::cout << "Ctrl + Left Click: rotz = " << rotz << std::endl;

	 }
	 if (ctrlPressed && leftMousePressed) {
		 // Rotate around Z axis (Ctrl + Left Click)
		 rotz += (xoffset + yoffset) * 0.5f; // Adjust the factor as needed
		 //std::cout << "Ctrl + Left Click: rotz = " << rotz << std::endl;
	 }

	 if (leftMousePressed && !ctrlPressed) {
		 // Rotate around X and Y axes (Ctrl + Right Click)
		 rotx += yoffset; // Rotate around the X-axis (vertical movement)
		 roty += xoffset; // Rotate around the Y-axis (horizontal movement)
		 // std::cout << "Ctrl + Right Click: rotx = " << rotx << ", roty = " << roty << std::endl;

	 }

 }
 void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	 if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		 if (action == GLFW_PRESS) {


			 rightMousePressed = true;
		 }
		 else if (action == GLFW_RELEASE)
			 rightMousePressed = false;
	 }
	 if (button == GLFW_MOUSE_BUTTON_LEFT) {
		 if (action == GLFW_PRESS) {


			 leftMousePressed = true;
		 }
		 else if (action == GLFW_RELEASE)
			 leftMousePressed = false;
	 }
 }

 void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	 movZ += yoffset * 0.5f; // Adjust the zoom speed as needed
 }


