#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <stdlib.h> //para usar rand() y srand().
#include <time.h> //para usar time(NULL)
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f, 0.41f, 0.71f, 1.0f);         \n\
}"; //color rosa 



void CrearFiguras()
{

	GLfloat vertices[] = {
		//Letra S
			//Triangulo 1
			-0.3f, 0.5f, 0.0f,
			-0.3f, 0.4f, 0.0f,
			-0.5f, 0.4f, 0.0f,
			//triangulo 2
			-0.5f, 0.5f, 0.0f,
			-0.5f, 0.4f, 0.0f,
			-0.3f, 0.5f, 0.0f,
			//Triangulo 3
			-0.5f, 0.4f, 0.0f,
			-0.6f, 0.4f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			//Triangulo 4
			-0.5f, 0.4f, 0.0f,
			-0.5f, 0.3f, 0.0f,
			-0.6f, 0.4f, 0.0f,
			//Triangulo 5
			-0.6f, 0.3f, 0.0f,
			-0.5f, 0.3f, 0.0f,
			-0.6f, 0.4f, 0.0f,
			//Triangulo 6
			-0.6f, 0.3f, 0.0f,
			-0.5f, 0.3f, 0.0f,
			-0.5f, 0.2f, 0.0f,
			//triangulo 7
			-0.5f, 0.2f, 0.0f,
			-0.5f, 0.3f, 0.0f,
			-0.4f, 0.2f, 0.0f,
			//Triangulo 8
			-0.5f, 0.3f, 0.0f,
			-0.4f, 0.3f, 0.0f,
			-0.4f, 0.2f, 0.0f,
			//Triangulo 9
			-0.4f, 0.3f, 0.0f,
			-0.4f, 0.2f, 0.0f,
			-0.3f, 0.2f, 0.0f,
			//Triangulo 10
			-0.4f, 0.2f, 0.0f,
			-0.3f, 0.2f, 0.0f,
			-0.3f, 0.1f, 0.0f,
			//Triangulo 11
			-0.3f, 0.1f, 0.0f,
			-0.4f, 0.1f, 0.0f,
			-0.4f, 0.2f, 0.0f,
			//Triangulo 12
			-0.4f, 0.1f, 0.0f,
			-0.3f, 0.1f, 0.0f,
			-0.4f, 0.0f, 0.0f,
			//Triangulo 13
			-0.6f, 0.0f, 0.0f,
			-0.4f, 0.1f, 0.0f,
			-0.4f, 0.0f, 0.0f,
			//Triangulo 14
			-0.6f, 0.1f, 0.0f,
			-0.6f, 0.0f, 0.0f,
			-0.4f, 0.1f, 0.0f,

		//Letra H
			//Triangulo 1
			-0.1f, 0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
			0.0f, 0.0f, 0.0f,
			//triangulo 2
			-0.1f, 0.0f, 0.0f,
			-0.1f, 0.5f, 0.0f,
			0.0f, 0.0f, 0.0f,
			//Triangulo 3
			0.0f, 0.3f, 0.0f,
			0.0f, 0.2f, 0.0f,
			0.1f, 0.2f, 0.0f,
			//Triangulo 4
			0.0f, 0.3f, 0.0f,
			0.1f, 0.3f, 0.0f,
			0.1f, 0.2f, 0.0f,
			//Triangulo 5
			0.1f, 0.5f, 0.0f,
			0.1f, 0.0f, 0.0f,
			0.2f, 0.0f, 0.0f,
			//Triangulo 6
			0.2f, 0.0f, 0.0f,
			0.1f, 0.5f, 0.0f,
			0.2f, 0.5f, 0.0f,

		//Letra F
			//Triangulo 1
			0.4f, 0.5f, 0.0f,
			0.4f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.0f,
			//triangulo 2
			0.4f, 0.5f, 0.0f,
			0.5f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.0f,
			//Triangulo 3
			0.5f, 0.5f, 0.0f,
			0.5f, 0.4f, 0.0f,
			0.7f, 0.5f, 0.0f,
			//Triangulo 4
			0.7f, 0.5f, 0.0f,
			0.7f, 0.4f, 0.0f,
			0.5f, 0.4f, 0.0f,
			//Triangulo 5
			0.5f, 0.3f, 0.0f,
			0.7f, 0.3f, 0.0f,
			0.5f, 0.2f, 0.0f,
			//Triangulo 6
			0.5f, 0.2f, 0.0f,
			0.7f, 0.2f, 0.0f,
			0.7f, 0.3f, 0.0f
	};

	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	//verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}
}

int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	CrearFiguras();
	CompileShaders();

	printf("Version de Opengl: %s \n", glGetString(GL_VERSION));
	printf("Marca: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("Shaders: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	GLfloat r = 0.0f;//es buena práctica poner la f para que no te lo convierta luego en double
	GLfloat g = 0.0f;
	GLfloat b = 0.0f;
	//se empieza con el color negro

	srand((unsigned int)time(NULL)); //la semilla es (unsigned int)time(NULL)
									//si haces srand(5) y luego llamas rand() tres veces, obtendrás SIEMPRE los mismos tres números. Por eso creamos una semilla con el tiempo
									// por eso usamos time(NULL) por que nunca se queda igual, a lo largo de la ejecución cambia, nunca se queda por ejemplo srand(7), justamente queremos una semilla que no es estática
									// en srand(aquí solo puede ser un entero unsigned) 
									// como su nombre lo dice, unsigned no guarda negativos
									// dato curioso, time(NULL) devuelve el número de segundos transcurridos desde el 1 de enero de 1970
	double tiempo1 = 0.0;//empezamos desde el tiempo 0.0

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		double tiempoActual = glfwGetTime();//en cada ciclo de reloj tomamos el tiempo actual
		if (tiempoActual - tiempo1 >= 2.0)//han transcurrido 2 segundos
		{
			r = (GLfloat)(rand() % 1000) / 999.0f; //el rand() me va a devolver un número entre 0 y un RAND_MAX que depende del compilador, por ejemplo RAND_MAX=32767
			g = (GLfloat)(rand() % 256) / 255.0f;
			b = (GLfloat)(rand() % 589) / 588.0f;
								// las 4 formas funcionan
			tiempo1 = tiempoActual;
		}
		//Limpiar la ventana
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//No es lo mismo 0.234f, 0.167f, 0.087 que 0.2f, 0.1f, 0.0f, la tarjeta gráfica si procesa con presición esos datos, en la mayoría de monitores estándar con bufer de color de 8 bits, te permite 256 niveles = del 0 al 255. Así que por ejemplo ese 0.234 se multiplica 255 y da 59.67->60, no es lo mismo que 0.2x255->51, la diferencia es de 10 tonalidades.
		//Más allá de 4 cifras no tiene caso, por ejemplo 0.2398
		//Puede valer desde 0.0f hasta 0.1f, por ejemplo es válido 0.987

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 78);//78 vertices, 3 vertices por cada triángulo y son 26 triángulos
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow); 
	}


	return 0;
}