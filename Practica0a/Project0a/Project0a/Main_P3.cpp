#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <vector>

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    // Inicializar GLFW
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Crear ventana
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cubo con Ejes Coordenados", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Obtener dimensiones del framebuffer
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    // Configurar viewport
    glViewport(0, 0, screenWidth, screenHeight);

    // Habilitar depth testing y blending
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Cargar shaders
    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Definir vértices del cubo
    float cubeVertices[] = {
        // Posiciones          // Colores
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Front
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Back
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, // Left
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 1.0f, 0.2f, 0.5f, // Top
        0.5f, 0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.2f, 0.5f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.2f, 0.5f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.2f, 0.5f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
    };

    // Definir vértices de los ejes
    float axisVertices[] = {
        // Ejes coordenados
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Origen a X (rojo)
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Origen a Y (azul)
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Origen a Z (verde)
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    };

    // Crear VAO y VBO para el cubo
    GLuint cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ----------------------------------------------------------------------------
    // -----------------------     EJES     ---------------------------------------
    // ----------------------------------------------------------------------------


    // Crear VAO y VBO para los ejes
    GLuint axisVBO, axisVAO;
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ----------------------------------------------------------------------------
   // -----------------------    FIN  EJES     ---------------------------------------
   // ----------------------------------------------------------------------------
   // 
   // 
     // ----------------------------------------------------------------------------
   // -----------------------    INICIO  RETICULA     ---------------------------------------
   // ----------------------------------------------------------------------------
    // Definir los vértices de la retícula
    std::vector<float> gridVertices;
    float gridSize = 1.0f;       // Tamaño de la retícula (de -1 a 1 en X y Z)
    float gridStep = 0.0999f;      // Separación entre líneas
    float gridColor = 0.5f;      // Color gris para la retícula

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

    // Crear VBO y VAO para la retícula
    GLuint gridVBO, gridVAO;
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // ----------------------------------------------------------------------------
   // -----------------------    FIN  reticula     ---------------------------------------
   // --

    // Configurar proyección
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Usar shader
        ourShader.Use();

        // Configurar matriz de vista y proyección
        glm::mat4 view = glm::mat4(1.0f);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f)); // Mover la cámara hacia atrás
        
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f)); // Mueve la cámara hacia atrás en el eje Z
        view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota la cámara 45 grados hacia arriba
        view = glm::rotate(view, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rota la cámara 45 grados a la derecha
        glm::mat4 projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

        // Pasar matrices de vista y proyección al shader
        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar el cubo con rotación
        glm::mat4 cubeModel = glm::mat4(1.0f);
        //cubeModel = glm::rotate(cubeModel, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f)); // Rotar el cubo
        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

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


        // fIN LA RETICULA



        // Intercambiar buffers y procesar eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberar recursos
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &axisVAO);
    glDeleteBuffers(1, &axisVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVAO);
    glfwTerminate();

    return EXIT_SUCCESS;
}