#include <iostream>         // Librer�a est�ndar de C++ para entrada/salida (cout, cerr, etc.)
#include <cmath>            // Librer�a matem�tica para funciones como sin(), cos(), etc.

// GLEW: Gestiona extensiones de OpenGL modernas
#include <GL/glew.h>

// GLFW: Crea ventanas, gestiona eventos de entrada, y contexto de OpenGL
#include <GLFW/glfw3.h>

// stb_image: Carga im�genes para usarlas como texturas
#include "stb_image.h"

// GLM: Biblioteca matem�tica para gr�ficos (vectores, matrices, transformaciones)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>   // Funciones como glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp>           // Convierte matrices/vectores a punteros para pasarlos a shaders

// SOIL2: Alternativa a stb_image, tambi�n para cargar texturas desde archivos
#include "SOIL2/SOIL2.h"

// Clases personalizadas:
// Shader: Clase para compilar y usar shaders de OpenGL (vertex, fragment)
// Camera: Implementaci�n de c�mara en primera persona
// Model: Carga y dibuja modelos 3D (por ejemplo, desde archivos .obj)
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// === Prototipos de funciones de callback (entrada de usuario) ===
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);  // Teclado
void MouseCallback(GLFWwindow* window, double xPos, double yPos);                   // Rat�n
void DoMovement();                                                                  // Movimiento de c�mara

// === Dimensiones de la ventana principal ===
const GLuint WIDTH = 1600, HEIGHT = 1200;  // Resoluci�n inicial
int SCREEN_WIDTH, SCREEN_HEIGHT;           // Se ajustan seg�n el framebuffer real

// === C�mara ===
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));  // C�mara colocada en Z=3, mirando hacia el origen
GLfloat lastX = WIDTH / 2.0f;               // Posici�n del rat�n al inicio (X)
GLfloat lastY = HEIGHT / 2.0f;              // Posici�n del rat�n al inicio (Y)
bool keys[1024];                            // Estado de teclas presionadas
bool firstMouse = true;                     // Para evitar saltos bruscos al mover el rat�n por primera vez

// === Luz direccional y estados ===
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);       // Posici�n inicial de la luz (puede usarse como luz puntual o direccional)
bool active;                                // Estado de encendido/apagado de una luz

// === Posiciones de luces puntuales ===
// Puedes activar hasta 4 luces en el shader y moverlas individualmente
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),  // Luz 0
	glm::vec3(0.0f, 0.0f, 0.0f),  // Luz 1
	glm::vec3(0.0f, 0.0f, 0.0f),  // Luz 2
	glm::vec3(0.0f, 0.0f, 0.0f)   // Luz 3
};

// === Control din�mico de color de la luz ===
// Light1 se usa para alterar el color de una luz puntual con sin() para lograr efectos animados
glm::vec3 Light1 = glm::vec3(0);

// === Control de tiempo entre frames ===
// deltaTime: tiempo transcurrido entre el frame anterior y el actual
// lastFrame: momento exacto en que ocurri� el �ltimo frame
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// === Inicializa GLFW ===
	glfwInit();

	// Opciones de contexto (comentadas pero �tiles si usas OpenGL Core Profile moderno)
	/*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);           // OpenGL versi�n 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core profile sin funciones obsoletas
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);     // Compatibilidad con Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                // Ventana no redimensionable
	*/

	// === Crea la ventana principal ===
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Establece el contexto actual de OpenGL
	glfwMakeContextCurrent(window);

	// Obtiene el tama�o real del framebuffer (�til para pantallas HiDPI)
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// === Registra callbacks de entrada ===
	glfwSetKeyCallback(window, KeyCallback);           // Teclado
	glfwSetCursorPosCallback(window, MouseCallback);   // Rat�n

	// === Configura el modo del cursor (opcional: oculta y centra el cursor) ===
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// === Inicializa GLEW para usar funciones modernas de OpenGL ===
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// === Define el �rea visible del viewport (normalmente igual a la resoluci�n de pantalla) ===
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	// === Carga de Shaders ===
// lightingShader: Shader principal con iluminaci�n (vertex y fragment)
// lampShader: Shader m�s simple, usado para dibujar los cubos que representan las luces
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// === Carga de Modelos 3D (.obj) ===
	// Cada uno se carga a partir de un archivo .obj ubicado en la carpeta Models/
	Model Crystal((char*)"Models/Crystal.obj");               // Modelo decorativo (ej. cristal)
	Model Ground((char*)"Models/Ground.obj");                 // Piso o suelo del escenario
	Model Tiny((char*)"Models/TinyHouse002_2021.obj");        // Casa o estructura principal

	// === Asignaci�n de unidades de textura a los uniforms del shader ===
	lightingShader.Use();  // Activa el shader antes de modificar sus uniforms

	// Vincula el uniform Material.difuse al GL_TEXTURE0
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);

	// Vincula el uniform Material.specular al GL_TEXTURE1
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	// === Matriz de proyecci�n en perspectiva ===
	// Define un campo de visi�n (FOV), una relaci�n de aspecto, y planos cercanos/lejanos
	// Esto simula c�mo ve una c�mara real: objetos m�s lejanos se ven m�s peque�os
	glm::mat4 projection = glm::perspective(
		camera.GetZoom(),                                 // Campo de visi�n (zoom)
		(GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,   // Relaci�n de aspecto
		0.1f,                                              // Plano cercano
		100.0f                                             // Plano lejano
	);

	// === Bucle principal de renderizado ===
// Este ciclo se ejecuta mientras la ventana no se haya cerrado manualmente
	while (!glfwWindowShouldClose(window))
	{
		// === Calcular tiempo entre frames ===
		// Esto permite que el movimiento y animaciones sean consistentes independientemente del framerate
		GLfloat currentFrame = glfwGetTime();          // Tiempo actual
		deltaTime = currentFrame - lastFrame;          // Diferencia entre frames
		lastFrame = currentFrame;                      // Actualiza el tiempo del �ltimo frame

		// === Procesar eventos de entrada del sistema operativo (teclado, mouse, etc.) ===
		glfwPollEvents();
		DoMovement();  // Aplica movimiento a la c�mara basado en teclas presionadas

		// === Limpiar el buffer de color y profundidad ===
		glClearColor(0.529f, 0.808f, 0.922f, 1.0f);     // Color de fondo (azul cielo)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Borra color y z-buffer

		// === Habilitar el test de profundidad (Z-buffer) para render 3D correcto ===
		glEnable(GL_DEPTH_TEST);

		// === Activar shader principal con iluminaci�n ===
		lightingShader.Use();

		// (Opcional) Uniform para textura difusa si no est� establecido globalmente
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		// glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"), 1); // Solo si se usa mapa especular

		// === Enviar posici�n de la c�mara al shader (�til para iluminaci�n especular) ===
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc,
			camera.GetPosition().x,
			camera.GetPosition().y,
			camera.GetPosition().z
		);

		// === Configurar luz direccional (tipo sol) ===
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 2.0f, -1.0f, -0.3f);  // Direcci�n de la luz
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.8f, 0.8f, 0.8f);    // Luz ambiental (iluminaci�n general)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);    // Luz difusa (iluminaci�n desde direcci�n)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);    // Luz especular (reflejo)
 
		// === Luz puntual 1 (din�mica) ===
// Esta luz var�a su color con el tiempo usando funciones seno
// Para efectos visuales animados tipo ne�n o RGB
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));  // Componente roja
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));  // Componente verde
		lightColor.z = sin(glfwGetTime() * Light1.z);       // Componente azul

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);  // Especular fija
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		// === Luz puntual 2 (inactiva) ===
		// Puedes activarla o configurar sus valores para otro prop�sito
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

		// === Luz puntual 3 (inactiva) ===
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

		// === Luz puntual 4 (inactiva) ===
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

		// === Luz tipo linterna (spotLight) ===
		// Esta luz sigue la c�mara (como una linterna en primera persona)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
			camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
			camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f); // Luz tenue base
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f); // Luz principal (por defecto apagada)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f); // Reflejos (por defecto apagados)

		// Atenuaci�n (decaimiento de la luz con la distancia)
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);

		// �ngulos de corte del haz de luz (cono de la linterna)
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
			glm::cos(glm::radians(12.0f)));  // N�cleo del haz
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
			glm::cos(glm::radians(18.0f)));  // Suavizado del borde


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);
		// === Carga y renderizado del modelo Ground (suelo) ===
// Aplica la matriz de vista y una matriz modelo identidad (sin transformaciones)
		view = camera.GetViewMatrix();
		model = glm::mat4(1);  // Sin traslaci�n ni escalado
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ground.Draw(lightingShader);  // Dibuja el modelo del piso

		// === Renderizado del modelo Tiny (la tiny house) ===
		// Se activa la funcionalidad de mezcla alfa para permitir transparencia si es necesaria
		model = glm::mat4(1);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Configura el modo de mezcla
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);  // Uniform que controla transparencia en el shader
		Tiny.Draw(lightingShader);  // Dibuja la casa
		glBindVertexArray(0);  // Desvincula el VAO para prevenir errores

		// === Renderizado del modelo Crystal (modelo decorativo transparente) ===
		model = glm::mat4(1);

		glEnable(GL_BLEND);  // Se activa el canal alfa para manejar transparencia del cristal
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);  // Se puede cambiar a 1 si el shader lo maneja como booleano
		Crystal.Draw(lightingShader);  // Dibuja el modelo del cristal
		glDisable(GL_BLEND);           // Desactiva la mezcla una vez dibujado
		glBindVertexArray(0);




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to top

	lastX = xPos;
	lastY = yPos;

	// Aumenta este valor para m�s sensibilidad (ej. 0.2, 0.3, etc.)
	float sensitivity = 3.0f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
