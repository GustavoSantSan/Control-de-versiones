/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()

#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;

// SE USAN PARA EL LETRERO ----------------------------------------
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;


float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float dragonavance = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture PokemonTexture;
Texture octaTexture;
Texture Pokemon2Texture;


Model Kitt_M;
Model Llanta_M;

Model Dragon_M;
Model Dragon_Ala_Der_M;
Model Dragon_Ala_Izq_M;

Model Tiamat_M;
Skybox skybox;

Model PuertaIzq_M;
Model PuertaDer_M;
Model Arco_M;
Model Letrero_M;




//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 120.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	GLfloat letraVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.8f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		  0.2f, 0.8f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.2f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
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


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número
  //

  Mesh* obj8 = new Mesh();
  obj8->CreateMesh(letraVertices, numeroIndices, 32, 6);
  meshList.push_back(obj8);  // Solo una letra
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



//DAdo es el indice 8
void CrearOctaedro() {
    unsigned int octaedro_indices[] = {
        0,  1,  2,     // Cara 1
        3,  4,  5,     // Cara 2
        6,  7,  8,     // Cara 3
        9,  10, 11,    // Cara 4
        12, 13, 14,    // Cara 5
        15, 16, 17,    // Cara 6
        18, 19, 20,    // Cara 7
        21, 22, 23     // Cara 8
    };

    GLfloat octaedro_vertices[] = {
// Cara 1 (arriba - frente)
0.0f, 1.0f, 0.0f,   0.50f, 0.73f,   0.0f, -1.0f, 0.0f,
1.0f, 0.0f, 0.0f,   0.27f, 0.51f,  -1.0f,  0.0f, 0.0f,
0.0f, 0.0f, 1.0f,   0.73f, 0.51f,   0.0f,  0.0f, -1.0f,

// Cara 2 (arriba - izquierda)
0.0f, 1.0f, 0.0f,   0.48f, 0.73f,   0.0f, -1.0f, 0.0f,
0.0f, 0.0f, 1.0f,   0.25f, 0.52f,   0.0f,  0.0f, -1.0f,
-1.0f,0.0f, 0.0f,   0.05f, 0.73f,   1.0f,  0.0f, 0.0f,

// Cara 3 (arriba - atrás)
0.0f, 1.0f, 0.0f,   0.46f, 0.78f,   0.0f, -1.0f, 0.0f,
-1.0f,0.0f, 0.0f,   0.04f, 0.78f,   1.0f,  0.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.25f, 0.98f,   0.0f,  0.0f, 1.0f,

// Cara 4 (arriba - derecha)
0.0f, 1.0f, 0.0f,   0.52f, 0.73f,   0.0f, -1.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.98f, 0.73f,   0.0f,  0.0f, 1.0f,
1.0f, 0.0f, 0.0f,   0.75f, 0.52f,  -1.0f,  0.0f, 0.0f,

// Cara 5 (abajo - frente)
0.0f,-1.0f, 0.0f,   0.50f, 0.26f,   0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f,   0.26f, 0.49f,   0.0f, 0.0f, -1.0f,
1.0f, 0.0f, 0.0f,   0.73f, 0.49f,  -1.0f, 0.0f, 0.0f,

// Cara 6 (abajo - izquierda)
0.0f,-1.0f, 0.0f,   0.48f, 0.26f,   0.0f, 1.0f, 0.0f,
-1.0f,0.0f, 0.0f,   0.02f, 0.26f,   1.0f, 0.0f, 0.0f,
0.0f, 0.0f, 1.0f,   0.26f, 0.47f,   0.0f, 0.0f, -1.0f,

// Cara 7 (abajo - atrás)
0.0f,-1.0f, 0.0f,   0.48f, 0.23f,   0.0f, 1.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.25f, 0.02f,   0.0f, 0.0f, 1.0f,
-1.0f,0.0f, 0.0f,   0.06f, 0.23f,   1.0f, 0.0f, 0.0f,

// Cara 8 (abajo - derecha)
0.0f,-1.0f, 0.0f,   0.54f, 0.27f,   0.0f, 1.0f, 0.0f,
1.0f, 0.0f, 0.0f,   0.73f, 0.47f,  -1.0f, 0.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.97f, 0.27f,   0.0f, 0.0f, 1.0f 
    };
    Mesh* oct = new Mesh();
    oct->CreateMesh(octaedro_vertices, octaedro_indices, 24 * 8, 24);
    meshList.push_back(oct);
}




int main() {
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
  CrearOctaedro();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
  PokemonTexture = Texture("Textures/letrasPokemon.png");
  PokemonTexture.LoadTextureA();
	octaTexture = Texture("Textures/octaedroNumeros.png"); // -----------------------
	octaTexture.LoadTextureA();
  Pokemon2Texture = Texture("Textures/POKEMONLETRAS.png");
  Pokemon2Texture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");

	Dragon_M = Model();
	Dragon_M.LoadModel("Models/MyModels/dragon sin base.obj");
	Dragon_Ala_Der_M = Model();
	Dragon_Ala_Der_M.LoadModel("Models/MyModels/ala-derecha.obj");
	Dragon_Ala_Izq_M = Model();
	Dragon_Ala_Izq_M.LoadModel("Models/MyModels/ala-izquierda.obj");





  PuertaIzq_M = Model();
  PuertaDer_M = Model();
  Arco_M = Model();
  Letrero_M = Model();
  PuertaIzq_M.LoadModel("Models/MyModels/puerta izq.obj");
  PuertaDer_M.LoadModel("Models/MyModels/puerta der.obj");
  Arco_M.LoadModel("Models/MyModels/Arco2.obj");
  Letrero_M.LoadModel("Models/MyModels/letrero.obj");





	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.5f; // Se aumentó de 0.01f a 0.5f
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;



// Variables que se agregan en ejercicio de clase 
	avanza = true;
	bool dragonAvanza = true;

  int contadorNum = 0;
	bool esUno = true;





	glm::vec3 lowerLight(0.0f,0.0f,0.0f);
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);



// Para puertas
  GLfloat auxiliar = 0.0f;


// Para letras
		GLfloat toffsetletracambiau = 0.0;
		GLfloat toffsetletrav = 0.0;

    GLfloat coordenadasLetrasU[] = {
    //          P      R     O     Y     E     C     T    O
         0.8f, 0.6f, 0.8f, 0.4f, 0.2f, 0.8f, 0.4f, 0.6f, 0.4f, 0.8f,

    //   C      G      E    I    H    C
        0.4f, 0.2f, 0.8f, 0.0f, 0.6f, 0.4f
    }; 
    
    GLfloat coordenadasLetrasV[] = {
    //         P      R     O     Y     E     C     T    O
      0.2f, 0.4f, 0.4f, 0.4f, 0.2f, 0.0f, 0.0f, 0.2f, 0.4f, 0.2f,

    //   C      G      E    I    H    C
        0.0f, 0.8f, 0.0f, 0.6f, 0.8f, 0.0f
    };
int abba = 0;
int abba2 = 0;



// DAdo
bool dadoCayendo = false;

// Inicializar semilla una sola vez (por ejemplo, al iniciar el programa)
srand(static_cast<unsigned int>(time(nullptr)));

// Generar número aleatorio entero entre 0 y N-1
int randomInt = rand() % 8;  // Ejemplo: N = 10 → valores [0..7]
int distanciaQueSubiraElDado = (rand() % 26) + 20;  // Ejemplo: N = 10 → valores [0..25]
int loly = distanciaQueSubiraElDado;
int lolx = (rand() % 26) + 10;
int lolz = (rand() % 26) + 10;

GLfloat angulos[] = {125.6f, 125.6, 125.6f, 125.6f,
54.74f, 54.74f, 54.74f, 54.74f 
  };

  glm::vec3 ejes[] = {
    glm::vec3(1.0f * 1/glm::sqrt(2), 0.0f, -1.0f * 1/glm::sqrt(2)),

    glm::vec3(-1.0f * 1/glm::sqrt(2), 0.0f, -1.0f * 1/glm::sqrt(2)),

    glm::vec3(-1.0f * 1/glm::sqrt(2), 0.0f, 1.0f * 1/glm::sqrt(2)),

    glm::vec3(1.0f * 1/glm::sqrt(2), 0.0f, 1.0f * 1/glm::sqrt(2)),

    glm::vec3(1.0f * 1/glm::sqrt(2), 0.0f, -1.0f * 1/glm::sqrt(2)),

    glm::vec3(-1.0f * 1/glm::sqrt(2), 0.0f, 1.0f * 1/glm::sqrt(2)),

    glm::vec3(-1.0f * 1/glm::sqrt(2), 0.0f, -1.0f * 1/glm::sqrt(2)),

    glm::vec3(1.0f * 1/glm::sqrt(2), 0.0f, 1.0f * 1/glm::sqrt(2)),
  };

GLfloat posicionX = 0.0f;
GLfloat posicionZ = 0.0f;
GLfloat posicionZAcumulada = 0.0f;

GLfloat posicionXAcumulada = 0.0f;


int signo1 = (rand() % 2 == 0) ? 1 : -1;
int signo2 = (rand() % 2 == 0) ? 1 : -1;
int contadoRoy = 0;

float gravedad = 4.0f;
bool dadoRodando = false;


    bool inicializado = false;
        float gravedadInicial;
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()) {
		
    GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;





		angulovaria += 3.14f*deltaTime; // Cambiamos de 0.5 a 3.14f

// EJERCICIO 3 DE CLASE. DRAGON
		//dragonavance en el eje X
		if (dragonAvanza) {
			if (dragonavance > -20.0f) {
				dragonavance -= 0.1f * deltaTime;
			} else {
				dragonAvanza = false;
			}
		} 
    else {
			if (dragonavance < 0.0f) {
				dragonavance += 0.1f * deltaTime;
			} else {
				dragonAvanza = true;
			}
		}

    // Carro avance
    // Animación en loop
		if (avanza) {
			if (movCoche > -250.0f) {
				movCoche -= movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
			} else {
				avanza = false;
			}
		} else {
			if (movCoche < 330.0f) {
				movCoche += movOffset * deltaTime;
				rotllanta -= rotllantaOffset * deltaTime;
			} else {
				avanza = true;
			}
		}



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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
    
    //PISO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();





		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();





		//dragonavance para moverlo hacia adelante y atrás en X
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f - dragonavance, 5.0f+1*sin(glm::radians(angulovaria)), 16.0));
		//model = glm::translate(model, glm::vec3(0.0f, 5.0f, 16.0));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    if (dragonAvanza) {
      model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    } 
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.95));
    model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, sin(angulovaria / 100) / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_Ala_Der_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.95));
    model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -sin(angulovaria / 100) / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_Ala_Izq_M.RenderModel();
	











/*
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
*/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


/*
		//textura con movimiento: FLECHA
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.005; // Se aumenta de 0.001 a 0.005
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
*/

/*
		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
*/

/*
		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();
*/
/*
		for (int i = 1; i<4; i++) {
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		 }
*/
/*
		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}
 */


		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
//EJERCICIO 1 DE CLASE 
/*    if (contadorNum % 64 == 0) {
			toffsetnumerocambiau += 0.25;
    }
		//toffsetnumerocambiau += 0.25;
	if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();
*/

/*
		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();
// EJERCICIO 2 DE CLASSE
    if (contadorNum % 128 == 0) {
			esUno = !esUno;
    }
		if (esUno) {
			Numero1Texture.UseTexture();
		} else {
			Numero2Texture.UseTexture();
		}
		contadorNum++;

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
*/
 // THE PRACTICE STARTS HERE --------------------------------------------------
		// Octaedro en Opengl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + posicionXAcumulada, 3.0f + mainWindow.getArticulacion2(), 40.0f + posicionZAcumulada));
    model = glm::rotate( model, /*randomInt * */angulos[randomInt] * toRadians, ejes[randomInt] );
    model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		octaTexture.UseTexture();
		meshList[8]->RenderMesh();


if ( mainWindow.getLanzarDado() ) {
  if ( mainWindow.getDadoSubiendo() ) {
    auxiliar = mainWindow.getArticulacion2();
    auxiliar += gravedad * deltaTime;
    mainWindow.setArticulacion2(auxiliar);
    if (gravedad > 0.15f)
        gravedad -= 0.15f;
    //if (contadoRoy % 2 == 0)
        randomInt = rand() % 8;
    if ( mainWindow.getArticulacion2() >= loly ) {
      mainWindow.setDadoSubiendo(false);
      dadoCayendo = true;
      gravedad = 0.0f;
    }
    signo1 = (rand() % 2 == 0) ? 1 : -1;
    signo2 = (rand() % 2 == 0) ? 1 : -1;
  }
  else if (dadoCayendo) {
    auxiliar = mainWindow.getArticulacion2();
    //if (contadoRoy % 2 == 0)
        randomInt = rand() % 8;
    gravedad += 0.15f;
    if (auxiliar - gravedad*deltaTime >= 0.0f) {
      auxiliar -= gravedad * deltaTime;
      mainWindow.setArticulacion2(auxiliar);
    }
    else {
      mainWindow.setArticulacion2(0.0f);
      dadoCayendo = false;
      dadoRodando = true;
      inicializado = false;
    }
  }
  else if (dadoRodando) {
      posicionXAcumulada += 0.25f * deltaTime * signo1;
      posicionX += 0.25f * deltaTime * signo1;
      posicionZAcumulada += 0.25f * deltaTime * signo2;
      posicionZ += 0.25f * deltaTime * signo2;
      //if(contadoRoy % 16 == 0)
      randomInt = rand() % 8;
      if (posicionX >= lolx || posicionZ >= lolz || posicionX <= -1*lolx || posicionZ <= -1*lolz) {
        posicionX = 0.0f;
        posicionZ = 0.0f;
        gravedad = 4.0f;
        dadoRodando = false;
      }
  }
  else {
    mainWindow.setLanzarDado(false);
  }
}



// ARCO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 17.3f, -20.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
    modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arco_M.RenderModel();

// PUERTA IZQUIERDA (Se abre por rotación)
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.1f, -2.73f, -1.57f));
		model = glm::rotate(model, mainWindow.getArticulacion1() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaIzq_M.RenderModel();

// PUERTA DERECHA (Se abre deslizando a la derecha)
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.1f, 0.35f, 1.46f + mainWindow.getArticulacion1()/84 ));
		//model = glm::rotate(model, mainWindow.getArticulacion1() * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaDer_M.RenderModel();

// PUERTAS
if (mainWindow.getEstaAbriendoI()) {
  if (mainWindow.getArticulacion1() < 84.0f) {
    auxiliar = mainWindow.getArticulacion1();
    auxiliar += 0.2f * deltaTime;
    mainWindow.setArticulacion1(auxiliar);
  }
  else {
    mainWindow.setEstaAbriendoI(false);
    // LOOP
    mainWindow.setEstaCerrandoI(true);
  }
}
else if (mainWindow.getEstaCerrandoI()) {
  if (mainWindow.getArticulacion1() > 0.0f) {
    auxiliar = mainWindow.getArticulacion1();
    auxiliar -= 0.2f * deltaTime;
    mainWindow.setArticulacion1(auxiliar);
  }
  else {
    mainWindow.setEstaCerrandoI(false);
    // LOOP
    mainWindow.setEstaAbriendoI(true);
  }
}

// LETRERO
    model = modelaux;
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero_M.RenderModel();

		toffsetflechau += 0.003; // Se aumenta de 0.001 a 0.00
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = modelaux;
    model = glm::translate(model, glm::vec3(-1.9f, 3.6f, 0.0));
    model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(9.0f, 1.2f, 1.2f));

    glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Pokemon2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


/*
// LETRERO MALO
for (int i=0; i<7; i++) {
		toffset = glm::vec2(coordenadasLetrasU[(i+abba)%16], coordenadasLetrasV[(i+abba)%16]);
		model = modelaux;
    model = glm::translate(model, glm::vec3(-1.9f, 3.6f, -3.6f + i*1.2f));
    model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));

    glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		PokemonTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();
}
abba2++;
if (abba2 % 32 == 0)
  abba = (abba + 1) % 16;
*/
contadoRoy++;
loly = /*(rand() % 260)*/ + 110;
lolx = (rand() % 260) + 10;
lolz = (rand() % 260) + 10;

		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}


