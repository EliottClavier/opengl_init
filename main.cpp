#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    // Si ECHAP => destruction de la fenêtre
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

struct Vertice {
    float x;
    float y;
    float z;
};

struct Vertex {
    Vertice a;
    Vertice b;
    Vertice c;
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Création du vertex
    /*
    std::vector<Vertice> vertices{
        Vertice {-0.5f, -0.5f, 0.0f,},
        Vertice {0.5f, -0.5f, 0.0f,},
        Vertice {0.0f,  0.5f, 0.0f},
    };
    */

    Shader ourShader("shader.vs", "shader.fs");

    float vertices[] = {
        // Position           // Couleur
        0.0f,  -0.8f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.4f,   0.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.8f,  -0.8f,  0.0f,  0.0f, 0.0f, 1.0f,

        0.0f,  -0.8f,  0.0f,  1.0f, 0.0f, 0.0f,
       -0.4f,   0.0f,  0.0f,  0.0f, 1.0f, 0.0f,
       -0.8f,  -0.8f,  0.0f,  0.0f, 0.0f, 1.0f,

       -0.4f,   0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.0f,   0.8f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.4f,   0.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    };

    int verticesNumber = { std::size(vertices) };


    // Link Vertex au VBO qui va rendre les vertices avec le shader setup au dessus
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertice), &vertices[0], GL_STATIC_DRAW);      
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Création d'un VAO (Vertex Array Object)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Utilisation du VAO courant pour la première fois
    glBindVertexArray(VAO);

    // Configuration pour dire à OpenGL comment interpréter le tableau de vertices
    // Ici on a 0 qui indique la location dans le shader, 3 le nombre de float par vertice et 3* le stride (l'espace à parcourir dans le tableau entre deux vertices)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Dans ce cas, on passe à 6 car chaque vertice est composé de 6 floats, 3 positions et 3 couleurs dans le stride est plus grand
    // On a le vertex attrib pointer location 0 pour la position, et location 1 pour la couleur
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // rendering commands 

        // gestion de la couleur du buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Utilisation du shader
        ourShader.use();

        /*
        // On set l'uniform xOffset à 0.5f dans le vertex shader
        float offset = 1.5f;
        ourShader.setFloat("xOffset", offset);
        */

        // Utilisation du VAO courant
        glBindVertexArray(VAO);

        // Dessin du triangle
        glDrawArrays(GL_TRIANGLES, 0, verticesNumber);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
