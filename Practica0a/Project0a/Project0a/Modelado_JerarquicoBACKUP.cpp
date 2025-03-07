#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);



const GLint WIDTH = 1200, HEIGHT = 800;

float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float roty = 0.0f;
float rotx = 0.0f;
float rotz = 0.0f;

bool ctrlPressed = false; // Track if Ctrl is pressed
bool rightMousePressed = false; // Track if right mouse button is pressed
bool leftMousePressed = false; // Track if right mouse button is pressed
double lastX = WIDTH / 2.0; // Last X position of the mouse
double lastY = HEIGHT / 2.0; // Last Y position of the mouse
double lastZ = WIDTH / 2.0; // Last Y position of the mouse

//For model
//ARTICULACIONES:
float	hombro = 0.0f;


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
		-0.5f, -0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		-0.5f,  0.5f, 0.5f, 
		-0.5f, -0.5f, 0.5f, 
		
	    -0.5f, -0.5f,-0.5f, 
		 0.5f, -0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
	    -0.5f,  0.5f,-0.5f, 
	    -0.5f, -0.5f,-0.5f, 
		
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,
      
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};




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
	glm::vec3 color= glm::vec3(0.3f, 0.3f, 0.3f);

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp



		// movZ / 10.0f -
		view = glm::translate(view, glm::vec3(movX, movY, movZ - 2.0f));
		view = glm::rotate(view, glm::radians(roty), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, glm::radians(-rotx), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, glm::radians(rotz), glm::vec3(0.0f, 0.0f, 1.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		//Model Loc = VISUALICACION EN BRUTO.
		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//PERMITE RENDERIZAR EL MODELO CADA VEZ QUE YO LO GIRE
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//ASIGNARLE COLORES.
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	

		glBindVertexArray(VAO);
		
		//Model 
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A


		glBindVertexArray(0);

		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
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


		 // Rotate around X and Y axes (Ctrl + Right Click)
		 rotx += yoffset; // Rotate around the X-axis (vertical movement)
		 roty += xoffset; // Rotate around the Y-axis (horizontal movement)
		 // std::cout << "Ctrl + Right Click: rotx = " << rotx << ", roty = " << roty << std::endl;


	 }
	 if (ctrlPressed && leftMousePressed) {
		 // Rotate around Z axis (Ctrl + Left Click)
		 rotz += (xoffset + yoffset) * 0.5f; // Adjust the factor as needed
		 //std::cout << "Ctrl + Left Click: rotz = " << rotz << std::endl;
	 }

	 if (leftMousePressed && !ctrlPressed) {
		 // Rotate around Z axis (Ctrl + Left Click)
		 movX += xoffset / 100.0f; // Adjust the factor as needed
		 movY += yoffset / 100.0f; // Adjust the factor as needed
		 //std::cout << "Ctrl + Left Click: rotz = " << rotz << std::endl;
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


 void Inputs(GLFWwindow* window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		 glfwSetWindowShouldClose(window, true);

	 // Track Ctrl key state
	 if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		 ctrlPressed = true;
	 else
		 ctrlPressed = false;

	 // Existing key inputs
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.02f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movY += 0.04f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movY -= 0.04f;
	 //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	   //  movZ -= 0.08f;
	 //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	   //  movZ += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 roty += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 roty -= 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		 rotx += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		 rotz -= 0.4f;
 }