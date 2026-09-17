//práctica 3: Modelado Geométrico y Cámara Sintética.
//Ejercicio 1: cohete            <-- ACTIVO
//Ejercicio 2: 8 pirámides       <-- comentado abajo
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z

using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

//colores que se mandan al uniform color
glm::vec3 azul = glm::vec3(0.1f, 0.15f, 0.9f);
glm::vec3 verde = glm::vec3(0.1f, 0.8f, 0.15f);
glm::vec3 rojo = glm::vec3(0.85f, 0.1f, 0.1f);
glm::vec3 amarillo = glm::vec3(1.0f, 0.8f, 0.1f);
glm::vec3 morado = glm::vec3(0.65f, 0.2f, 0.95f);
glm::vec3 negro = glm::vec3(0.0f, 0.0f, 0.0f);

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

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidet = new Mesh();
	piramidet->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(piramidet);

}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,//frontal
		3,2,4,//izquierda
		2,1,4,//trasera
		1,0,4,//derecha
		0,1,2,//abajo1
		0,2,3//abajo2

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramidec = new Mesh();
	piramidec->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramidec);
}

//La misma pirámide cuadrangular pero partida en sus 5 caras.
//Se separa porque el shader manda un solo color por cada RenderMesh(),
//así cada cara se dibuja aparte y puede llevar su propio color.
void CrearPiramideCuadrangularPorCaras()
{
	unsigned int tri_idx[] = { 0, 1, 2 };

	// 1. CARA FRONTAL (Verde)
	GLfloat v_front[] = { 0.5f,-0.5f,0.5f,  -0.5f,-0.5f,0.5f,  0.0f,0.5f,0.0f };
	Mesh* mFront = new Mesh(); mFront->CreateMesh(v_front, tri_idx, 9, 3);
	meshList.push_back(mFront);

	// 2. CARA IZQUIERDA (Morada)
	GLfloat v_left[] = { -0.5f,-0.5f,0.5f,  -0.5f,-0.5f,-0.5f, 0.0f,0.5f,0.0f };
	Mesh* mLeft = new Mesh(); mLeft->CreateMesh(v_left, tri_idx, 9, 3);
	meshList.push_back(mLeft);

	// 3. CARA TRASERA (Amarilla)
	GLfloat v_back[] = { -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f, 0.0f,0.5f,0.0f };
	Mesh* mBack = new Mesh(); mBack->CreateMesh(v_back, tri_idx, 9, 3);
	meshList.push_back(mBack);

	// 4. CARA DERECHA (Roja)
	GLfloat v_right[] = { 0.5f,-0.5f,-0.5f,  0.5f,-0.5f,0.5f,  0.0f,0.5f,0.0f };
	Mesh* mRight = new Mesh(); mRight->CreateMesh(v_right, tri_idx, 9, 3);
	meshList.push_back(mRight);

	// 5. BASE CUADRADA (Azul)
	unsigned int quad_idx[] = { 0, 1, 2, 0, 2, 3 };
	GLfloat v_base[] = { 0.5f,-0.5f,0.5f, 0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f,0.5f };
	Mesh* mBase = new Mesh(); mBase->CreateMesh(v_base, quad_idx, 12, 6);
	meshList.push_back(mBase);
}

/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	//vertices.size() cuenta floats, y cada vértice son 3 floats,
	//por eso se divide entre 3 para no pedir más vértices de los que hay
	for (i = 0; i < vertices.size() / 3; i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);

	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}

// Dibuja una pirámide cuadrangular completa con sus 5 caras de colores.
// Recibe ya armada la matriz de modelo de esa pirámide.
void DibujarPiramideMulticolor(glm::mat4 modelMatrix, GLuint uniformModel, GLuint uniformColor)
{
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glUniform3fv(uniformColor, 1, glm::value_ptr(verde));
	meshList[5]->RenderMesh(); // Frontal

	glUniform3fv(uniformColor, 1, glm::value_ptr(morado));
	meshList[6]->RenderMesh(); // Izquierda

	glUniform3fv(uniformColor, 1, glm::value_ptr(amarillo));
	meshList[7]->RenderMesh(); // Trasera

	glUniform3fv(uniformColor, 1, glm::value_ptr(rojo));
	meshList[8]->RenderMesh(); // Derecha

	glUniform3fv(uniformColor, 1, glm::value_ptr(azul));
	meshList[9]->RenderMesh(); // Base
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(20, 1.0f);//índice 2 en MeshList
	CrearCono(20, 1.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CrearPiramideCuadrangularPorCaras();//índices 5 al 9 en MeshList
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.3f, 0.3f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);//inicializar Color para enviar a variable Uniform;

	//Medidas del cohete, todo se mide sobre el eje Y
	//El cuerpo es el cubo estirado: escala (0.8, 2.0, 0.8), o sea va de y=-1.0 a y=+1.0
	//De ahí salen las demás piezas
	float anchoCuerpo = 0.8f;
	float altoCuerpo = 2.0f;

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//FONDO Y PISO, son los mismos para los dos ejercicios
		//Limpiar la ventana
		glClearColor(0.9608f, 0.0f, 0.539f, 1.0f); //rosa mexicano
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		//PISO NEGRO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.8f, -5.0f));
		model = glm::scale(model, glm::vec3(14.0f, 0.1f, 14.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = negro;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		/*
		// EJERCICIO 1: COHETE

		//Matriz base del cohete
		//Aquí va la traslación al frente de la cámara y la rotación de las teclas E, R, T, para que todas las piezas del cohete se muevan juntas
		glm::mat4 base = glm::mat4(1.0);
		base = glm::translate(base, glm::vec3(0.0f, -0.2f, -6.0f));
		base = glm::rotate(base, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		base = glm::rotate(base, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		base = glm::rotate(base, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		//CUERPO: cubo estirado en el eje Y
		model = base;
		model = glm::scale(model, glm::vec3(anchoCuerpo, altoCuerpo, anchoCuerpo));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = azul;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//PUNTA: pirámide cuadrangular
		//Su base local está en y=-0.5, así que el centro sube media altura para que la base caiga justo en la tapa del cuerpo (y=1.0)
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.85f, 1.0f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = verde;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();

		//ESFERA de hasta arriba
		//La punta de la pirámide queda en y=2.0, la esfera se encima un poco
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, 2.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = rojo;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//VENTANA: cilindro acostado sobre la cara del frente
		//El cilindro nace parado en Y, se gira 90 grados en X para que su eje apunte en Z
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, 0.35f, 0.50f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.28f, 0.14f, 0.28f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = amarillo;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//ALETA DERECHA: pirámide triangular
		//Gira -90 grados en Z para que la punta apunte hacia +X
		model = base;
		model = glm::translate(model, glm::vec3(0.55f, -0.45f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 1.0f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = morado;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		//ALETA IZQUIERDA: misma pirámide, girada al otro lado
		model = base;
		model = glm::translate(model, glm::vec3(-0.55f, -0.45f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 1.0f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = morado;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		//MOTOR: cono de abajo
		//El cono nace con la punta hacia arriba, se gira 180 grados en X para voltearlo
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, -1.25f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.42f, 0.5f, 0.43f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = rojo;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();

		*/
		
		// EJERCICIO 2: FIGURA DE 8 PIRAMIDES CUADRANGULARES

		//MATRIZ BASE CON TECLAS DE ROTACION (E, R, T)
		//De aquí parten las 8 pirámides, así todas giran juntas
		glm::mat4 base = glm::mat4(1.0);
		base = glm::translate(base, glm::vec3(0.0f, 0.0f, -4.0f));
		base = glm::rotate(base, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		base = glm::rotate(base, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		base = glm::rotate(base, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		//Cada pirámide se coloca con su translate, rotate y scale, y luego se
		//dibujan sus 5 caras una por una, cambiando el color antes de cada una.
		//meshList[5] frontal, [6] izquierda, [7] trasera, [8] derecha, [9] base

		//Pirámide 1 (centro izquierda, muestra la cara verde)
		model = glm::translate(base, glm::vec3(-0.3f, 0.0f, 0.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.0f));
		DibujarPiramideMulticolor(model, uniformModel, uniformColor);

		//Pirámide 2 (centro derecha, rotada para mostrar la cara roja)
		model = glm::translate(base, glm::vec3(0.3f, 0.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.0f));
		DibujarPiramideMulticolor(model, uniformModel, uniformColor);

		//Pirámide 3 (arriba centro, muestra morado y amarillo)
		model = glm::translate(base, glm::vec3(0.0f, 0.3f, 0.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -225.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.0f));
		DibujarPiramideMulticolor(model, uniformModel, uniformColor);

		//Pirámide 4 (abajo centro, muestra morado y amarillo)
		model = glm::translate(base, glm::vec3(0.0f, -0.3f, 0.0f));
		model = glm::rotate(model, 135.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.0f));
		DibujarPiramideMulticolor(model, uniformModel, uniformColor);

		//Pirámide 5 (extremo izquierdo, base pegada con la 1)
		model = glm::translate(base, glm::vec3(-0.9f, 0.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.0f));
		DibujarPiramideMulticolor(model, uniformModel, uniformColor);

		//Pirámide 6 (extremo derecho, base pegada con la 2)
		model = glm::translate(base, glm::vec3(0.9f, 0.0f, 0.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.0f));
		DibujarPiramideMulticolor(model, uniformModel, uniformColor);

		//Pirámide 7 (extremo superior, base pegada con la 3)
		model = glm::translate(base, glm::vec3(0.0f, 0.9f, 0.0f));
		model = glm::rotate(model, 135.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.0f));
		DibujarPiramideMulticolor(model, uniformModel, uniformColor);

		//Pirámide 8 (extremo inferior, base pegada con la 4)
		model = glm::translate(base, glm::vec3(0.0f, -0.9f, 0.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 135.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.0f));
		DibujarPiramideMulticolor(model, uniformModel, uniformColor);
		
		
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}