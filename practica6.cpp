/*
Práctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz

std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture holocronTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Holocron_M;
Model Avion_M;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};


	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


/*void CrearHolocron()
{
	unsigned int holocron_indices[] = {

	12, 1, 0,
	 12, 4, 15,
	 14, 9, 2,
	 2, 12, 15,
	 8,  3,  4,
	 15,  4,  3,
	 3, 6, 14,
	 4, 5,  8,
	 6, 7, 13,
	 9, 10, 11,
	 11, 2, 9,
	 10, 7, 5,
	 13, 14, 6,
	 2, 3, 14,
	 2, 15, 3,
	 11, 12, 2,
	 13, 9, 14,
	 8, 6, 3,
	 12, 0, 4,
	 12, 11, 1,
	 4, 0, 5,
	 6, 8, 7,
	 9, 13, 10,
	 5, 0, 1,
	 1, 11, 10,
	 10, 13, 7,
	 7, 8, 5,
	 5, 1, 10

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat holocron_vertices[] = {
		// front
		//x					y			z			S		T		NX		NY		NZ
		 -2.517274,		-2.579095,		0.002115,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//0
		-1.185055,		-2.530499,		-1.198261,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//1
		0.049649,		2.440128,		-2.417217,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//2
		0.051152,		2.399781,		2.545043,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//3
		-2.646786,		-0.139047,		2.493757,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//4
		-1.328995,		-2.578386,		1.297838,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//5
		2.593003,		-0.085222,		2.505755,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//6
		1.385028,		-2.549476,		1.308677,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//7
		-0.140696,		-2.601934,		2.558561,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//8
		2.534944,		-0.061146,		-2.580280,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//9
		1.344182,		-2.542571,		-1.294387,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//10
		0.081432,		-2.513204,		-2.426005,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//11
		-2.453980,		 -0.063388,		-2.477451,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//12
		2.632201,		-2.553892,		0.007226,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//13
		2.521206,		2.448241,		0.063388,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//14
		-2.451372,		2.421759,		0.077538,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//15
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 120, 84);
	meshListModel.push_back(holocron);

}*/

void CrearHolocron()
{
	//indices: cada cara usa SUS vertices (ver el arreglo de abajo), los triangulos son los mismos que dio el profe
	unsigned int holocron_indices[] = {
		// cara 1: cuadrado de arriba (+Y)
		2, 0, 1,
		2, 3, 0,
		// cara 2: cuadrado de enfrente (+Z)
		4, 6, 7,
		4, 5, 6,
		// cara 3: cuadrado derecho (+X)
		8, 10, 11,
		8, 9, 10,
		// cara 4: cuadrado de atras (-Z)
		12, 14, 15,
		12, 13, 14,
		// cara 5: cuadrado izquierdo (-X)
		19, 17, 18,
		19, 16, 17,
		// cara 6: cuadrado de abajo (-Y), trae 8 vertices
		25, 27, 21,
		21, 22, 23,
		23, 24, 25,
		25, 26, 27,
		27, 20, 21,
		21, 23, 25,
		// cara 7: triangulo arriba-enfrente-derecha
		30, 28, 29,
		// cara 8: triangulo arriba-enfrente-izquierda
		33, 31, 32,
		// cara 9: triangulo arriba-atras-derecha
		36, 34, 35,
		// cara 10: triangulo arriba-atras-izquierda
		39, 37, 38,
		// cara 11: triangulo abajo-enfrente-derecha
		43, 41, 42,
		43, 40, 41,
		// cara 12: triangulo abajo-enfrente-izquierda
		47, 45, 46,
		47, 44, 45,
		// cara 13: triangulo abajo-atras-derecha
		51, 49, 50,
		51, 48, 49,
		// cara 14: triangulo abajo-atras-izquierda
		55, 53, 54,
		55, 52, 53,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	//OJO: como en el dado, cada cara tiene sus propias copias de los vertices.
	//un mismo punto del holocron cae en lugares distintos de la imagen segun la cara, por eso se repite
	//la imagen se parte en una cuadricula de 4x4, cada casilla mide 0.25 en S y 0.25 en T
	//se deja 0.01 de margen para que no se cuele el color de la casilla de al lado
	//T=0 es la orilla de ABAJO de la imagen (stbi la voltea al cargarla)
	//las normales van hacia adentro, igual que en el dado y en Model.cpp (ahi las multiplican por -1)
	GLfloat holocron_vertices[] = {
		// front
		//x				y				z				S		T			NX		NY		NZ
		// cara 1: cuadrado de arriba (+Y) -> casilla columna 1, renglon 1 (contando desde arriba a la izquierda)
		0.051152f,	2.399781f,	2.545043f,		0.01f,	0.76f,		0.01f,	-1.0f,	-0.01f,	//0 (antes era el 3)
		2.521206f,	2.448241f,	0.063388f,		0.24f,	0.76f,		0.01f,	-1.0f,	-0.01f,	//1 (antes era el 14)
		0.049649f,	2.440128f,	-2.417217f,		0.24f,	0.99f,		0.01f,	-1.0f,	-0.01f,	//2 (antes era el 2)
		-2.451372f,	2.421759f,	0.077538f,		0.01f,	0.99f,		0.01f,	-1.0f,	-0.01f,	//3 (antes era el 15)
		// cara 2: cuadrado de enfrente (+Z) -> casilla columna 2, renglon 1 (contando desde arriba a la izquierda)
		-0.140696f,	-2.601934f,	2.558561f,		0.26f,	0.76f,		0.0f,	0.0f,	-1.0f,	//4 (antes era el 8)
		2.593003f,	-0.085222f,	2.505755f,		0.49f,	0.76f,		0.0f,	0.0f,	-1.0f,	//5 (antes era el 6)
		0.051152f,	2.399781f,	2.545043f,		0.49f,	0.99f,		0.0f,	0.0f,	-1.0f,	//6 (antes era el 3)
		-2.646786f,	-0.139047f,	2.493757f,		0.26f,	0.99f,		0.0f,	0.0f,	-1.0f,	//7 (antes era el 4)
		// cara 3: cuadrado derecho (+X) -> casilla columna 3, renglon 1 (contando desde arriba a la izquierda)
		2.632201f,	-2.553892f,	0.007226f,		0.51f,	0.76f,		-1.0f,	-0.02f,	0.01f,	//8 (antes era el 13)
		2.534944f,	-0.061146f,	-2.580280f,		0.74f,	0.76f,		-1.0f,	-0.02f,	0.01f,	//9 (antes era el 9)
		2.521206f,	2.448241f,	0.063388f,		0.74f,	0.99f,		-1.0f,	-0.02f,	0.01f,	//10 (antes era el 14)
		2.593003f,	-0.085222f,	2.505755f,		0.51f,	0.99f,		-1.0f,	-0.02f,	0.01f,	//11 (antes era el 6)
		// cara 4: cuadrado de atras (-Z) -> casilla columna 4, renglon 1 (contando desde arriba a la izquierda)
		0.081432f,	-2.513204f,	-2.426005f,		0.76f,	0.76f,		0.02f,	0.0f,	1.0f,	//12 (antes era el 11)
		-2.453980f,	-0.063388f,	-2.477451f,		0.99f,	0.76f,		0.02f,	0.0f,	1.0f,	//13 (antes era el 12)
		0.049649f,	2.440128f,	-2.417217f,		0.99f,	0.99f,		0.02f,	0.0f,	1.0f,	//14 (antes era el 2)
		2.534944f,	-0.061146f,	-2.580280f,		0.76f,	0.99f,		0.02f,	0.0f,	1.0f,	//15 (antes era el 9)
		// cara 5: cuadrado izquierdo (-X) -> casilla columna 1, renglon 2 (contando desde arriba a la izquierda)
		-2.517274f,	-2.579095f,	0.002115f,		0.01f,	0.51f,		1.0f,	-0.01f,	0.04f,	//16 (antes era el 0)
		-2.646786f,	-0.139047f,	2.493757f,		0.24f,	0.51f,		1.0f,	-0.01f,	0.04f,	//17 (antes era el 4)
		-2.451372f,	2.421759f,	0.077538f,		0.24f,	0.74f,		1.0f,	-0.01f,	0.04f,	//18 (antes era el 15)
		-2.453980f,	-0.063388f,	-2.477451f,		0.01f,	0.74f,		1.0f,	-0.01f,	0.04f,	//19 (antes era el 12)
		// cara 6: cuadrado de abajo (-Y), trae 8 vertices -> casilla columna 2, renglon 2 (contando desde arriba a la izquierda)
		-0.140696f,	-2.601934f,	2.558561f,		0.26f,	0.51f,		0.0f,	1.0f,	0.02f,	//20 (antes era el 8)
		-1.328995f,	-2.578386f,	1.297838f,		0.375f,	0.51f,		0.0f,	1.0f,	0.02f,	//21 (antes era el 5)
		-2.517274f,	-2.579095f,	0.002115f,		0.49f,	0.51f,		0.0f,	1.0f,	0.02f,	//22 (antes era el 0)
		-1.185055f,	-2.530499f,	-1.198261f,		0.49f,	0.625f,		0.0f,	1.0f,	0.02f,	//23 (antes era el 1)
		0.081432f,	-2.513204f,	-2.426005f,		0.49f,	0.74f,		0.0f,	1.0f,	0.02f,	//24 (antes era el 11)
		1.344182f,	-2.542571f,	-1.294387f,		0.375f,	0.74f,		0.0f,	1.0f,	0.02f,	//25 (antes era el 10)
		2.632201f,	-2.553892f,	0.007226f,		0.26f,	0.74f,		0.0f,	1.0f,	0.02f,	//26 (antes era el 13)
		1.385028f,	-2.549476f,	1.308677f,		0.26f,	0.625f,		0.0f,	1.0f,	0.02f,	//27 (antes era el 7)
		// cara 7: triangulo arriba-enfrente-derecha -> casilla columna 3, renglon 2 (contando desde arriba a la izquierda)
		2.593003f,	-0.085222f,	2.505755f,		0.625f,	0.525f,		-0.57f,	-0.58f,	-0.58f,	//28 (antes era el 6)
		2.521206f,	2.448241f,	0.063388f,		0.74f,	0.725f,		-0.57f,	-0.58f,	-0.58f,	//29 (antes era el 14)
		0.051152f,	2.399781f,	2.545043f,		0.51f,	0.725f,		-0.57f,	-0.58f,	-0.58f,	//30 (antes era el 3)
		// cara 8: triangulo arriba-enfrente-izquierda -> casilla columna 4, renglon 2 (contando desde arriba a la izquierda)
		-2.646786f,	-0.139047f,	2.493757f,		0.875f,	0.525f,		0.56f,	-0.59f,	-0.58f,	//31 (antes era el 4)
		0.051152f,	2.399781f,	2.545043f,		0.99f,	0.725f,		0.56f,	-0.59f,	-0.58f,	//32 (antes era el 3)
		-2.451372f,	2.421759f,	0.077538f,		0.76f,	0.725f,		0.56f,	-0.59f,	-0.58f,	//33 (antes era el 15)
		// cara 9: triangulo arriba-atras-derecha -> casilla columna 1, renglon 3 (contando desde arriba a la izquierda)
		2.534944f,	-0.061146f,	-2.580280f,		0.125f,	0.275f,		-0.57f,	-0.6f,	0.57f,	//34 (antes era el 9)
		0.049649f,	2.440128f,	-2.417217f,		0.24f,	0.475f,		-0.57f,	-0.6f,	0.57f,	//35 (antes era el 2)
		2.521206f,	2.448241f,	0.063388f,		0.01f,	0.475f,		-0.57f,	-0.6f,	0.57f,	//36 (antes era el 14)
		// cara 10: triangulo arriba-atras-izquierda -> casilla columna 2, renglon 3 (contando desde arriba a la izquierda)
		-2.453980f,	-0.063388f,	-2.477451f,		0.375f,	0.275f,		0.57f,	-0.59f,	0.57f,	//37 (antes era el 12)
		-2.451372f,	2.421759f,	0.077538f,		0.49f,	0.475f,		0.57f,	-0.59f,	0.57f,	//38 (antes era el 15)
		0.049649f,	2.440128f,	-2.417217f,		0.26f,	0.475f,		0.57f,	-0.59f,	0.57f,	//39 (antes era el 2)
		// cara 11: triangulo abajo-enfrente-derecha -> casilla columna 3, renglon 3 (contando desde arriba a la izquierda)
		-0.140696f,	-2.601934f,	2.558561f,		0.51f,	0.275f,		-0.56f,	0.58f,	-0.59f,	//40 (antes era el 8)
		1.385028f,	-2.549476f,	1.308677f,		0.625f,	0.275f,		-0.56f,	0.58f,	-0.59f,	//41 (antes era el 7)
		2.632201f,	-2.553892f,	0.007226f,		0.74f,	0.275f,		-0.56f,	0.58f,	-0.59f,	//42 (antes era el 13)
		2.593003f,	-0.085222f,	2.505755f,		0.625f,	0.475f,		-0.56f,	0.58f,	-0.59f,	//43 (antes era el 6)
		// cara 12: triangulo abajo-enfrente-izquierda -> casilla columna 4, renglon 3 (contando desde arriba a la izquierda)
		-2.517274f,	-2.579095f,	0.002115f,		0.76f,	0.275f,		0.59f,	0.59f,	-0.55f,	//44 (antes era el 0)
		-1.328995f,	-2.578386f,	1.297838f,		0.875f,	0.275f,		0.59f,	0.59f,	-0.55f,	//45 (antes era el 5)
		-0.140696f,	-2.601934f,	2.558561f,		0.99f,	0.275f,		0.59f,	0.59f,	-0.55f,	//46 (antes era el 8)
		-2.646786f,	-0.139047f,	2.493757f,		0.875f,	0.475f,		0.59f,	0.59f,	-0.55f,	//47 (antes era el 4)
		// cara 13: triangulo abajo-atras-derecha -> casilla columna 1, renglon 4 (contando desde arriba a la izquierda)
		2.632201f,	-2.553892f,	0.007226f,		0.01f,	0.025f,		-0.55f,	0.59f,	0.59f,	//48 (antes era el 13)
		1.344182f,	-2.542571f,	-1.294387f,		0.125f,	0.025f,		-0.55f,	0.59f,	0.59f,	//49 (antes era el 10)
		0.081432f,	-2.513204f,	-2.426005f,		0.24f,	0.025f,		-0.55f,	0.59f,	0.59f,	//50 (antes era el 11)
		2.534944f,	-0.061146f,	-2.580280f,		0.125f,	0.225f,		-0.55f,	0.59f,	0.59f,	//51 (antes era el 9)
		// cara 14: triangulo abajo-atras-izquierda -> casilla columna 2, renglon 4 (contando desde arriba a la izquierda)
		0.081432f,	-2.513204f,	-2.426005f,		0.26f,	0.025f,		0.55f,	0.58f,	0.6f,	//52 (antes era el 11)
		-1.185055f,	-2.530499f,	-1.198261f,		0.375f,	0.025f,		0.55f,	0.58f,	0.6f,	//53 (antes era el 1)
		-2.517274f,	-2.579095f,	0.002115f,		0.49f,	0.025f,		0.55f,	0.58f,	0.6f,	//54 (antes era el 0)
		-2.453980f,	-0.063388f,	-2.477451f,		0.375f,	0.225f,		0.55f,	0.58f,	0.6f,	//55 (antes era el 12)
	};

	MeshModel* holocron = new MeshModel();
	//antes decia 120 y eran 128 floats (16x8), el vertice 15 se quedaba fuera del buffer
	//ahora que lo cuente el compilador: sizeof(arreglo completo) / sizeof(un elemento)
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, sizeof(holocron_vertices) / sizeof(GLfloat), sizeof(holocron_indices) / sizeof(unsigned int));
	meshListModel.push_back(holocron);

}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado-de-numeros.png");
	dadoTexture.LoadTextureA();

	//s holocronTexture = Texture("Textures/holocronunwrap.png");
	holocronTexture = Texture("Textures/holocron_jedi.png");
	holocronTexture.LoadTextureA();

	
	Holocron_M = Model();
	//s Holocron_M.LoadModel("Models/holocron_simple.obj");
	Holocron_M.LoadModel("Models/holocron_texturizado.obj");
	Avion_M = Model();
	Avion_M.LoadModel("Models/avion_texturizado.obj");
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshListModel[2]->RenderMeshModel();


		

		//Dado de Opengl
		//Ejercicio 1: Texturizar su dado con la imagen ya optimizada por ustedes con logos de star wars
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen ya optimizada por ustedes
		
		/*
		//Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 3.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();
		*/
	



		/*Reporte de práctica :
		
		Ejercicio 1: Crear o modificar el holocron y texturizarlo por medio de código
		Ejercicio 2: Importar el modelo del holocron texturizardo en el programa de modelado
		Ejercicio 3: Importar un modelo de avión con con la textura de la cara del personaje de la imagen del previo:
		Vidrio fonrtal: OJOS
		Frente del avión: Nariz y Sonrisa
		Alas: Logos del universo del personaje
		
		*/


		//Holocrones
		//s color = glm::vec3(0.0f, 1.0f, 0.0f);//color que multiplica a la información de color de la textura
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		//s model = glm::translate(model, glm::vec3(-8.5f, 4.5f, 0.0f));
		model = glm::translate(model, glm::vec3(-4.5f, 2.5f, 4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();
		
		//s Ejercicio
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.5f, 2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Holocron_M.RenderModel();

		
		//s Ejercicio 3: avion con la cara del personaje
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.5f, 1.5f, -7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion_M.RenderModel();

	
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

//si quieres habilitar creo transparencia, tienes que activar lo siguiente: ten cuidado va a después del render

/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/