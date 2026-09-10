//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0;//grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList; //lista
std::vector<MeshColor*> meshColorList;
std::vector<Shader> shaderList;
//Vertex Shader
//Shaders originales
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

//Shaders creados
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* fShaderRojo = "shaders/shaderrojo.frag";
static const char* vShaderAmarillo = "shaders/shaderamarillo.vert";
static const char* fShaderAmarillo = "shaders/shaderamarillo.frag";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* fShaderVerde = "shaders/shaderverde.frag";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* fShaderAzul = "shaders/shaderazul.frag";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* fShaderCafe = "shaders/shadercafe.frag";
static const char* vShaderNegro = "shaders/shadernegro.vert";
static const char* fShaderNegro = "shaders/shadernegro.frag";
static const char* vShaderMorado = "shaders/shadermorado.vert";
static const char* fShaderMorado = "shaders/shadermorado.frag";
static const char* vShaderMagenta = "shaders/shadermagenta.vert";
static const char* fShaderMagenta = "shaders/shadermagenta.frag";

float angulo = 0.0f;

//color café/marrón en RGB : 0.478, 0.255, 0.067

using std::vector;


//Pirámide triangular regular
void CreaPiramide()
{
    unsigned int indices[] = {
        0,1,2,
        1,3,2,
        3,0,2,
        1,0,3
    };

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,   //0
         0.5f, -0.5f, 0.0f,   //1
         0.0f,  0.5f,-0.25f,  //2
         0.0f, -0.5f,-0.5f    //3
    };

    Mesh* piramidetriangular = new Mesh();
    piramidetriangular->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(piramidetriangular);
}


//Pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
    unsigned int piramidecuadrangular_indices[] = {
        0,3,4,
        3,2,4,
        2,1,4,
        1,0,4,
        0,1,2,
        0,2,4
    };

    GLfloat piramidecuadrangular_vertices[] = {
         0.5f,-0.5f, 0.5f,
         0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f,
         0.0f, 0.5f, 0.0f
    };

    Mesh* piramide = new Mesh();
    piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
    meshList.push_back(piramide);
}


//Cubo
void CrearCubo()
{
    unsigned int cubo_indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    GLfloat cubo_vertices[] = {
        // front
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // back
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
    meshList.push_back(cubo);
}


void AgregarCuboColor(glm::vec3 color)
{
    GLfloat vertices[] = {
        // frente
        -0.5f,-0.5f, 0.5f,  color.r,color.g,color.b,
         0.5f,-0.5f, 0.5f,  color.r,color.g,color.b,
         0.5f, 0.5f, 0.5f,  color.r,color.g,color.b,
        -0.5f,-0.5f, 0.5f,  color.r,color.g,color.b,
         0.5f, 0.5f, 0.5f,  color.r,color.g,color.b,
        -0.5f, 0.5f, 0.5f,  color.r,color.g,color.b,
        // derecha
         0.5f,-0.5f, 0.5f,  color.r,color.g,color.b,
         0.5f,-0.5f,-0.5f,  color.r,color.g,color.b,
         0.5f, 0.5f,-0.5f,  color.r,color.g,color.b,
         0.5f,-0.5f, 0.5f,  color.r,color.g,color.b,
         0.5f, 0.5f,-0.5f,  color.r,color.g,color.b,
         0.5f, 0.5f, 0.5f,  color.r,color.g,color.b,
         // atrás
          0.5f,-0.5f,-0.5f,  color.r,color.g,color.b,
         -0.5f,-0.5f,-0.5f,  color.r,color.g,color.b,
         -0.5f, 0.5f,-0.5f,  color.r,color.g,color.b,
          0.5f,-0.5f,-0.5f,  color.r,color.g,color.b,
         -0.5f, 0.5f,-0.5f,  color.r,color.g,color.b,
          0.5f, 0.5f,-0.5f,  color.r,color.g,color.b,
          // izquierda
          -0.5f,-0.5f,-0.5f,  color.r,color.g,color.b,
          -0.5f,-0.5f, 0.5f,  color.r,color.g,color.b,
          -0.5f, 0.5f, 0.5f,  color.r,color.g,color.b,
          -0.5f,-0.5f,-0.5f,  color.r,color.g,color.b,
          -0.5f, 0.5f, 0.5f,  color.r,color.g,color.b,
          -0.5f, 0.5f,-0.5f,  color.r,color.g,color.b,
          // abajo
          -0.5f,-0.5f,-0.5f,  color.r,color.g,color.b,
           0.5f,-0.5f,-0.5f,  color.r,color.g,color.b,
           0.5f,-0.5f, 0.5f,  color.r,color.g,color.b,
          -0.5f,-0.5f,-0.5f,  color.r,color.g,color.b,
           0.5f,-0.5f, 0.5f,  color.r,color.g,color.b,
          -0.5f,-0.5f, 0.5f,  color.r,color.g,color.b,
          // arriba
          -0.5f, 0.5f, 0.5f,  color.r,color.g,color.b,
           0.5f, 0.5f, 0.5f,  color.r,color.g,color.b,
           0.5f, 0.5f,-0.5f,  color.r,color.g,color.b,
          -0.5f, 0.5f, 0.5f,  color.r,color.g,color.b,
           0.5f, 0.5f,-0.5f,  color.r,color.g,color.b,
          -0.5f, 0.5f,-0.5f,  color.r,color.g,color.b
    };

    MeshColor* cubo = new MeshColor();
    cubo->CreateMeshColor(vertices, sizeof(vertices) / sizeof(GLfloat));
    meshColorList.push_back(cubo);
}


void AgregarPiramideTriangularColor(glm::vec3 color)
{
    GLfloat vertices[] = {
        // base triangular, z = 0
        -0.5f,-0.5f,0.0f, color.r,color.g,color.b,
         0.5f,-0.5f,0.0f, color.r,color.g,color.b,
         0.0f, 0.5f,0.0f, color.r,color.g,color.b,
         // cara 1
         -0.5f,-0.5f,0.0f, color.r,color.g,color.b,
          0.5f,-0.5f,0.0f, color.r,color.g,color.b,
          0.0f, 0.0f,0.65f, color.r,color.g,color.b,
          // cara 2
           0.5f,-0.5f,0.0f, color.r,color.g,color.b,
           0.0f, 0.5f,0.0f, color.r,color.g,color.b,
           0.0f, 0.0f,0.65f, color.r,color.g,color.b,
           // cara 3
            0.0f, 0.5f,0.0f, color.r,color.g,color.b,
           -0.5f,-0.5f,0.0f, color.r,color.g,color.b,
            0.0f, 0.0f,0.65f, color.r,color.g,color.b,
            // base
             0.0f, 0.5f,0.0f, color.r,color.g,color.b,
             0.5f,-0.5f,0.0f, color.r,color.g,color.b,
            -0.5f,-0.5f,0.0f, color.r,color.g,color.b
    };

    MeshColor* piramide = new MeshColor();
    piramide->CreateMeshColor(vertices, sizeof(vertices) / sizeof(GLfloat));
    meshColorList.push_back(piramide);
}


void AgregarTrianguloRectoColor(glm::vec3 color)
{
    GLfloat vertices[] = {
        //angulo recto abajo a la izquierda
        -0.5f,-0.5f,0.0f, color.r,color.g,color.b,
         0.5f,-0.5f,0.0f, color.r,color.g,color.b,
        -0.5f, 0.5f,0.0f, color.r,color.g,color.b
    };

    MeshColor* triangulo = new MeshColor();
    triangulo->CreateMeshColor(vertices, sizeof(vertices) / sizeof(GLfloat));
    meshColorList.push_back(triangulo);
}


void CrearLetrasyFiguras()
{
    //letra S
    GLfloat vertices_S[] = {
        //X      Y      Z      R     G     B
        //Triangulo 1
        -0.3f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.3f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 2
        -0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.3f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 3
        -0.5f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.6f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 4
        -0.5f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.6f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 5
        -0.6f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.6f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 6
        -0.6f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 7
        -0.5f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 8
        -0.5f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 9
        -0.4f, 0.3f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.3f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 10
        -0.4f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.3f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.3f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 11
        -0.3f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 12
        -0.4f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.3f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 13
        -0.6f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        //Triangulo 14
        -0.6f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.6f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.4f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f
    };
    MeshColor* letraS = new MeshColor();
    letraS->CreateMeshColor(vertices_S, sizeof(vertices_S) / sizeof(GLfloat));
    meshColorList.push_back(letraS);

    //letra H
    GLfloat vertices_H[] = {
        //X      Y      Z      R     G     B
        //Triangulo 1
        -0.1f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.0f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         //Triangulo 2
         -0.1f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         -0.1f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         //Triangulo 3
         0.0f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.0f, 0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.1f, 0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
         //Triangulo 4
         0.0f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.1f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.1f, 0.2f, 0.0f,   1.0f, 0.0f, 0.0f,
         //Triangulo 5
         0.1f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.1f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.2f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         //Triangulo 6
         0.2f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.1f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.2f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f
    };
    MeshColor* letraH = new MeshColor();
    letraH->CreateMeshColor(vertices_H, sizeof(vertices_H) / sizeof(GLfloat));
    meshColorList.push_back(letraH);

    //letra F
    GLfloat vertices_F[] = {
        //X      Y      Z      R     G     B
        //Triangulo 1
        0.4f, 0.5f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.4f, 0.0f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.5f, 0.0f, 0.0f,   0.6f, 0.2f, 0.8f,
        //Triangulo 2
        0.4f, 0.5f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.5f, 0.0f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.5f, 0.5f, 0.0f,   0.6f, 0.2f, 0.8f,
        //Triangulo 3
        0.5f, 0.5f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.5f, 0.4f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.7f, 0.5f, 0.0f,   0.6f, 0.2f, 0.8f,
        //Triangulo 4
        0.7f, 0.5f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.7f, 0.4f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.5f, 0.4f, 0.0f,   0.6f, 0.2f, 0.8f,
        //Triangulo 5
        0.5f, 0.3f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.7f, 0.3f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.5f, 0.2f, 0.0f,   0.6f, 0.2f, 0.8f,
        //Triangulo 6
        0.5f, 0.2f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.7f, 0.2f, 0.0f,   0.6f, 0.2f, 0.8f,
        0.7f, 0.3f, 0.0f,   0.6f, 0.2f, 0.8f
    };
    MeshColor* letraF = new MeshColor();
    letraF->CreateMeshColor(vertices_F, sizeof(vertices_F) / sizeof(GLfloat));
    meshColorList.push_back(letraF);

    AgregarCuboColor(glm::vec3(0.0f, 0.0f, 1.0f));                 //[3] cubo azul
    AgregarCuboColor(glm::vec3(0.478f, 0.255f, 0.067f));           //[4] cubo cafe
    AgregarCuboColor(glm::vec3(0.0f, 0.0f, 0.0f));                 //[5] cubo negro
    AgregarPiramideTriangularColor(glm::vec3(1.0f, 1.0f, 0.0f));   //[6] piramide amarilla
    AgregarPiramideTriangularColor(glm::vec3(0.0f, 1.0f, 0.0f));   //[7] piramide verde
    AgregarPiramideTriangularColor(glm::vec3(1.0f, 0.0f, 0.0f));   //[8] piramide roja
    AgregarPiramideTriangularColor(glm::vec3(0.6f, 0.2f, 0.8f));   //[9] piramide morada
    AgregarTrianguloRectoColor(glm::vec3(1.0f, 1.0f, 0.0f));       //[10] esquina amarilla
    AgregarTrianguloRectoColor(glm::vec3(1.0f, 0.0f, 0.0f));       //[11] esquina roja
    AgregarTrianguloRectoColor(glm::vec3(0.0f, 1.0f, 0.0f));       //[12] esquina verde
    AgregarTrianguloRectoColor(glm::vec3(0.6f, 0.2f, 0.8f));       //[13] esquina morada
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);//shaderList[0]

    Shader* shader2 = new Shader();
    shader2->CreateFromFiles(vShaderColor, fShaderColor);
    shaderList.push_back(*shader2);//shaderList[1]

    Shader* shaderRojo = new Shader();
    shaderRojo->CreateFromFiles(vShaderRojo, fShaderRojo);
    shaderList.push_back(*shaderRojo);//shaderList[2]

    Shader* shaderAmarillo = new Shader();
    shaderAmarillo->CreateFromFiles(vShaderAmarillo, fShaderAmarillo);
    shaderList.push_back(*shaderAmarillo);//shaderList[3]

    Shader* shaderVerde = new Shader();
    shaderVerde->CreateFromFiles(vShaderVerde, fShaderVerde);
    shaderList.push_back(*shaderVerde);//shaderList[4]

    Shader* shaderAzul = new Shader();
    shaderAzul->CreateFromFiles(vShaderAzul, fShaderAzul);
    shaderList.push_back(*shaderAzul);//shaderList[5]

    Shader* shaderCafe = new Shader();
    shaderCafe->CreateFromFiles(vShaderCafe, fShaderCafe);
    shaderList.push_back(*shaderCafe);//shaderList[6]

    Shader* shaderNegro = new Shader();
    shaderNegro->CreateFromFiles(vShaderNegro, fShaderNegro);
    shaderList.push_back(*shaderNegro);//shaderList[7]

    Shader* shaderMorado = new Shader();
    shaderMorado->CreateFromFiles(vShaderMorado, fShaderMorado);
    shaderList.push_back(*shaderMorado);//shaderList[8]

    Shader* shaderMagenta = new Shader();
    shaderMagenta->CreateFromFiles(vShaderMagenta, fShaderMagenta);
    shaderList.push_back(*shaderMagenta);//shaderList[9]
}


int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    glEnable(GL_DEPTH_TEST);

    CreaPiramide();//índice 0 en MeshList
    CrearCubo();//índice 1 en MeshList
    CrearPiramideCuadrangular();//índice 2 en MeshList
    CrearLetrasyFiguras();//usa MeshColor, índices en MeshColorList
    CreateShaders();

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    //Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
    //glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 projection = glm::perspective(glm::radians(55.0f), (float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight(), 0.1f, 100.0f);

    //Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
    glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

    //s todo lo que se crea una sola vez (ventanas, mallas, shaders, matriz de porportion si la cámara no se meuve) va afuera dle while.
    //s lo que va adentro del main es lo que cambia da frame, 
    //Loop mientras no se cierra la ventana
    while (!mainWindow.getShouldClose())
    {
        //Recibir eventos del usuario
        glfwPollEvents();
        //Limpiar la ventana
        //s este es negro glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClearColor(0.9608f, 0.0f, 0.539f, 1.0f);   //rosa mexicano
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

        /*
        //Letra S, shader azul
        shaderList[5].useShader();
        uniformModel = shaderList[5].getModelLocation();
        uniformProjection = shaderList[5].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.09f, -0.42f, -2.0f));
        model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[0]->RenderMeshColor();

        //Letra H, shader rojo
        shaderList[2].useShader();
        uniformModel = shaderList[2].getModelLocation();
        uniformProjection = shaderList[2].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.09f, -0.42f, -2.0f));
        model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[1]->RenderMeshColor();

        //Letra F, shader morado
        shaderList[8].useShader();
        uniformModel = shaderList[8].getModelLocation();
        uniformProjection = shaderList[8].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.09f, -0.42f, -2.0f));
        model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[2]->RenderMeshColor();
        */

        

        //Piso
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -0.35f, -2.5f));
        model = glm::scale(model, glm::vec3(2.6f, 0.15f, 0.35f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[5]->RenderMeshColor();

        //Primera figura
        //Poste izquierdo
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.75f, 0.0275f, -2.0f));
        model = glm::scale(model, glm::vec3(0.045f, 0.605f, 0.28f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[4]->RenderMeshColor();

        //Poste derecho
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.57f, 0.0275f, -2.0f));
        model = glm::scale(model, glm::vec3(0.045f, 0.605f, 0.28f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[4]->RenderMeshColor();

        //Piramide amarilla
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.66f, 0.175f, -2.0f));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[6]->RenderMeshColor();

        //Piramide roja
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.66f, -0.005f, -2.0f));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[8]->RenderMeshColor();

        //Piramide verde
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.66f, -0.185f, -2.0f));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[7]->RenderMeshColor();

        //Segunda figura

        //Rombo azul
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -0.0204f, -2.0f));
        model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.36f, 0.36f, 0.06f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[3]->RenderMeshColor();

        //Rombo cafe
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -0.0204f, -1.85f));
        model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.06f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[4]->RenderMeshColor();

        //Esquina amarilla
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.1273f, 0.1069f, -2.0f));
        model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.2546f, 0.2546f, 0.2546f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[10]->RenderMeshColor();

        //Esquina roja
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.1273f, 0.1069f, -2.0f));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.2546f, 0.2546f, 0.2546f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[11]->RenderMeshColor();

        //Esquina morada
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.1273f, -0.1477f, -2.0f));
        model = glm::scale(model, glm::vec3(0.2546f, 0.2546f, 0.2546f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[13]->RenderMeshColor();

        //Esquina verde
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.1273f, -0.1477f, -2.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.2546f, 0.2546f, 0.2546f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[12]->RenderMeshColor();

        //Tercera figura

        //Piramide morada
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.70f, 0.025f, -2.0f));
        model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[9]->RenderMeshColor();

        //Piramide verde
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.60f, -0.175f, -2.0f));
        model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[7]->RenderMeshColor();

        //Piramide roja
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.80f, -0.175f, -2.0f));
        model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[8]->RenderMeshColor();

        //Piramide amarilla
        shaderList[1].useShader();
        uniformModel = shaderList[1].getModelLocation();
        uniformProjection = shaderList[1].getProjectLocation();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.70f, -0.175f, -2.0f));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshColorList[6]->RenderMeshColor();
        


        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
