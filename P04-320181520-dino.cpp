#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

//clases para dar orden y limpieza al còdigo
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
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
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

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
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
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
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
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
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

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


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main() {
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(36, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList

	CreateShaders();
	
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía
  glm::mat4 model2(1.0);

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose()) {
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); // solo una ves
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix())); // solo una







    // Cuerpo
    model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f)); // se realizan al último
    model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0)); // menos
    
    modelaux = model;  // Punto de guardado 1
    model2 = model;    // Punto de guardado 2
   
    model = glm::rotate(model, 5.0f*toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(5.0f, 3.0f, 3.0f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();



    // Articulacion 1 (Cola)
    model = modelaux;         //->X 
    model = glm::translate(model, glm::vec3(2.5f, 0.7f, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();



    // cola (primera parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(0.7f, -0.4f, 0.0));

    modelaux = model;

		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.25f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();




    // Articulacion 2 (Cola)
    model = modelaux;         //->X 
    model = glm::translate(model, glm::vec3(0.7f, 0.4f, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();



    // cola (segunda parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(1.2f, -0.0f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(2.5f, 1.25f, 1.25f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();






    // Articulacion 3 (Cola)
    model = modelaux;         //->X 
    model = glm::translate(model, glm::vec3(1.25f, 0.2f, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();







    // cola (tercera parte parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(2.75f, 0.0f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(5.5f, 0.75f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();

    model = modelaux; // cola parte final
    model = glm::translate(model, glm::vec3(5.25f, 0.02f, 0.0)); // menos
    model = glm::rotate(model, 90*toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(0.2f, 5.5f, 0.2f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[3]->RenderMeshGeometry();






    // Para que model2 siga almacenando la posicion del cuerpo
    modelaux = model2;


    // Articulacion 4 (piernas con torso)
    model = modelaux;         //->X 
    model = glm::translate(model, glm::vec3(1.25f, 0.5f, 2.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();


    // Pierna que si ves de frente al dinosaurio es la izquierda (primera parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(0.0f, -1.25f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(1.25f, 3.5f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();


    // Articulacion 5 (piernas)
    model = modelaux;         //->X 
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();


    // Pierna que si ves de frente al dinosaurio es la izquierda (segunda parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();


    // Articulacion 6 (piernas)
    model = modelaux;         //->X 
    model = glm::translate(model, glm::vec3(0.0f, -1.0, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, -1.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();


    // Pierna que si ves de frente al dinosaurio es la izquierda (tercera parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(0.8f, 1.0f, 0.8f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();



    modelaux = model2;

    // Articulacion 7 (piernas con torso)
    model = modelaux;         //->X 
    model = glm::translate(model, glm::vec3(1.25f, 0.5f, -2.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, -1.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();



    // Pierna que si ves de frente al dinosaurio es la derecha (primera parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(0.0f, -1.25f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(1.25f, 3.5f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();






    // Articulacion 8 (piernas)
    model = modelaux;         //->X 
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, -1.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();



    // Pierna que si ves de frente al dinosaurio es la derecha (segunda parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();




    // Articulacion 9 (piernas)
    model = modelaux;         //->X 
    model = glm::translate(model, glm::vec3(0.0f, -1.0, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();



    // Pierna que si ves de frente al dinosaurio es la derecha (tercera parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(0.8f, 1.0f, 0.8f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();
    
    



    model = model2;  // Articulacion brazo-torso izquierdo

    model = glm::translate(model, glm::vec3(-1.5f, -0.75, 1.5));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, -1.0f));
    
    //modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();




    modelaux = model2;

    // Brazo que si ves de frente al dinosaurio es el izquierdo (primera parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(-1.5f, -0.75f, 2.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, -1.0f));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(0.8f, 2.0f, 0.8f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();




    model = modelaux;  // Articulacion codo izquierdo

    model = glm::translate(model, glm::vec3(0.0f, -1.0, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, -1.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();





    // Brazo que si ves de frente al dinosaurio es el izquierdo (segunda parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();











    model = model2;  // Articulacion brazo-torso izquierdo

    model = glm::translate(model, glm::vec3(-1.5f, -0.75, 1.5));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
    
    //modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();

    modelaux = model2;



    // Brazo que si ves de frente al dinosaurio es el izquierdo (primera parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(-1.5f, -0.75f, -2.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(0.8f, 2.0f, 0.8f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();




    model = modelaux;  // Articulacion codo izquierdo

    model = glm::translate(model, glm::vec3(0.0f, -1.0, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, -1.0f));
    
    modelaux = model; // punto de guardado

		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();





    // Brazo que si ves de frente al dinosaurio es el izquierdo (segunda parte)
    model = modelaux;
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0));

    modelaux = model; // Punto de guardado

		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
    glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    meshList[0]->RenderMesh();












    modelaux = model2;

    // CUELLO
    model = modelaux;
    model = glm::translate(model, glm::vec3(-3.0f, 2.0f, 0.0)); // menos
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));

    modelaux = model;  // Punto de guardado 1

    model = glm::rotate(model, 45.0f*toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(3.0f, 1.5f, 1.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();




    // articulación del cuello-torso
    model = modelaux;
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();








    // Articulación del cuello-cabeza(parte de arriba)
    model = modelaux;
    model = glm::translate(model, glm::vec3(-1.05f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();





    model = modelaux; // CABESA (parte de arriba) (Aqui van los ojos, cejas y cresta)
    model = glm::translate(model, glm::vec3(-1.8f, 1.5f, 0.0)); // menos
    model = glm::scale(model, glm::vec3(2.5f, 1.5f, 1.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();


    model = modelaux; // cejas
    model = glm::translate(model, glm::vec3(-2.8f, 1.85f, 1.0)); // menos
    model = glm::rotate(model, 25*toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f, 0.25f, 0.25f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();


    model = modelaux; // pico
    model = glm::translate(model, glm::vec3(-2.5f, 2.75f, 0.0)); // menos
    model = glm::scale(model, glm::vec3(0.15f, 1.0f, 0.15f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[3]->RenderMeshGeometry();

    model = modelaux; // pico
    model = glm::translate(model, glm::vec3(-1.8f, 2.75f, 0.0)); // menos
    model = glm::scale(model, glm::vec3(0.15f, 1.0f, 0.15f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[3]->RenderMeshGeometry();

    model = modelaux; // pico
    model = glm::translate(model, glm::vec3(-1.1f, 2.75f, 0.0)); // menos
    model = glm::scale(model, glm::vec3(0.15f, 1.0f, 0.15f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[3]->RenderMeshGeometry();






    model = modelaux; // CABESA (parte de arriba) (Aqui van los dientes)
    model = glm::translate(model, glm::vec3(-3.5f, 1.3f, 0.0)); // menos
    model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.0f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();




    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-4.5f, 0.8f, -0.35)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-4.5f, 0.8f, -0.2)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-4.5f, 0.8f, -0.0)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-4.5f, 0.8f, 0.15)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-4.25f, 0.8f, -0.36)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-4.0f, 0.8f, -0.36)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-3.75f, 0.8f, -0.36)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-3.5f, 0.8f, -0.36)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-3.25f, 0.8f, -0.36)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();




    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-3.0f, 0.8f, -0.36)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-2.75f, 0.8f, -0.36)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();







    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-4.25f, 0.8f, 0.15f)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-4.0f, 0.8f, 0.15f)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-3.75f, 0.8f, 0.15f)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-3.5f, 0.8f, 0.15f)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-3.25f, 0.8f, 0.15f)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();




    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-3.0f, 0.8f, 0.15f)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-2.75f, 0.8f, 0.15f)); // menos
    model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

//-----------------------------------------------------------



    model = modelaux; // OJO
    model = glm::translate(model, glm::vec3(-2.75f, 1.5f, 0.6)); // menos
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();

    model = modelaux; // Pupila
    model = glm::translate(model, glm::vec3(-2.9f, 1.5f, 0.8)); // menos
    model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.2f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(0.0f, 0.0f, 0.0f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();


    model = modelaux; // OJO
    model = glm::translate(model, glm::vec3(-2.75f, 1.5f, -0.6)); // menos
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();

    model = modelaux; // Pupila
    model = glm::translate(model, glm::vec3(-2.9f, 1.5f, -0.8)); // menos
    model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.2f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(0.0f, 0.0f, 0.0f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();






    // Articulacion de la mandibula
    model = modelaux;
    model = glm::translate(model, glm::vec3(-1.05f, 1.0f, 0.0));
    model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, 12*toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    
    modelaux = model;

		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
	  color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));     
    sp.render();





    model = modelaux; // CABESA (parte de abajo) o bien mandibula (primera parte)
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0)); // menos

    modelaux = model;  // Punto de guardado

    model = glm::scale(model, glm::vec3(2.0f, 0.75f, 1.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(0.478f, 0.255f, 0.067f);
    //color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();





    model = modelaux; // mandibula (segunda parte)
    model = glm::translate(model, glm::vec3(-1.75f, 0.0f, 0.0)); // menos

    modelaux = model;  // Punto de guardado

    model = glm::scale(model, glm::vec3(2.0f, 0.75f, 0.75f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //color = glm::vec3(0.478f, 0.255f, 0.067f);
    color = glm::vec3(0.153f, 0.133f, 0.086f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[0]->RenderMesh();





    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-0.75f, 0.4f, -0.12)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-0.75f, 0.4f, 0.12)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-0.75f, 0.4f, 0.36)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-0.55f, 0.4f, 0.36)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-0.35f, 0.4f, 0.36)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-0.15f, 0.4f, 0.36)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(0.05f, 0.4f, 0.36)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(0.25f, 0.4f, 0.36)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();




    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(0.45f, 0.4f, 0.36)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(0.65f, 0.4f, 0.36)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();







    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-0.55f, 0.4f, -0.12f)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-0.35f, 0.4f, -0.12f)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(-0.15f, 0.4f, -0.12f)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();


    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(0.05f, 0.4f, -0.12f)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(0.25f, 0.4f, -0.12f)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();




    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(0.45f, 0.4f, -0.12f)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();

    model = modelaux; // diente
    model = glm::translate(model, glm::vec3(0.65f, 0.4f, -0.12f)); // menos
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f)); // Se realiza primero
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
    meshList[1]->RenderMesh();






		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}





