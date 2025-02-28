#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <vector>

//SI
// Definir el ancho y alto de la ventana
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    // Inicializar GLFW, que es una biblioteca para crear ventanas y manejar entrada
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Hacer que la ventana no sea redimensionable

    // Crear una ventana GLFW con las dimensiones especificadas
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cubo con Ejes Coordenados", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE; // Salir si no se pudo crear la ventana
    }
    glfwMakeContextCurrent(window); // Hacer que la ventana creada sea el contexto actual

    // Inicializar GLEW, que es una biblioteca para cargar extensiones de OpenGL
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE; // Salir si no se pudo inicializar GLEW
    }

    // Obtener las dimensiones del framebuffer (área de dibujo) de la ventana
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    // Configurar el viewport (área de la ventana donde se dibujará)
    glViewport(0, 0, screenWidth, screenHeight);

    // Habilitar pruebas de profundidad (para que los objetos más cercanos oculten a los lejanos)
    glEnable(GL_DEPTH_TEST);
    // Habilitar blending (mezcla de colores para transparencias)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Configurar la función de blending

    // Cargar los shaders desde los archivos "core.vs" (vertex shader) y "core.frag" (fragment shader)
    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Definir los vértices del cubo. Cada vértice tiene una posición (x, y, z) y un color (r, g, b)
    float cubeVertices[] = {
        // Positions          // Colors (Gradients mejorados)
        // Front (Gradiente de rojo a naranja)
        -0.1f, -0.1f,  0.1f,  1.0f, 0.2f, 0.2f, // Rojo oscuro
         0.1f, -0.1f,  0.1f,  1.0f, 0.5f, 0.2f, // Naranja oscuro
         0.1f,  0.1f,  0.1f,  1.0f, 0.8f, 0.2f, // Naranja claro
         0.1f,  0.1f,  0.1f,  1.0f, 0.8f, 0.2f,
        -0.1f,  0.1f,  0.1f,  1.0f, 0.5f, 0.2f,
        -0.1f, -0.1f,  0.1f,  1.0f, 0.2f, 0.2f,

        // Back (Gradiente de azul a morado)
        -0.1f, -0.1f, -0.1f,  0.2f, 0.2f, 1.0f, // Azul oscuro
         0.1f, -0.1f, -0.1f,  0.5f, 0.2f, 1.0f, // Morado oscuro
         0.1f,  0.1f, -0.1f,  0.8f, 0.2f, 1.0f, // Morado claro
         0.1f,  0.1f, -0.1f,  0.8f, 0.2f, 1.0f,
        -0.1f,  0.1f, -0.1f,  0.5f, 0.2f, 1.0f,
        -0.1f, -0.1f, -0.1f,  0.2f, 0.2f, 1.0f,

        // Right (Gradiente de verde a amarillo)
         0.1f, -0.1f,  0.1f,  0.2f, 1.0f, 0.2f, // Verde oscuro
         0.1f, -0.1f, -0.1f,  0.5f, 1.0f, 0.2f, // Verde amarillento
         0.1f,  0.1f, -0.1f,  0.8f, 1.0f, 0.2f, // Amarillo verdoso
         0.1f,  0.1f, -0.1f,  0.8f, 1.0f, 0.2f,
         0.1f,  0.1f,  0.1f,  0.5f, 1.0f, 0.2f,
         0.1f, -0.1f,  0.1f,  0.2f, 1.0f, 0.2f,

         // Left (Gradiente de morado a rosa)
         -0.1f,  0.1f,  0.1f,  1.0f, 0.2f, 1.0f, // Morado oscuro
         -0.1f,  0.1f, -0.1f,  1.0f, 0.5f, 1.0f, // Rosa oscuro
         -0.1f, -0.1f, -0.1f,  1.0f, 0.8f, 1.0f, // Rosa claro
         -0.1f, -0.1f, -0.1f,  1.0f, 0.8f, 1.0f,
         -0.1f, -0.1f,  0.1f,  1.0f, 0.5f, 1.0f,
         -0.1f,  0.1f,  0.1f,  1.0f, 0.2f, 1.0f,

         // Bottom (Gradiente de amarillo a blanco)
         -0.1f, -0.1f, -0.1f,  1.0f, 1.0f, 0.2f, // Amarillo oscuro
          0.1f, -0.1f, -0.1f,  1.0f, 1.0f, 0.5f, // Amarillo claro
          0.1f, -0.1f,  0.1f,  1.0f, 1.0f, 0.8f, // Blanco amarillento
          0.1f, -0.1f,  0.1f,  1.0f, 1.0f, 0.8f,
         -0.1f, -0.1f,  0.1f,  1.0f, 1.0f, 0.5f,
         -0.1f, -0.1f, -0.1f,  1.0f, 1.0f, 0.2f,

         // Top (Gradiente de cian a azul claro)
         -0.1f,  0.1f, -0.1f,  0.2f, 1.0f, 1.0f, // Cian oscuro
          0.1f,  0.1f, -0.1f,  0.5f, 1.0f, 1.0f, // Cian claro
          0.1f,  0.1f,  0.1f,  0.8f, 1.0f, 1.0f, // Azul claro
          0.1f,  0.1f,  0.1f,  0.8f, 1.0f, 1.0f,
         -0.1f,  0.1f,  0.1f,  0.5f, 1.0f, 1.0f,
         -0.1f,  0.1f, -0.1f,  0.2f, 1.0f, 1.0f,
    };


    // Definir los vértices de los ejes coordenados. Cada eje tiene un color diferente.
    float axisVertices[] = {
        // Ejes coordenados
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Origen a X (rojo)
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Origen a Y (azul)
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Origen a Z (verde)
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    };

    // Crear un VAO (Vertex Array Object) y un VBO (Vertex Buffer Object) para el cubo
    GLuint cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO); // Generar un VAO
    glGenBuffers(1, &cubeVBO); // Generar un VBO

    // Vincular el VAO y el VBO
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW); // Copiar los datos de los vértices al VBO

    // Configurar el atributo de posición (primer atributo, 3 componentes por vértice)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Configurar el atributo de color (segundo atributo, 3 componentes por vértice)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Desvincular el VBO y el VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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


    //------------------------------------------------------- Inicio de la retícula--------------
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
    //------------------------------------------------------- Final de la retícula--------------
    // Configurar la matriz de proyección (perspectiva)
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f,100.0f);

    // Bucle de renderizado (se ejecuta continuamente hasta que se cierra la ventana)
    while (!glfwWindowShouldClose(window)) {
        // Limpiar el buffer de color y el buffer de profundidad
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Balanced grayish-black

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Usar el shader que cargamos anteriormente
        ourShader.Use();

        // Configurar la matriz de vista (cámara)
        glm::mat4 view = glm::mat4(1.0f); //Genera una matriz identidad.


        //Por alguna razón desconocida, esto conviene hacerlo en este orden. 
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f)); // Mover la cámara hacia atrás en el eje Z
        view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotar la cámara 30 grados hacia arriba
        view = glm::rotate(view, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar la cámara 45 grados a la derecha
        view = glm::rotate(view, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar el cubo

        // Pasar las matrices de vista y proyección al shader
        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        
        // Dibujar el primer cubo con transformación (cubo original)
        glm::mat4 cubeModel1 = glm::mat4(1.0f);
        
        cubeModel1 = glm::scale(cubeModel1, glm::vec3(2.0f,2.0f, 2.0f));
        cubeModel1 = glm::translate(cubeModel1, glm::vec3(0.3f, 0.1f, -0.2f)); 

        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel1));
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        //Dibujar el segundo cubo (nuevo cubo)
        glm::mat4 cubeModel2 = glm::mat4(1.0f);
        cubeModel2 = glm::scale(cubeModel2, glm::vec3(1.0f, 2.0f, 1.0f));
        cubeModel2 = glm::translate(cubeModel2, glm::vec3(-0.5f, 0.1f, -0.5f));
        cubeModel2 = glm::rotate(cubeModel2, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel2));
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


        //Dibujar el tercer cubo (nuevo cubo)
        glm::mat4 cubeModel3 = glm::mat4(1.0f);
        cubeModel3 = glm::translate(cubeModel3, glm::vec3(-0.5f,0.2f,0.5f));
        cubeModel3 = glm::rotate(cubeModel3, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        cubeModel3 = glm::rotate(cubeModel3, glm::radians(215.0f), glm::vec3(0.0f, 1.0f, .0f));
        cubeModel3 = glm::scale(cubeModel3, glm::vec3(0.5f, 3.0f, 0.5f));

        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel3));
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


        //Dibujar el cuarto cubo (nuevo cubo)
        glm::mat4 cubeModel4 = glm::mat4(1.0f);
        cubeModel4 = glm::translate(cubeModel4, glm::vec3(0.5f, 0.2f, 0.5f));
        cubeModel4 = glm::rotate(cubeModel4, glm::radians(70.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        cubeModel4 = glm::rotate(cubeModel4, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, .0f));
        cubeModel4 = glm::scale(cubeModel4, glm::vec3(1.1f, 2.0f, 1.2f));

        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel4));
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