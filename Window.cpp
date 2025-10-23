#include "Window.h"

Window::Window() {
	width = 800;
	height = 600;
  lamparaEstaApagada = true;
   

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;

  lamparaEstaApagada = true;
  gokuEstaElevandoSuKi = false;
  carroEstaAvanzando = false;
  carroEstaRetrocediendo = false;
  esCofreAbierto = false;

  articulacion1 = 0.0f;
  articulacion2 = 0.0f;
  articulacion3 = 0.0f;


	muevex = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise() {
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "PracticaXX:Nombre de la practica", NULL, NULL);

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

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
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
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}




	if (key == GLFW_KEY_R)
	{
    theWindow->carroEstaRetrocediendo = true;
    theWindow->articulacion2 += 0.5f;
	}
	if (key == GLFW_KEY_R && action == GLFW_RELEASE)
	{
    theWindow->carroEstaRetrocediendo = false;
	}

	if (key == GLFW_KEY_T)
	{
    theWindow->carroEstaAvanzando = true;
    theWindow->articulacion2 -= 0.5f;
	}
	if (key == GLFW_KEY_T && action == GLFW_RELEASE)
	{
    theWindow->carroEstaAvanzando = false;
	}




  static float paso = 2.0;
  if (theWindow->articulacion1 == 0.0f) {
	  paso = 2.0f;
  }
  else if (theWindow->articulacion1 == 40.0f) {
	  paso = -2.0f;
  }
  if (key == GLFW_KEY_C) {
    theWindow->articulacion1 += paso;
  } 
  if (theWindow->articulacion1 >= 0 && theWindow->articulacion1 <= 4) {
    theWindow->esCofreAbierto = false;
  }
  else {
    theWindow->esCofreAbierto = true;
  }




if (key == GLFW_KEY_F && GLFW_PRESS == action){
	theWindow->lamparaEstaApagada = false;
}

if (key == GLFW_KEY_V && GLFW_PRESS == action) {
	theWindow->lamparaEstaApagada = true;
}

if (key == GLFW_KEY_G && GLFW_PRESS == action){
	theWindow->gokuEstaElevandoSuKi = true;
}

if (key == GLFW_KEY_B && GLFW_PRESS == action) {
	theWindow->gokuEstaElevandoSuKi = false;
}








	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
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
