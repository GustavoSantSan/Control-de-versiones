#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

  bool apagarLampara() { return lamparaEstaApagada; }
  bool elevarKi() { return gokuEstaElevandoSuKi;}
  bool avanzarCarro() { return carroEstaAvanzando; }
  bool retrocederCarro() { return carroEstaRetrocediendo; }
  bool cofreEstaAbierto() { return esCofreAbierto; }


  bool getEstaAbriendoI () {  return estaAbriendoI; }
  void setEstaAbriendoI (bool v) {  estaAbriendoI = v; }

  bool getEstaCerrandoI () {  return estaCerrandoI;}
  void setEstaCerrandoI (bool v) { estaCerrandoI = v;} 

  GLfloat getArticulacion1() { return articulacion1; }
  void setArticulacion1(GLfloat v) { articulacion1 = v; }
  
  bool getLanzarDado() {return lanzarDado; }
  void setLanzarDado(bool v) {lanzarDado = v;}

  bool getDadoSubiendo() { return dadoSubiendo; }

  void setDadoSubiendo(bool v) { dadoSubiendo = v; }


  GLfloat getArticulacion2() { return articulacion2; }
  void setArticulacion2(GLfloat v) { articulacion2 = v; }

  GLfloat getArticulacion3() { return articulacion3; }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool mouseFirstMoved;

  bool lamparaEstaApagada;
  bool gokuEstaElevandoSuKi;
  bool carroEstaAvanzando;
  bool carroEstaRetrocediendo;
  bool esCofreAbierto;

  bool estaAbriendoI;
  bool estaCerrandoI;
  bool lanzarDado;
  bool dadoSubiendo;

  GLfloat rotax,rotay,rotaz;
  GLfloat articulacion1;
  GLfloat articulacion2;
  GLfloat articulacion3;
  
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

