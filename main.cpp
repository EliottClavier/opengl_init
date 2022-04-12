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

// Source du shader de vertex par défaut en C (Shader de positionnement dans un Normalized Device Coordinates)
// Shader qui communique la couleur entre eux avec vertexColor out et in
/*
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";
*/

// Utilisateur uniform
/*
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";
*/

// location 0 (3 premieres valeurs float pour un vertex) = position
// location 1 (3 valeurs suivantes float pour un vertex) = couleur
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

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

    // Intégration du shader de vertex pour OpenGL
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Intégration du shader de fragment (colorisation des pixels)
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Ajout des deux shaders au programme
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Suppression des shaders (inclus dans le programme)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Création du vertex
    /*
    std::vector<Vertice> vertices{
        Vertice {-0.5f, -0.5f, 0.0f,},
        Vertice {0.5f, -0.5f, 0.0f,},
        Vertice {0.0f,  0.5f, 0.0f},
    };
    */

    /*
    float vertices[] = {
          0.0f,  -0.8f,  0.0f,
          0.4f,   0.0f,  0.0f,
          0.8f,  -0.8f,  0.0f,

          0.0f,  -0.8f,  0.0f,
         -0.4f,   0.0f,  0.0f,
         -0.8f,  -0.8f,  0.0f,

         -0.4f,   0.0f,  0.0f,
          0.0f,   0.8f,  0.0f,
          0.4f,   0.0f,  0.0f,
    };
    */

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

    int verticesNumber = { std::size(vertices)};


    /*
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    */
    
    // Dessine seulement les lignes
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Dessine en remplissant (valeur par défaut)
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Création de l'EBO qui permet de lier indices et vertices
    /*
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */


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

        // rendering commands here

        // gestion de la couleur du buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Utilisation du programme de shaders
        glUseProgram(shaderProgram);

        // Génération random d'une couleur en fonction du temps
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // On précise que cette couleur doit être appliqué à ourColor dans l'uniform du fragment shader
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // Utilisateur de la couleur random dans les shaders
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        // Utilisation du VAO courant
        glBindVertexArray(VAO);

        // Dessin du triangle
        glDrawArrays(GL_TRIANGLES, 0, verticesNumber);

        // Dessin du rectangle
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
