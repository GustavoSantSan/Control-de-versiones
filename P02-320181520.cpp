//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // para transformaciones
#include <glm/gtc/type_ptr.hpp>         // para pasar a OpenGL
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";




//shaders nuevos se crearían acá
static const char* vShaderRojo = "shaders/shaderRojo.vert";
static const char* fShaderRojo = "shaders/shaderRojo.frag";

static const char* vShaderVerde = "shaders/shaderVerde.vert";
static const char* fShaderVerde = "shaders/shaderVerde.frag";

static const char* vShaderAzul = "shaders/shaderAzul.vert";
static const char* fShaderAzul = "shaders/shaderAzul.frag";

static const char* vShaderCafe = "shaders/shaderCafe.vert";
static const char* fShaderCafe = "shaders/shaderCafe.frag";

static const char* vShaderVerdeOscuro = "shaders/shaderVerdeOscuro.vert";
static const char* fShaderVerdeOscuro = "shaders/shaderVerdeOscuro.frag";





float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

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
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);



// Índices de los triángulos (cada grupo de 3 define una cara)
unsigned int indices2[] = {
    // Cara frontal
    0, 1, 4,
    // Cara derecha
    1, 2, 4,
    // Cara trasera
    2, 3, 4,
    // Cara izquierda
    3, 0, 4,
    // Base (dividida en 2 triángulos)
    0, 1, 2,
    0, 2, 3
};

// Vértices (x, y, z):
GLfloat vertices2[] = {
    -0.5f, 0.0f,  0.5f,   // 0: frente-izquierda
     0.5f, 0.0f,  0.5f,   // 1: frente-derecha
     0.5f, 0.0f, -0.5f,   // 2: atrás-derecha
    -0.5f, 0.0f, -0.5f,   // 3: atrás-izquierda
     0.0f, 0.8f,  0.0f    // 4: vértice superior
};

Mesh* obj2 = new Mesh();
obj2->CreateMesh(vertices2, indices2, 15, 18); // 15 floats (5 vértices * 3 coords), 18 indices (6 triángulos * 3 vértices)
meshList.push_back(obj2);

}

//Vértices de un cubo
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
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{

	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
    -0.85f,	-0.8f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.73f,	-0.8f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.73f,	0.8f, -5.0f, 1.0f, 0.0f, 0.0f,
    
    -0.85f,	-0.8f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.73f,	0.8f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.85f,	0.8f, -5.0f, 1.0f, 0.0f, 0.0f,

    -0.85f,	0.68f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	0.68f,	-5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	0.8f, -5.0f, 1.0f, 0.0f, 0.0f,

    -0.85f,	0.68f,	-5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	0.8f,	-5.0f,1.0f, 0.0f, 0.0f,
    -0.85f,	0.8f, -5.0f, 1.0f, 0.0f, 0.0f,

    -0.85f,	-0.8f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	-0.8f,	-5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	-0.68f, -5.0f, 1.0f, 0.0f, 0.0f,

    -0.85f,	-0.8f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	-0.68f,	-5.0f, 1.0f, 0.0f, 0.0f,
    -0.85f,	-0.68f, -5.0f, 1.0f, 0.0f, 0.0f,

    -0.47f, -0.8f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	-0.8f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	0.0f, -5.0f, 1.0f, 0.0f, 0.0f,

    -0.47f,	-0.8f,	-5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	0.0f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.47f,	0.0f, -5.0f, 1.0f, 0.0f, 0.0f,

    -0.6f,	-0.054f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	-0.054f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	0.054f, -5.0f, 1.0f, 0.0f, 0.0f,

    -0.6f, -0.054f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.35f,	0.054f, -5.0f, 1.0f, 0.0f, 0.0f,
    -0.6f,	0.054f, -5.0f, 1.0f, 0.0f, 0.0f,
    //U
    -0.2f, -0.68f,	-5.0f, 0.0f, 1.0f, 0.0f,
    -0.08f,	-0.68f, -5.0f, 0.0f, 1.0f, 0.0f,
    -0.08f,	0.8f, -5.0f, 0.0f, 1.0f, 0.0f,

    -0.2f, -0.68f, -5.0f, 0.0f, 1.0f, 0.0f,
    -0.08f,	0.8f,	-5.0f, 0.0f, 1.0f, 0.0f,
    -0.2f,	0.8f, -5.0f, 0.0f, 1.0f, 0.0f,

    0.18f, -0.68f, -5.0f, 0.0f, 1.0f, 0.0f,
    0.3f,	-0.68f, -5.0f, 0.0f, 1.0f, 0.0f,
    0.3f,	0.8f, -5.0f, 0.0f, 1.0f, 0.0f,

    0.18f, -0.68f, -5.0f, 0.0f, 1.0f, 0.0f,
    0.3f,	0.8f, -5.0f, 0.0f, 1.0f, 0.0f,
    0.18f, 0.8f, -5.0f, 0.0f, 1.0f, 0.0f,

    -0.2f, -0.8f, -5.0f, 0.0f, 1.0f, 0.0f,
    0.3f, -0.8f, -5.0f, 0.0f, 1.0f, 0.0f,
    0.3f,	-0.68f, -5.0f, 0.0f, 1.0f, 0.0f,

    -0.2f,	-0.8f, -5.0f, 0.0f, 1.0f, 0.0f,
    0.3f,	-0.68f, -5.0f, 0.0f, 1.0f, 0.0f,
    -0.2f,	-0.68f, -5.0f, 0.0f, 1.0f, 0.0f,
    //S
    0.45f,	0.68f,	-5.0f, 0.0f, 0.0f, 1.0f,
    0.95f,	0.68f,	-5.0f, 0.0f, 0.0f, 1.0f,
    0.95f,	0.8f, -5.0f, 0.0f, 0.0f, 1.0f,

    0.45f,	0.68f, -5.0f, 0.0f, 0.0f, 1.0f,
    0.95f,	0.8f, -5.0f,	0.0f, 0.0f, 1.0f,
    0.45f,	0.8f, -5.0f, 0.0f, 0.0f, 1.0f,

    0.45f,	0.0f,	-5.0f, 0.0f, 0.0f, 1.0f,
    0.57f,	0.0f,	-5.0f, 0.0f, 0.0f, 1.0f,
    0.57f,	0.8f, -5.0f, 0.0f, 0.0f, 1.0f,

    0.45f,	0.0f,	-5.0f, 0.0f, 0.0f, 1.0f,
    0.57f,	0.8f, -5.0f, 0.0f, 0.0f, 1.0f,
    0.45f,	0.8f, -5.0f, 0.0f, 0.0f, 1.0f,

    0.45f,	-0.06f, -5.0f,	0.0f, 0.0f, 1.0f,
    0.95f,	-0.06f, -5.0f, 0.0f, 0.0f, 1.0f,
    0.95f,	0.06f, -5.0f, 0.0f, 0.0f, 1.0f,

    0.45f,	-0.06f, -5.0f, 0.0f, 0.0f, 1.0f,
    0.95f,	0.06f, -5.0f, 0.0f, 0.0f, 1.0f,
    0.45f,	0.06f, -5.0f, 0.0f, 0.0f, 1.0f,

    0.83f,	-0.8f, -5.0f, 0.0f, 0.0f, 1.0f,
    0.95f,	-0.8f, -5.0f, 0.0f, 0.0f, 1.0f,
    0.95f,	0.0f, -5.0f, 0.0f, 0.0f, 1.0f,

    0.83f,	-0.8f, -5.0f,	0.0f, 0.0f, 1.0f,
    0.95f,	0.0f, -5.0f, 0.0f, 0.0f, 1.0f,
    0.83f,	0.0f, -5.0f, 0.0f, 0.0f, 1.0f,

    0.45f,	-0.8f, -5.0f, 0.0f, 0.0f, 1.0f,
    0.95f,	-0.8f, -5.0f,	0.0f, 0.0f, 1.0f,
    0.95f,	-0.68f, -5.0f, 0.0f, 0.0f, 1.0f,

    0.45f,	-0.8f,	-5.0f, 0.0f, 0.0f, 1.0f,
    0.95f,	-0.68f, -5.0f,	0.0f, 0.0f, 1.0f,
    0.45f,	-0.68f, -5.0f, 0.0f, 0.0f, 1.0f
	};

	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,504);
	meshColorList.push_back(letras);



	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		
	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);


}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);


	Shader *shader3 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader3->CreateFromFiles(vShaderRojo, fShaderRojo);
	shaderList.push_back(*shader3);

  Shader *shader4 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader4->CreateFromFiles(vShaderVerde, fShaderVerde);
	shaderList.push_back(*shader4);

	Shader *shader5 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader5->CreateFromFiles(vShaderAzul, fShaderAzul);
	shaderList.push_back(*shader5);

	Shader *shader6 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader6->CreateFromFiles(vShaderCafe, fShaderCafe);
	shaderList.push_back(*shader6);

	Shader *shader7 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader7->CreateFromFiles(vShaderVerdeOscuro, fShaderVerdeOscuro);
	shaderList.push_back(*shader7);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
    if (angulo == 360) {
      angulo = 0;
    }
		angulo += 1;
													


/*
    // LETRAS
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.5f));
    model = glm::translate(model, glm::vec3(6.75f, 5.0f, -5.5f));

  	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
*/


    // CASA
		shaderList[2].useShader(); // Cubo Rojo 
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, -3.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.25f,1.125f,1.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();


		shaderList[5].useShader();  // Tronco derecho
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.125f, -0.75f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25f,1.1f,0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		shaderList[5].useShader();  // Tronco izq
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.125f, -0.75f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25f,1.1f,0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();


    // Hojas derechas
		shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.125f, 0.5f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f,1.5f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.125f, 0.5f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f,1.5f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.125f, 0.5f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f,1.5f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.125f, 0.5f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f,1.5f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


    // Hojas izq
	  shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.125f, 0.5f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f,1.5f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.125f, 0.5f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f,1.5f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.125f, 0.5f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f,1.5f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.125f, 0.5f, -4.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f,1.5f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


/*		shaderList[4].useShader();  // Techo2
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, -3.0f));
    model = glm::scale(model, glm::vec3(1.45f,1.5f,2.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
*/

    // Techo
		shaderList[4].useShader(); 
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
    model = glm::scale(model, glm::vec3(2.0f,1.25f,2.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[4].useShader(); 
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
    model = glm::scale(model, glm::vec3(2.0f,1.25f,2.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 315 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[4].useShader(); 
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
    model = glm::scale(model, glm::vec3(2.0f,1.25f,2.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		shaderList[4].useShader(); 
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
    model = glm::scale(model, glm::vec3(2.0f,1.25f,2.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -3.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 335 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -3.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 25 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -3.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -3.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 335 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();


		shaderList[3].useShader(); // Puerta
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, -3.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));//ESto??
    model = glm::scale(model, glm::vec3(0.125f,0.25f,0.125f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();


    // Ventanas
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, -0.35f, -3.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));//ESto??
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));//ESto??
    model = glm::scale(model, glm::vec3(0.25f,0.25f,0.125f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, -0.35f, -3.0f));
    model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-0.35f, 0.0f, 0.0f));//ESto??
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.6f));//ESto??
    model = glm::scale(model, glm::vec3(0.25f,0.25f,0.125f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();



		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}



