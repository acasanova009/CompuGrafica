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

// Prototipos de funciones
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animacion();

// Dimensiones de la ventana
const GLuint WIDTH = 1600, HEIGHT = 1200;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Cámara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024];
bool firstMouse = true;

// Variables para animaciones
bool doorOpen = false;
bool windowOpen = false;
float doorOffset = 0.0f;
float windowOffset = 0.0f;
const float doorMaxOffset = 0.5f;
const float windowMaxOffset = 0.38f;
const float animationSpeed = 2.0f;

// Animación microondas
float doorAngle = 0.0f;
bool isDoorOpening = false;
float doorOpenSpeed = 50.0f;
const float microwaveMaxAngle = 90.0f;
glm::vec3 doorPosition(0.0f, 0.0f, 0.0f);

// Animación de la casa
bool houseMoving = false;
float houseOffset = 0.0f;
const float houseMaxOffset = 5.0f; // Máximo desplazamiento
const float houseMoveSpeed = 1.5f; // Velocidad de movimiento
glm::vec3 houseInitialPosition(0.0f, 0.0f, 0.0f); // Posición inicial

// Variables de iluminación
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};
glm::vec3 Light1 = glm::vec3(0);

// Control de tiempo
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Inicialización de GLFW
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz", nullptr, nullptr);
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

    // Inicialización de GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Carga de shaders y modelos
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    Model Crystal((char*)"Models/crystal2.obj");
    Model Ground((char*)"Models/Ground.obj");
    Model Tiny((char*)"Models/TinyHouse003_2021.obj");
    Model Door((char*)"Models/door.obj");
    Model Window((char*)"Models/window.obj");
    Model DoorCrystal((char*)"Models/DoorCrystal.obj");
    Model WindowCrystal((char*)"Models/windowCrystal.obj");
    Model Microwave((char*)"Models/microwave.obj");

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(
        camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
        0.1f,
        100.0f
    );

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        Animacion();  // Llamada a la función de animación

        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc,
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z
        );

        // Configuración de luces 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 2.0f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);

        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // === Luz puntual 2 (inactiva) ===
        // Puedes activarla o configurar sus valores para otro propósito
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
        // Esta luz sigue la cámara (como una linterna en primera persona)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f); // Luz tenue base
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f); // Luz principal (por defecto apagada)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f); // Reflejos (por defecto apagados)

        // Atenuación (decaimiento de la luz con la distancia)
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);

        // Ángulos de corte del haz de luz (cono de la linterna)
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
            glm::cos(glm::radians(12.0f)));  // Núcleo del haz
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
            glm::cos(glm::radians(18.0f)));  // Suavizado del borde


        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model(1);

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Renderizado de modelos estáticos
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ground.Draw(lightingShader);

        // Renderizado de la casa con animación
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, houseOffset));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        Tiny.Draw(lightingShader);

        // Renderizado de la puerta principal
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, houseOffset));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, doorOffset));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Door.Draw(lightingShader);

        // Renderizado de la ventana
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, houseOffset));
        model = glm::translate(model, glm::vec3(0.0f, -windowOffset, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Window.Draw(lightingShader);

        // Renderizado de la puerta del microondas 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, houseOffset));
        model = glm::translate(model, doorPosition);

        const float hingeOffsetX = 5.32f;
        const float hingeOffsetY = 0.0f;
        const float hingeOffsetZ = 0.5f;

        // Aplicar transformaciones de rotación sobre el pivote
        model = glm::translate(model, glm::vec3(hingeOffsetX / 2, hingeOffsetY, hingeOffsetZ));
        model = glm::rotate(model, glm::radians(doorAngle), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-hingeOffsetX / 2, -hingeOffsetY, -hingeOffsetZ));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Microwave.Draw(lightingShader);

        // Renderizado del cristal decorativo
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, houseOffset));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        Crystal.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        // Renderizado del cristal de la puerta con animación
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, houseOffset));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, doorOffset));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        DoorCrystal.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        // Renderizado del cristal de la ventana con animación
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, houseOffset));
        model = glm::translate(model, glm::vec3(0.0f, -windowOffset, 0.0f));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        WindowCrystal.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Función para manejar las animaciones
void Animacion()
{
    // Animación de la puerta
    if (doorOpen && doorOffset < doorMaxOffset) {
        doorOffset += animationSpeed * deltaTime;
        if (doorOffset > doorMaxOffset) doorOffset = doorMaxOffset;
    }
    else if (!doorOpen && doorOffset > 0.0f) {
        doorOffset -= animationSpeed * deltaTime;
        if (doorOffset < 0.0f) doorOffset = 0.0f;
    }

    // Animación de la ventana
    if (windowOpen && windowOffset < windowMaxOffset) {
        windowOffset += animationSpeed * deltaTime;
        if (windowOffset > windowMaxOffset) windowOffset = windowMaxOffset;
    }
    else if (!windowOpen && windowOffset > 0.0f) {
        windowOffset -= animationSpeed * deltaTime;
        if (windowOffset < 0.0f) windowOffset = 0.0f;
    }

    // Animación puerta microondas
    if (isDoorOpening) {
        doorAngle += doorOpenSpeed * deltaTime;
        if (doorAngle >= microwaveMaxAngle) doorAngle = microwaveMaxAngle;
    }
    else {
        doorAngle -= doorOpenSpeed * deltaTime;
        if (doorAngle <= 0.0f) doorAngle = 0.0f;
    }

    // Animación de la casa
    if (houseMoving && houseOffset < houseMaxOffset) {
        houseOffset += houseMoveSpeed * deltaTime;
        if (houseOffset > houseMaxOffset) houseOffset = houseMaxOffset;
    }
    else if (!houseMoving && houseOffset > 0.0f) {
        houseOffset -= houseMoveSpeed * deltaTime;
        if (houseOffset < 0.0f) houseOffset = 0.0f;
    }
}

void DoMovement()
{
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

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
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

            // Control de animación de la puerta con tecla 1
            if (key == GLFW_KEY_1)
            {
                doorOpen = !doorOpen;
            }

            // Control de animación de la ventana con tecla 2
            if (key == GLFW_KEY_2)
            {
                windowOpen = !windowOpen;
            }

            // Control de animación del microondas con tecla 3
            if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
                isDoorOpening = !isDoorOpening;
            }

            // Control de animación de la casa con tecla 4
            if (key == GLFW_KEY_4) {
                houseMoving = !houseMoving;
            }
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
            Light1 = glm::vec3(0);
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
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    float sensitivity = 3.0f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    camera.ProcessMouseMovement(xOffset, yOffset);
}