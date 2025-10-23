/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <iostream>

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
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture octaTexture; 

Model Kitt_M;

Model Llanta1_M; // PARA IMPORTAR EL CARRO
Model Llanta2_M; 
Model Llanta3_M; 
Model Llanta4_M; 

Model Cofre_M;

Model Goku_M;

Model Blackhawk_M;
Model Dado_M; 
Model Lampara_M; 

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
//PointLight pointLights2[1];
SpotLight spotLights[MAX_SPOT_LIGHTS];


PointLight pointLightsAux[MAX_POINT_LIGHTS];
SpotLight spotLightsAux[MAX_SPOT_LIGHTS];


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset) {
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


void CreateObjects() {
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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




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



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
  CrearOctaedro(); // -------------------------------------

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

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
	octaTexture = Texture("Textures/octaedroNumeros.png"); // -----------------------
	octaTexture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/carroPRUEBA.obj");

	Llanta1_M = Model();
	Llanta1_M.LoadModel("Models/llanta1Santana.obj");
	Llanta2_M = Model();
	Llanta2_M.LoadModel("Models/llanta1Santana.obj");
	Llanta3_M = Model();
	Llanta3_M.LoadModel("Models/llanta2Santana.obj");
	Llanta4_M = Model();
	Llanta4_M.LoadModel("Models/llanta2Santana.obj");
  
  Cofre_M = Model();
  Cofre_M.LoadModel("Models/cofre.obj");

  Goku_M = Model();
  Goku_M.LoadModel("Models/Goku.obj");



	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

  Lampara_M = Model();
  Lampara_M.LoadModel("Models/lampara.obj");   // SEgunda captura .----------------------------

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






	unsigned int pointLightCount = 0;

  // Luz lampara ....------- -------------------------------------------------------
  pointLights[0] = PointLight(0.4f, 1.0f, 1.0f,
	    12.0f, 10.0f,                                       
	    -1.0f, 2.0f, 0.0f, 
	    0.3f, 0.2f, 0.1f);
  pointLightCount++;
//Aura GOku
  pointLights[1] = PointLight(1.0f, 0.95f, 0.0f,
      2.0f, 5.0f,
      0.0, 43.0, 0.0,
      0.3, 0.2, 0.1f);                        
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

// CARRO Adelante
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
    15.0, 0.2,
    -31.25, 3.125, -112.5,
		0.0f, 0.0f, -1.0f,
    1.0f, 0.05f, 0.01f,
    20.0f);
	spotLightCount++;

// CARRO Atras
	spotLights[2] = SpotLight(1.0f, 0.0f, 0.0f,
		15.f, 0.2f,
    -41.25, 11.25, 75,
		0.0f, 0.0f, 1.0f,
    1.0f, 0.05f, 0.01f,
		20.0f);
	spotLightCount++;

//COFRE carro
	spotLights[3] = SpotLight(0.0f, 1.0f, 0.0f,
    15.0, 0.2,
    1.2, -0.6, -22.0,
		0.0f, -1.0f, 0.0f,
    1.0f, 0.05f, 0.01f,
    22.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


  glm::vec3 lowerLight = camera.getCameraPosition();
	GLfloat now = glfwGetTime();
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


  glm::vec4 posLuzAzulCarro = glm::vec4(-31.25, 3.125, -112.5, 1.0f);
  glm::vec3 dirLuzLocal1 = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
  glm::vec3 dirLuzLocal2 = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
  glm::vec4 posLuzRojaCarro = glm::vec4(-41.25, 11.25, 75, 1.0f);
  glm::vec4 posLuzVerdeCarro = glm::vec4(1.2, -0.6, -22.0, 1.0f);
  glm::vec3 dirLuzLocal3 = glm::vec3(0.0f, -1.0f, 0.0f);

  glm::vec4 posLuzLampara = glm::vec4(-1.0f, 2.0f, 0.0f, 1.0f);
  glm::vec4 posAuraGoku = glm::vec4(0.0, 43.0, 0.0, 1.0f);



  glm::vec3 posFinal;
  glm::vec3 dirFinal;


  unsigned int numPointOn = 0;
  unsigned int numSpotOn = 0;

	while (!mainWindow.getShouldClose()) {
		now = glfwGetTime();
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
	  lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());



		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);



// Points: (0:Lampara) (1:Goku)
// Spots: (0:Camara) (1:FaroAzul) (2:FaroRojo) (3:Cofre)
    numPointOn = 0;
    if (mainWindow.apagarLampara() == false) {
      pointLightsAux[numPointOn] = pointLights[0];
      numPointOn += 1;
    }
    if (mainWindow.elevarKi() == true) {
      pointLightsAux[numPointOn] = pointLights[1];
      numPointOn += 1;
    }

    numSpotOn = 0;
    if (mainWindow.avanzarCarro() == true) {
      spotLightsAux[numSpotOn] = spotLights[1];
      numSpotOn += 1;
    }
    if (mainWindow.retrocederCarro() == true) {
      spotLightsAux[numSpotOn] = spotLights[2];
      numSpotOn += 1;
    }
    if (mainWindow.cofreEstaAbierto() == true) {
      spotLightsAux[numSpotOn] = spotLights[3];
      numSpotOn += 1;
    }
    spotLightsAux[numSpotOn] = spotLights[0];
    numSpotOn++;
  
    shaderList[0].SetPointLights(pointLightsAux, numPointOn);
		shaderList[0].SetSpotLights(spotLightsAux, numSpotOn);






		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


















    // LAMPARA
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(5.0f, 0.0f, 40.0f));
    model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
    modelaux = model;

    
    //posaux = glm::vec3(modelaux[3]);
    //pointLights[0].SetPos( glm::vec3(posaux.x - 7, posaux.y + 10, posaux.z)  );

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    Lampara_M.RenderModel();

    posFinal = glm::vec3(model * posLuzLampara);
    pointLights[0].SetPos(posFinal);





// GOKU ----------------------------------------------------------------
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -40.0f));
    model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
    modelaux = model;

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    Goku_M.RenderModel();

    posFinal = glm::vec3(model * posAuraGoku);
    pointLights[1].SetPos(posFinal);











    // -----------------------------INICIA DIBUJO DEL CARRO ------------------------------------------------ //
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f + mainWindow.getArticulacion2(), 1.725f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		modelaux = model;

    //posaux = glm::vec3(modelaux[3]);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();





posFinal = glm::vec3(model * posLuzAzulCarro);
dirFinal = glm::normalize(glm::mat3(model) * dirLuzLocal1);
spotLights[1].SetFlash(posFinal, dirFinal);



posFinal = glm::vec3(model * posLuzRojaCarro);
dirFinal = glm::normalize(glm::mat3(model) * dirLuzLocal2);
spotLights[2].SetFlash(posFinal, dirFinal);




/*
//glm::vec3 posGlobalDeseada(14.0f, 1.975f, -2.5f);
glm::vec3 posGlobalDeseada(-1, 2.625, -3.3);

// Convierte esa posición global en coordenadas locales:
glm::mat4 invModel = glm::inverse(model);
glm::vec4 posLocal = invModel * glm::vec4(posGlobalDeseada, 1.0f);

//  Imprime el resultado y usa ese valor como posición local fija
std::cout << "posLocal = " 
          << posLocal.x << ", " 
          << posLocal.y << ", " 
          << posLocal.z << std::endl;
*/










		//spotLights[1].SetPos( glm::vec3( model * posLuzAzulCarro)  );
		//spotLights[2].SetPos( glm::vec3(posaux.x + 9.0f, posaux.y + 0.25, posaux.z - 2.5f)  );
    //std::cout << posLuzAux.x << " HOLA " << posLuzAux.y << " LOL " << posLuzAux.z << " xd\n";
		//spotLights[2].SetPos( glm::vec3(10.0f, 2.875f, -5.8f) );
    //std::cout << posaux.x - 6.0f << " HOLA " << posaux.y + 0.9f << " LOL " << posaux.z - 3.3f << " xd\n";


















    model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 15.75f, -50.75f));
    model = glm::rotate(model,  glm::radians(mainWindow.getArticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

posFinal = glm::vec3(model * posLuzVerdeCarro);
dirFinal = glm::normalize(glm::mat3(model) * dirLuzLocal3);
spotLights[3].SetFlash(posFinal, dirFinal);

    //std::cout << dirFinal.x << " ," << dirFinal.y << " ," << dirFinal.z << "\n";





    model = modelaux;
		model = glm::translate(model, glm::vec3(-31.0f, -15.75f, -90.75f));
    model = glm::rotate(model,  glm::radians(mainWindow.getArticulacion2()*5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta1_M.RenderModel();


    model = modelaux;
		model = glm::translate(model, glm::vec3(-31.0f, -15.75f, 55.75f));
    model = glm::rotate(model,  glm::radians(mainWindow.getArticulacion2()*5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta2_M.RenderModel();


    model = modelaux;
		model = glm::translate(model, glm::vec3(33.5f, -15.75f, -90.75f));
    model = glm::rotate(model,  glm::radians(mainWindow.getArticulacion2()*5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta3_M.RenderModel();


    model = modelaux;
		model = glm::translate(model, glm::vec3(33.5f, -15.75f, 55.75f));
    model = glm::rotate(model,  glm::radians(mainWindow.getArticulacion2()*5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta4_M.RenderModel();






		mainWindow.swapBuffers();
	}

	return 0;
}

