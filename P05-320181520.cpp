/*
Práctica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Model Goddard_M;
Model mandibula;
Model pataDelantera1;
Model pataDelantera2;
Model pataTrasera1;
Model pataTrasera2;

Model Carro;
Model Cofre;
Model Llanta1;
Model Llanta2;
Model Llanta3;
Model Llanta4;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





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

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	Goddard_M = Model();
	Goddard_M.LoadModel("models/cuerpo.fbx");

  mandibula = Model();
	mandibula.LoadModel("models/mandibula.fbx");

  pataTrasera1 = Model();
  pataTrasera1.LoadModel("models/pataTrasera1.fbx");

  pataTrasera2 = Model();
  pataTrasera2.LoadModel("models/pataTrasera1.fbx");

  pataDelantera1 = Model();
  pataDelantera1.LoadModel("models/pataDelantera1.fbx");

  pataDelantera2 = Model();
  pataDelantera2.LoadModel("models/pataDelantera1.fbx");


  Carro = Model();
  Carro.LoadModel("models/carro_sinCofreNiLlantas.fbx");

  Cofre = Model();
  Cofre.LoadModel("models/cofre.fbx");

  Llanta1 = Model();
  Llanta1.LoadModel("models/llanta.fbx");
  Llanta2 = Model();
  Llanta2.LoadModel("models/llanta.fbx");
  Llanta3 = Model();
  Llanta3.LoadModel("models/llanta.fbx");
  Llanta4 = Model();
  Llanta4.LoadModel("models/llanta.fbx");





	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
  glm::mat4 modelaux2(1.0);
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
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();



		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//Goddard

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -1.5f));
		modelaux = model;
    color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_M.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

    model = modelaux;
    model = glm::translate(model, glm::vec3(2.95f, 0.9f, -0.1f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //modelo de goddard de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mandibula.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
    
    model = modelaux;
    model = glm::translate(model, glm::vec3(1.0f, -0.4f, -0.6f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //modelo de goddard de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataDelantera1.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

    model = modelaux;
    model = glm::translate(model, glm::vec3(1.0f, -0.4f, 0.6f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //modelo de goddard de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataDelantera2.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

    model = modelaux;
    model = glm::translate(model, glm::vec3(-0.5f, -1.0f, 0.6f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //modelo de goddard de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTrasera2.RenderModel();//modificar por el modelo sin las 4 patas y sin cola

    model = modelaux;
    model = glm::translate(model, glm::vec3(-0.5f, -1.0f, -0.6f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //modelo de goddard de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTrasera1.RenderModel();//modificar por el modelo sin las 4 patas y sin cola


    // -----------------------------INICIA DIBUJO DEL CARRO ------------------------------------------------ //
		model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0, 0, mainWindow.getarticulacion7()/2));
		model = glm::translate(model, glm::vec3(25.0f, 2.4f, 65.5f));
    model = glm::rotate(model, 90*toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, 180*toRadians, glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(0.15, 0.15, 0.15));
		modelaux2 = model;
    color = glm::vec3(0.00f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro.RenderModel();

    model = modelaux2;
		model = glm::translate(model, glm::vec3(2.05f, -40.75f, 20.75f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion6()), glm::vec3(1.0f, 0.0f, 0.0f));
    color = glm::vec3(0.0, 0.0f, 0.0f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();

    model = modelaux2;
		model = glm::translate(model, glm::vec3(31.0f, -80.75f, -10.75f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion7()), glm::vec3(1.0f, 0.0f, 0.0f));
    color = glm::vec3(0.0f, 0.0f, 0.0f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta1.RenderModel();

    model = modelaux2;
		model = glm::translate(model, glm::vec3(-23.75f, -80.75f, -10.75f));
    model = glm::rotate(model, 180*toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion7()), glm::vec3(-1.0f, 0.0f, 0.0f));
    color = glm::vec3(0.0f, 0.0f, 0.0f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta2.RenderModel();

    model = modelaux2;
		model = glm::translate(model, glm::vec3(33.5f, 62.7f, -10.75f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion7()), glm::vec3(1.0f, 0.0f, 0.0f));
    color = glm::vec3(0.0f, 0.0f, 0.0f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta3.RenderModel();

    model = modelaux2;
		model = glm::translate(model, glm::vec3(-28.75f, 62.75f, -10.75f));
    model = glm::rotate(model, 180*toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model,  glm::radians(mainWindow.getarticulacion7()), glm::vec3(-1.0f, 0.0f, 0.0f));
    color = glm::vec3(0.0f, 0.0f, 0.0f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta4.RenderModel();


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

