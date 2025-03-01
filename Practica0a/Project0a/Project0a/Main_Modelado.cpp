#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Agregar 1/5
#include <vector>

#include "Shader.h"

void Inputs(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

const GLint WIDTH = 800, HEIGHT = 600;
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

int main() {

    
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado geometrico", nullptr, nullptr);

    

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    

    if (nullptr == window) {
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

    //float vertices[] = {
    //    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Front
    //    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    //    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    //    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    //    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    //    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

    //    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Back
    //    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

    //    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Right
    //    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    //    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    //    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    //    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    //    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    //    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, // Left
    //    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    //    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    //    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    //    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
    //    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

    //    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom
    //    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    //    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    //    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    //    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,

    //    -0.5f, 0.5f, -0.5f, 1.0f, 0.2f, 0.5f, // Top
    //    0.5f, 0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
    //    0.5f, 0.5f, 0.5f, 1.0f, 0.2f, 0.5f,
    //    0.5f, 0.5f, 0.5f, 1.0f, 0.2f, 0.5f,
    //    -0.5f, 0.5f, 0.5f, 1.0f, 0.2f, 0.5f,
    //    -0.5f, 0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
    //};

    float vertices[] = {
        // Front face (Café claro a café medio)
        -0.5f, -0.5f,  0.5f,  0.8f, 0.6f, 0.4f, // Café claro
         0.5f, -0.5f,  0.5f,  0.7f, 0.5f, 0.3f,
         0.5f,  0.5f,  0.5f,  0.6f, 0.4f, 0.2f, // Café medio
         0.5f,  0.5f,  0.5f,  0.6f, 0.4f, 0.2f,
        -0.5f,  0.5f,  0.5f,  0.7f, 0.5f, 0.3f,
        -0.5f, -0.5f,  0.5f,  0.8f, 0.6f, 0.4f,

        // Back face (Café medio a café oscuro)
        -0.5f, -0.5f, -0.5f,  0.6f, 0.4f, 0.2f, // Café medio
         0.5f, -0.5f, -0.5f,  0.5f, 0.3f, 0.1f,
         0.5f,  0.5f, -0.5f,  0.4f, 0.2f, 0.0f, // Café oscuro
         0.5f,  0.5f, -0.5f,  0.4f, 0.2f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.5f, 0.3f, 0.1f,
        -0.5f, -0.5f, -0.5f,  0.6f, 0.4f, 0.2f,

        // Right face (Café claro a café oscuro)
         0.5f, -0.5f,  0.5f,  0.8f, 0.6f, 0.4f, // Café claro
         0.5f, -0.5f, -0.5f,  0.7f, 0.5f, 0.3f,
         0.5f,  0.5f, -0.5f,  0.6f, 0.4f, 0.2f, // Café medio
         0.5f,  0.5f, -0.5f,  0.6f, 0.4f, 0.2f,
         0.5f,  0.5f,  0.5f,  0.7f, 0.5f, 0.3f,
         0.5f, -0.5f,  0.5f,  0.8f, 0.6f, 0.4f,

         // Left face (Café medio a café claro)
         -0.5f,  0.5f,  0.5f,  0.6f, 0.4f, 0.2f, // Café medio
         -0.5f,  0.5f, -0.5f,  0.7f, 0.5f, 0.3f,
         -0.5f, -0.5f, -0.5f,  0.8f, 0.6f, 0.4f, // Café claro
         -0.5f, -0.5f, -0.5f,  0.8f, 0.6f, 0.4f,
         -0.5f, -0.5f,  0.5f,  0.7f, 0.5f, 0.3f,
         -0.5f,  0.5f,  0.5f,  0.6f, 0.4f, 0.2f,

         // Bottom face (Café oscuro a café medio)
         -0.5f, -0.5f, -0.5f,  0.4f, 0.2f, 0.0f, // Café oscuro
          0.5f, -0.5f, -0.5f,  0.5f, 0.3f, 0.1f,
          0.5f, -0.5f,  0.5f,  0.6f, 0.4f, 0.2f, // Café medio
          0.5f, -0.5f,  0.5f,  0.6f, 0.4f, 0.2f,
         -0.5f, -0.5f,  0.5f,  0.5f, 0.3f, 0.1f,
         -0.5f, -0.5f, -0.5f,  0.4f, 0.2f, 0.0f,

         // Top face (Café claro a café medio)
         -0.5f,  0.5f, -0.5f,  0.8f, 0.6f, 0.4f, // Café claro
          0.5f,  0.5f, -0.5f,  0.7f, 0.5f, 0.3f,
          0.5f,  0.5f,  0.5f,  0.6f, 0.4f, 0.2f, // Café medio
          0.5f,  0.5f,  0.5f,  0.6f, 0.4f, 0.2f,
         -0.5f,  0.5f,  0.5f,  0.7f, 0.5f, 0.3f,
         -0.5f,  0.5f, -0.5f,  0.8f, 0.6f, 0.4f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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


    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);


    while (!glfwWindowShouldClose(window)) {

        
        
        Inputs(window);
        glfwPollEvents();



        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        ourShader.Use();
        glm::mat4 view = glm::mat4(1);
        glm::mat4 model = glm::mat4(1);
        // movZ / 10.0f -
        view = glm::translate(view, glm::vec3(movX , movY , movZ -2.0f));
        view = glm::rotate(view, glm::radians(roty), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(-rotx), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotz), glm::vec3(0.0f, 0.0f, 1.0f));

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

        pata1 = glm::translate(pata1, glm::vec3(7.0f, -0.05f, 4.5f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pata1));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);



        glm::mat4 pata2 = glm::mat4(1.0f);
        pata2 = glm::scale(pata2, glm::vec3(0.1f, 0.5f, 0.1f));

        pata2 = glm::translate(pata2, glm::vec3(-7.0f, -0.05f, 4.5f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pata2));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);



        glm::mat4 pata3 = glm::mat4(1.0f);
        pata3 = glm::scale(pata3, glm::vec3(0.1f, 0.5f, 0.1f));
        pata3 = glm::translate(pata3, glm::vec3(7.0f, -0.05f, -4.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pata3));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glm::mat4 pata4 = glm::mat4(1.0f);
        pata4 = glm::scale(pata4, glm::vec3(0.1f, 0.5f, 0.1f));
        pata4 = glm::translate(pata4, glm::vec3(-7.0f, -0.05f, -4.5f));
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


        //std::cout << "Here" << std::endl;

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
        roty+= 0.4f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        roty-= 0.4f;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        rotx += 0.4f;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        rotz -= 0.4f;
}

//void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
//    if (ctrlPressed && rightMousePressed) {
//        double xoffset = xpos - lastX;
//        double yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
//
//        std::cout << "Both ctrl and right: "<<roty<< std::endl;
//        
//        lastX = xpos;
//        lastY = ypos;
//
//        float sensitivity = 0.5f; // Adjust sensitivity as needed
//        xoffset *= sensitivity;
//        yoffset *= sensitivity;
//
//        // Update rotation angles
//        rotx+= xoffset; // Rotate around the Y-axis (horizontal movement)
//        roty+= yoffset; // Rotate around the X-axis (vertical movement)
//    }
//}

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
            std::cout << "Ctrl + Right Click: rotx = " << rotx << ", roty = " << roty << std::endl;
        

    }
        if (ctrlPressed && leftMousePressed ) {
            // Rotate around Z axis (Ctrl + Left Click)
            rotz += (xoffset + yoffset) * 0.5f; // Adjust the factor as needed
            std::cout << "Ctrl + Left Click: rotz = " << rotz << std::endl;
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