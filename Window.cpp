
#include "Window.h"

//Constructor por defecto
Window::Window()
{
	width = 800;
	height = 600;
	mainWindow = nullptr;
	bufferWidth = 0;
	bufferHeight = 0;
	//rotaciones de la práctica anterior (en esta práctica no se usan)
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	//Cada articulación guarda su ángulo en grados. Deben arrancar en 0 para que el brazo empiece
	//en la pose que dibujamos; si no se inicializan, C++ les deja basura y el brazo saldría en una pose al azar.
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	//NUEVO: ángulo de cada llanta, también arrancan en 0
	llanta1 = 0.0f;
	llanta2 = 0.0f;
	llanta3 = 0.0f;
	llanta4 = 0.0f;
	llanta5 = 0.0f;
	llanta6 = 0.0f;
	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

//Constructor con tamaño de ventana (es el que usa el main: Window(800, 600))
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	mainWindow = nullptr;
	bufferWidth = 0;
	bufferHeight = 0;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	llanta1 = 0.0f;
	llanta2 = 0.0f;
	llanta3 = 0.0f;
	llanta4 = 0.0f;
	llanta5 = 0.0f;
	llanta6 = 0.0f;
	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Fallo inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 4: Modelado Jerarquico - Rover", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//Guardamos un apuntador a ESTE objeto Window dentro de la ventana de GLFW.
	//Así las funciones static ManejaTeclado y ManejaMouse pueden llegar a sus variables (articulaciones, llantas, keys...)
	glfwSetWindowUserPointer(mainWindow, this);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fallo inicializacion de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	//recuperamos el objeto Window que guardamos con glfwSetWindowUserPointer
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//GLFW llama a esta función al presionar la tecla (PRESS), mientras se deja presionada (REPEAT)
	//y al soltarla (RELEASE). Solo sumamos grados en PRESS y REPEAT; si no, cada toque sumaría dos veces.
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		//Con Shift presionado se gira en sentido contrario
		GLfloat sentido = 1.0f;
		if (mode & GLFW_MOD_SHIFT)
		{
			sentido = -1.0f;
		}

		//rotaciones de la práctica anterior (se dejan, pero el main de esta práctica no las usa)
		if (key == GLFW_KEY_E)
		{
			theWindow->rotax += 10.0f * sentido;
		}
		if (key == GLFW_KEY_R)
		{
			theWindow->rotay += 10.0f * sentido;
		}
		if (key == GLFW_KEY_T)
		{
			theWindow->rotaz += 10.0f * sentido;
		}

		//BRAZO
		if (key == GLFW_KEY_F)
		{
			theWindow->articulacion1 += 10.0f * sentido; //hombro (EJE BB-BR1)
		}
		if (key == GLFW_KEY_G)
		{
			theWindow->articulacion2 += 10.0f * sentido; //codo (EJE BR1-BR2)
		}
		if (key == GLFW_KEY_H)
		{
			theWindow->articulacion3 += 10.0f * sentido; //muñeca (EJE BR2-PINZA)
		}
		if (key == GLFW_KEY_J)
		{
			//J abre la pinza y Shift + J la cierra; se limita de 0 a 45 grados para que los dedos no se crucen
			theWindow->articulacion4 += 5.0f * sentido;
			if (theWindow->articulacion4 > 45.0f)
			{
				theWindow->articulacion4 = 45.0f;
			}
			if (theWindow->articulacion4 < 0.0f)
			{
				theWindow->articulacion4 = 0.0f;
			}
		}
		if (key == GLFW_KEY_K)
		{
			theWindow->articulacion5 += 10.0f * sentido; //base del brazo, gira en y
		}
		if (key == GLFW_KEY_L)
		{
			theWindow->articulacion6 += 10.0f * sentido; //la pinza gira sobre su propio eje
		}

		//LLANTAS (cada una por separado). Sin Shift ruedan como si el rover avanzara hacia el frente (-x)
		if (key == GLFW_KEY_1)
		{
			theWindow->llanta1 += 10.0f * sentido; //frontal +Z
		}
		if (key == GLFW_KEY_2)
		{
			theWindow->llanta2 += 10.0f * sentido; //frontal -Z
		}
		if (key == GLFW_KEY_3)
		{
			theWindow->llanta3 += 10.0f * sentido; //en medio +Z
		}
		if (key == GLFW_KEY_4)
		{
			theWindow->llanta4 += 10.0f * sentido; //en medio -Z
		}
		if (key == GLFW_KEY_5)
		{
			theWindow->llanta5 += 10.0f * sentido; //atras +Z
		}
		if (key == GLFW_KEY_6)
		{
			theWindow->llanta6 += 10.0f * sentido; //atras -Z
		}
	}

	//arreglo keys: lo usa la cámara (W A S D) para saber qué teclas siguen presionadas
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
