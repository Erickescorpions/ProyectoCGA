#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

// Font rendering include
#include "Headers/FontTypeRendering.h"

#include "Enemy.h"
#include "Cube.h"
#include "Player.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"


int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;

// Variables para la introducción
bool iniciaPartida = false, presionarOpcion = false;
bool player1 = false, player2 = false, player3 = false;
GLuint textureActivaID, textureInit1ID, textureInit2ID, textureInit3ID, textureScreenID;

//=========================Variables para el conteno de cubos=====================================
GLuint textureCuboID;
// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;
int cuboContador = 0;
bool cuboAgarrado = false;  // Bandera que indica si el cubo ha sido agarrado
float proximidadUmbral = 3.0f;  // Umbral de proximidad para que el personaje agarre el cubo

// Modelo para renderizar la pantalla de introducción
Box boxIntro;
Box Conteo;
Shader shaderTexture;
Box boxCollider;

std::shared_ptr<ThirdPersonCamera> camera(new ThirdPersonCamera());
float distanceFromPlayer = 6.5f;
float angleTarget = 0.0f;
glm::vec3 positionTarget;

std::shared_ptr<FirstPersonCamera> firstPersonCamera(new FirstPersonCamera()); 	// Cámara en primera persona
bool isFirstPersonActive = false;  												// Para controlar qué cámara está activa

Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);

// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;

// Teletransporte
Model modelCirculoMagico;

// Terrain model instance
Terrain island1(-1, -1, 200, 8, "../textures/heightmap_practica05.png");
Terrain island2(-1, -1, 200, 8, "../textures/heightmap_exercise2.png");
Terrain island3(-1, -1, 200, 8, "../textures/heightmap.png");


GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureIsland1RID, textureIsland1GID, textureIsland1BID, textureIsland1BlendMapID;
GLuint textureIsland2RID, textureIsland2GID, textureIsland2BID, textureIsland2BlendMapID;
GLuint textureIsland3RID, textureIsland3GID, textureIsland3BID, textureIsland3BlendMapID;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { 
		"../skybox_proyecto/front.png",
		"../skybox_proyecto/back.png",
		"../skybox_proyecto/top.png",
		"../skybox_proyecto/bottom.png",
		"../skybox_proyecto/right.png",
		"../skybox_proyecto/left.png"};

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

glm::mat4 matrixModelCirculo = glm::mat4(1.0);
uint8_t modelSelected = 0;
bool enableCountSelected = true;

enum Personaje {
	KAKASHI = 0,
	KRATOS = 1,
	NARUTO = 2
};

uint8_t numeroPersonajesIntercambiar = 3;
uint8_t kakashiState = 0;
uint8_t kratosState = 0;
uint8_t narutoState = 0;

// Lamps position
std::vector<glm::vec3> lamp1Position = {
	glm::vec3(-7.03, 0, -19.14),
	glm::vec3(24.41, 0, -34.57),
	glm::vec3(-10.15, 0, -54.1)
};
std::vector<float> lamp1Orientation = {
	-17.0, -82.67, 23.70
};
std::vector<glm::vec3> lamp2Position = {
	glm::vec3(-36.52, 0, -23.24),
	glm::vec3(-52.73, 0, -3.90)
};
std::vector<float> lamp2Orientation = {
	21.37 + 90, -65.0 + 90
};

double deltaTime;
double currTime, lastTime;

// Modelo para el render del texto
FontTypeRendering::FontTypeRendering *modelText;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);

// Firma del metodo para usar el scroll
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true, Player* jugador = nullptr);
void GenerarTextura(Texture texture, GLuint &textureID);
void RenderTextura(GLuint Cesped, GLuint R, GLuint G, GLuint B, GLuint BlendMap);
bool checkCollision(const AbstractModel::OBB& box1, const AbstractModel::OBB& box2);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scrollCallback);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../Shaders/terrain.vs", "../Shaders/terrain.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");


	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);

	//Lamps models
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.loadModel("../models/Street_Light/LampPost.obj");
	modelLampPost2.setShader(&shaderMulLighting);


	// Circulo de transporte
	modelCirculoMagico.loadModel("../models/teletransportador/circulo_magico.obj");
	modelCirculoMagico.setShader(&shaderMulLighting);

	// Inicialización del boxIntro para la introducción
	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	// Terreno
	island1.init();
	island1.setShader(&shaderTerrain);
	island2.init();
	island2.setShader(&shaderTerrain);
	island3.init();
	island3.setShader(&shaderTerrain);

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	firstPersonCamera->setPosition(glm::vec3(0.0f, 6.0f, 8.0f));
	
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// Cesped
	Texture textureCesped("../textures/grassy2.png");
	GenerarTextura(textureCesped, textureCespedID);

	// Island1
	Texture texture1R("../textures/tierra.png");
	GenerarTextura(texture1R, textureIsland1RID);
	Texture texture1G("../textures/flores.png");
	GenerarTextura(texture1G, textureIsland1GID);
	Texture texture1B("../textures/path.png");
	GenerarTextura(texture1B, textureIsland1BID);
	Texture texture1BlendMap("../textures/blendMap_Practica04.png");
	GenerarTextura(texture1BlendMap, textureIsland1BlendMapID);

	// Island2
	Texture texture2R("../textures/tierra.png");
	GenerarTextura(texture2R, textureIsland2RID);
	Texture texture2G("../textures/flores.png");
	GenerarTextura(texture2G, textureIsland2GID);
	Texture texture2B("../textures/path.png");
	GenerarTextura(texture2B, textureIsland2BID);
	Texture texture2BlendMap("../textures/blendMap.png");
	GenerarTextura(texture2BlendMap, textureIsland2BlendMapID);

	// Island3
	Texture texture3R("../textures/tierra.png");
	GenerarTextura(texture3R, textureIsland3RID);
	Texture texture3G("../textures/flores.png");
	GenerarTextura(texture3G, textureIsland3GID);
	Texture texture3B("../textures/path.png");
	GenerarTextura(texture3B, textureIsland3BID);
	Texture texture3BlendMap("../textures/blendMap_Practica05.png");
	GenerarTextura(texture3BlendMap, textureIsland3BlendMapID);

	Texture textureIntro1("../textures/kakashi.png");
	GenerarTextura(textureIntro1, textureInit1ID);
	Texture textureIntro2("../textures/kratos.png");
	GenerarTextura(textureIntro2, textureInit2ID);
	Texture textureIntro3("../textures/naruto.png");
	GenerarTextura(textureIntro3, textureInit3ID);
	// Texture textureScreen("../textures/Screen.png");
	// GenerarTextura(textureScreen, textureScreenID);
	textureActivaID = textureInit1ID;

	Texture textureCubo("../texture/cubo.png");
	GLuint textureCuboID;
	// Generar y cargar la textura
	GenerarTextura(textureCubo, textureCuboID);
	// Enlazar la textura
	glBindTexture(GL_TEXTURE_2D, textureCuboID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Ajustes de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Ajustes de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtro de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtro de maximización

	// Se inicializa el model de render text
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();
}

// Verificar si dos AABBs se superponen
bool checkCollision(const AbstractModel::OBB& box1, const AbstractModel::OBB& box2) {
    // Comprueba si las cajas no se superponen en cualquiera de los ejes
    if (std::abs(box1.c.x - box2.c.x) > (box1.e.x + box2.e.x)) return false;
    if (std::abs(box1.c.y - box2.c.y) > (box1.e.y + box2.e.y)) return false;
    if (std::abs(box1.c.z - box2.c.z) > (box1.e.z + box2.e.z)) return false;

    return true; // Las cajas se superponen
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderTexture.destroy();
	boxIntro.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	esfera1.destroy();

	// Custom objects Delete
	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLampPost2.destroy();
	modelCirculoMagico.destroy();

	// Terrains objects Delete
	island1.destroy();
	island2.destroy();
	island3.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureIsland1BID);
	glDeleteTextures(1, &textureIsland1GID);
	glDeleteTextures(1, &textureIsland1RID);
	glDeleteTextures(1, &textureIsland1BlendMapID);
	glDeleteTextures(1, &textureIsland2BID);
	glDeleteTextures(1, &textureIsland2GID);
	glDeleteTextures(1, &textureIsland2RID);
	glDeleteTextures(1, &textureIsland2BlendMapID);
	glDeleteTextures(1, &textureIsland3BID);
	glDeleteTextures(1, &textureIsland3GID);
	glDeleteTextures(1, &textureIsland3RID);
	glDeleteTextures(1, &textureIsland3BlendMapID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Texturas de la introducción
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureInit1ID);
	glDeleteTextures(1, &textureInit2ID);
	glDeleteTextures(1, &textureInit3ID);
	glDeleteTextures(1, &textureScreenID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                exitApp = true;
                break;
            // Alternar entre la cámara en tercera persona y la cámara en primera persona
            case GLFW_KEY_K:
                if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
                    isFirstPersonActive = !isFirstPersonActive;  // Apagar, encender la cámara
                }
                break;
        }
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	static bool firstMouse = true;
    if (firstMouse) {
        lastMousePosX = xpos;
        lastMousePosY = ypos;
        firstMouse = false;
    }

    offsetX = xpos - lastMousePosX;
    offsetY = ypos - lastMousePosY;
    lastMousePosX = xpos;
    lastMousePosY = ypos;

    camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	firstPersonCamera->mouseMoveCamera(offsetX, offsetY, deltaTime);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	distanceFromPlayer -= yoffset;
	camera->setDistanceFromTarget(distanceFromPlayer);
}

bool processInput(bool continueApplication, Player* jugador) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if(!iniciaPartida){
    bool presionarEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
    if(presionarEnter){
        iniciaPartida = true;
        // Set modelSelected based on the active texture before changing textureActivaID
        if(textureActivaID == textureInit1ID){
            modelSelected = Personaje::KAKASHI;
					jugador->setJugador("kakashi");
        }
        else if(textureActivaID == textureInit2ID){
            modelSelected = Personaje::KRATOS;
					jugador->setJugador("kratos");
        }
        else if(textureActivaID == textureInit3ID){
            modelSelected = Personaje::NARUTO;
					jugador->setJugador("naruto");
        }
        textureActivaID = textureCuboID;
    }
    else if(!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        presionarOpcion = true;
        if(textureActivaID == textureInit1ID){
            textureActivaID = textureInit2ID;
            modelSelected = Personaje::KRATOS;
        }
        else if(textureActivaID == textureInit2ID){
            textureActivaID = textureInit3ID;
            modelSelected = Personaje::NARUTO;
        }
        else if(textureActivaID == textureInit3ID){
            textureActivaID = textureInit1ID;
            modelSelected = Personaje::KAKASHI;
        }
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
        presionarOpcion = false;

    // Important to return here to prevent other inputs from interfering
    return continueApplication;
}

	// Si la cámara en primera persona está activa
    if (isFirstPersonActive) {
        // Factor de velocidad para acelerar el movimiento de la cámara
		float speedMultiplier = 2.0f; // Para aumentar la velocidad
		float superSpeedMultiplier = 5.0f;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			firstPersonCamera->moveFrontCamera(true, deltaTime * speedMultiplier);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			firstPersonCamera->moveFrontCamera(false, deltaTime * speedMultiplier);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			firstPersonCamera->moveRightCamera(false, deltaTime * speedMultiplier);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			firstPersonCamera->moveRightCamera(true, deltaTime * speedMultiplier);
		
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				firstPersonCamera->moveFrontCamera(true, deltaTime * superSpeedMultiplier);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				firstPersonCamera->moveFrontCamera(false, deltaTime * superSpeedMultiplier);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				firstPersonCamera->moveRightCamera(false, deltaTime * superSpeedMultiplier);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				firstPersonCamera->moveRightCamera(true, deltaTime * superSpeedMultiplier);
		}
    }

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0f, deltaTime);

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) 
		camera->mouseMoveCamera(0.0f, offsetY, deltaTime);

	offsetX = 0;
	offsetY = 0;
	kakashiState = 0;
	kratosState = 0;
	narutoState = 0;

	// Controles para mover izquierda y a la derecha
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		angleTarget += 0.02f;
		jugador->modelMatrix = glm::rotate(jugador->modelMatrix, 0.02f, glm::vec3(0, 1, 0));
	} 

	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		angleTarget -= 0.02f;
		jugador->modelMatrix = glm::rotate(jugador->modelMatrix, -0.02f, glm::vec3(0, 1, 0));
	}

	// Controles para ir adelante y atras
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			kakashiState = 2; // Estado de correr
			jugador->modelMatrix = glm::translate(jugador->modelMatrix, glm::vec3(0.0, 0.0, 20.0));
		} else {
			kakashiState = 1; // Estado de caminar
			jugador->modelMatrix = glm::translate(jugador->modelMatrix, glm::vec3(0.0, 0.0, 5.0));
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		kakashiState = 3;
		jugador->modelMatrix = glm::translate(jugador->modelMatrix, glm::vec3(0.0, 0.0, -5.0));
	}

	glfwPollEvents();
	return continueApplication;
}


void renderContador(GLuint textureCuboID, FontTypeRendering::FontTypeRendering *modelText, const std::string& text) {
    // 1. Renderiza la textura (el cubo)
    glBindTexture(GL_TEXTURE_2D, textureCuboID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Define el Quad con la textura
    float quadVertices[] = {
        1.0f,  1.0f, 0.0f,    1.0f, 1.0f, // Arriba derecha
       -1.0f,  1.0f, 0.0f,    0.0f, 1.0f, // Arriba izquierda
       -1.0f, -1.0f, 0.0f,    0.0f, 0.0f, // Abajo izquierda
        1.0f, -1.0f, 0.0f,    1.0f, 0.0f  // Abajo derecha
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Configuración de atributos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Renderiza la textura
    glBindTexture(GL_TEXTURE_2D, textureCuboID);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // 2. Renderiza el texto
    // Aquí es donde se utiliza el modelo de texto para renderizar el texto.
    modelText->render(text, 0.9f, 0.9f);  // Posiciona el texto en la parte superior derecha
}


void applicationLoop() {
	bool psi = true;

	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	lastTime = TimeManager::Instance().GetTime();

	// Inicializacoin de valores de la camara
	camera->setSensitivity(1.2f);
	camera->setDistanceFromTarget(distanceFromPlayer);

	Player jugador = Player(&shaderMulLighting);
	jugador.setTerrain(&island1);

	Cube cube("../models/cubo/cubo.fbx", &shaderMulLighting, glm::vec3(0.0f, 0.0f, 0.0f));
	cube.setTerrain(&island1);  

	glm::vec3 posicionEnemigo = glm::vec3(0.0f, 3.0f, 0.0f);
	// Creamos un enemigo
	Enemy enemigo = Enemy("../models/enemy/Zombie1.fbx", &shaderMulLighting, posicionEnemigo, 15.0f);
	enemigo.setTerrain(&island1);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true, &jugador);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Renderizar la introducción si aún no ha comenzado la partida
		if(!iniciaPartida) {
			// Configuración de matrices de proyección y vista para la introducción
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));

			// Enlazar y activar la textura activa
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);

			// Renderizar el boxIntro
			boxIntro.render();

			// Intercambiar buffers y continuar al siguiente ciclo
			glfwSwapBuffers(window);
			continue;
		}


		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 1000.0f);

		positionTarget = jugador.modelMatrix[3];
		positionTarget.y += 3.0f;

		glm::mat4 view;
		if (isFirstPersonActive) {
            view = firstPersonCamera->getViewMatrix();  // Usar cámara en primera persona
        } else {
            camera->setCameraTarget(positionTarget);
			camera->setAngleTarget(angleTarget);
			camera->updateCamera();
			view = camera->getViewMatrix();
        }

		// Después de actualizar la cámara
        float angleAroundTarget = camera->getAngleAroundTarget();
        float maxAngleDifference = glm::radians(60.0f); // Ángulo máximo antes de rotar el personaje
        float characterRotation = 0.0f;

        if (angleAroundTarget > maxAngleDifference) {
            characterRotation = angleAroundTarget - maxAngleDifference;
        } else if (angleAroundTarget < -maxAngleDifference) {
            characterRotation = angleAroundTarget + maxAngleDifference;
        }

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderTerrain.setInt("spotLightCount", 1);
		glm::vec3 spotPosition = glm::vec3(jugador.modelMatrix * glm::vec4(0.0, 0.2, 1.75, 1.0));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.07);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		shaderTerrain.setFloat("spotLights[0].linear", 0.07);
		shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
		shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size() + lamp2Position.size());
		shaderTerrain.setInt("pointLightCount", lamp1Position.size() + lamp2Position.size());
		for(int i = 0; i < lamp1Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.0, 10.35, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}
		for(int i = 0; i < lamp2Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.75, 5.0, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
		}

		/*******************************************
		 * Terrain Cesped
		 *******************************************/
		RenderTextura(textureCespedID, textureIsland1RID, textureIsland1GID, textureIsland1BID, textureIsland1BlendMapID);
		//glDisable(GL_CULL_FACE);
		island1.setPosition(glm::vec3(100, 0, 100));
		island1.render();
		//glEnable(GL_CULL_FACE);

		RenderTextura(textureCespedID, textureIsland2RID, textureIsland2GID, textureIsland2BID, textureIsland2BlendMapID);
		//glDisable(GL_CULL_FACE);
		island2.setPosition(glm::vec3(100, 150, 100));
		island2.setOrientation(glm::vec3(0, 0, 45));
		island2.render();
		//glEnable(GL_CULL_FACE);

		RenderTextura(textureCespedID, textureIsland3RID, textureIsland3GID, textureIsland3BID, textureIsland3BlendMapID);
		//glDisable(GL_CULL_FACE);
		island3.setPosition(glm::vec3(100, 150, 100));
		island3.setOrientation(glm::vec3(0, 0, 135));
		island3.render();
		//glEnable(GL_CULL_FACE);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Custom objects obj
		 *******************************************/
		//Magic Circle Rende
		matrixModelCirculo[3][1] = island1.getHeightTerrain(matrixModelCirculo[3][0], matrixModelCirculo[3][2]);
		matrixModelCirculo = glm::translate(matrixModelCirculo, glm::vec3(0.0, 0.0, 0.0));
		modelCirculoMagico.render(matrixModelCirculo);

		matrixModelCirculo[3][1] = island2.getHeightTerrain(matrixModelCirculo[3][0], matrixModelCirculo[3][2]);
		matrixModelCirculo = glm::translate(matrixModelCirculo, glm::vec3(0.0, 150.0, 0.0));
		matrixModelCirculo = glm::rotate(matrixModelCirculo, 0.45f, glm::vec3(0, 0, 1));
		modelCirculoMagico.render(matrixModelCirculo);

		matrixModelCirculo[3][1] = island3.getHeightTerrain(matrixModelCirculo[3][0], matrixModelCirculo[3][2]);
		matrixModelCirculo = glm::translate(matrixModelCirculo, glm::vec3(0.0, 150.0, 0.0));
		//matrixModelCirculo = glm::rotate(matrixModelCirculo, 0.45f, glm::vec3(0, 0, 1));
		modelCirculoMagico.render(matrixModelCirculo);

		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		// Render lamp
		for(int i = 0; i < lamp1Position.size(); i++){
			lamp1Position[i].y = island1.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z);
			modelLamp1.setPosition(lamp1Position[i]);
			modelLamp1.setScale(glm::vec3(0.5));
			modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
			modelLamp1.render();
		}
		for(int i = 0; i < lamp2Position.size(); i++){
			lamp2Position[i].y = island1.getHeightTerrain(lamp2Position[i].x, lamp2Position[i].z);
			modelLamp2.setPosition(lamp2Position[i]);
			modelLamp2.setScale(glm::vec3(0.5));
			modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
			modelLamp2.render();
			modelLampPost2.setPosition(lamp2Position[i]);
			modelLampPost2.setScale(glm::vec3(0.5));
			modelLampPost2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
			modelLampPost2.render();
		}
		
		/*****************************************
		 * Jugador
		 * **************************************/
		// Renderizamos al jugador
		jugador.render();

		glm::vec3 posicionJugador = jugador.modelMatrix[3];

		enemigo.update(deltaTime, posicionJugador);
		enemigo.render();
		//=======================================================Contador para los fragmentos recogido===========================================================
		// Actualiza la lógica del cubo (flotación, rotación)
		cube.update(deltaTime, posicionJugador);
		// Obtener la posición del cubo
		glm::vec3 posicionCubo = glm::vec3(cube.modelMatrix[3]); // O usar otro método si modelMatrix no tiene la posición directamente
		float distancia = glm::distance(posicionJugador, posicionCubo);
		if (distancia < proximidadUmbral && !cuboAgarrado) {
			cuboAgarrado = true;
			cuboContador++; 
		}
		if (!cuboAgarrado) {
			cube.render(); 
		}
		std::string text = std::to_string(cuboContador) + "/5";
		renderContador(textureCuboID, modelText, text); // Actualiza el contador en pantalla


		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		// //========================================Collider para los personajes========================================================
		// glm::mat4 modelMatrixColliderKakashi = glm::mat4(modelMatrixKakashi); // Copia de la matriz de kakashi
		// AbstractModel::OBB kakashiCollider;
		// // Establece la orientación del collider
		// kakashiCollider.u = glm::quat_cast(modelMatrixKakashi);
		// // Escalar el collider según las dimensiones del cubo
		// modelMatrixColliderKakashi = glm::scale(modelMatrixKakashi, glm::vec3(1.5f, 1.5f, 1.5f)); // Ajusta la escala si es necesario
		// // Traducir el collider al centro del modelo
		// modelMatrixColliderKakashi = glm::translate(modelMatrixKakashi,
		// 	glm::vec3(modelKakashiCorriendo.getObb().c.x,  // Centro en X del modelo
		// 			modelKakashiCorriendo.getObb().c.y,  // Centro en Y del modelo
		// 			modelKakashiCorriendo.getObb().c.z)  // Centro en Z del modelo
		// );
		// // Establecer el centro del collider
		// kakashiCollider.c = glm::vec3(modelMatrixColliderKakashi[3]);
		// // Establecer la escala del collider según las dimensiones originales del OBB del modelo y la escala aplicada
		// kakashiCollider.e = modelKakashiCorriendo.getObb().e * glm::vec3(1.0f, 1.0f, 1.0f); // Ajusta según la escala aplicada
		// // Actualizar o agregar el collider del cubo en el mapa de colliders
		// addOrUpdateColliders(collidersOBB, "kakashi", kakashiCollider, modelMatrixKakashi);

		// // Collider para el enemigo
		// glm::mat4 modelMatrixColliderEnemigo = glm::mat4(enemigo.modelMatrix); // Copia de la matriz del enemigo
		// AbstractModel::OBB enemigoCollider;
		// // Establece la orientación del collider
		// enemigoCollider.u = glm::quat_cast(modelMatrixColliderEnemigo);
		// // Escalar el collider según las dimensiones del enemigo (ajustar según sea necesario)
		// modelMatrixColliderEnemigo = glm::scale(modelMatrixColliderEnemigo, glm::vec3(1.0f, 1.0f, 1.0f)); // Ajustar escala
		// // Traducir el collider al centro del modelo del enemigo
		// modelMatrixColliderEnemigo = glm::translate(modelMatrixColliderEnemigo,
		// 	glm::vec3(enemigo.modelMatrix[3][0],  // Centro en X del enemigo
		// 			enemigo.modelMatrix[3][1],  // Centro en Y del enemigo
		// 			enemigo.modelMatrix[3][2])  // Centro en Z del enemigo
		// );
		// // Establecer el centro del collider
		// enemigoCollider.c = glm::vec3(modelMatrixColliderEnemigo[3]);
		// // Establecer la escala del collider según las dimensiones originales del OBB del enemigo y la escala aplicada
		// enemigoCollider.e = glm::vec3(1.0f, 1.0f, 1.0f); // Ajusta según la escala aplicada
		// // Actualizar o agregar el collider del enemigo en el mapa de colliders
		// addOrUpdateColliders(collidersOBB, "enemigo", enemigoCollider, modelMatrixColliderEnemigo);

		//=====================================Colisiones===================================================
		// AbstractModel::OBB kakashiCollideUpdate = std::get<0>(collidersOBB["kakashi"]);
		// AbstractModel::OBB enemigoColliderUpdate = std::get<0>(collidersOBB["enemigo"]);
		// // Calcular la distancia entre los centros de los collide	
		// glm::vec3 direction = enemigoColliderUpdate.c - kakashiCollideUpdate.c;
		// float distance = glm::length(direction);
		// // Sumar los radios (la mitad de las dimensiones de cada collider)
		// float combinedRadius = kakashiCollideUpdate.e.x + enemigoColliderUpdate.e.x;  // En el caso de un AABB, se suman las dimensiones
		// // Si la distancia entre los centros es menor que la suma de los radios, hay colisión
		// if (distance < combinedRadius) {
		// 	// Si hay colisión, normalizamos la dirección de colisión y movemos al enemigo fuera de la colisión
		// 	glm::vec3 moveDirection = glm::normalize(direction);
		// 				// Calculamos el solapamiento y lo movemos fuera de la colisión
		// 	float overlap = combinedRadius - distance;
		// 	enemigo.modelMatrix[3] += moveDirection * overlap;  // Mueve al enemigo fuera del collider de Kakashi
		// }
		

		// shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		// shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, textureActivaID);
		// shaderTexture.setInt("outTexture", 0);
		// glEnable(GL_BLEND);
		// boxIntro.render();
		// glDisable(GL_BLEND);

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}

void GenerarTextura(Texture texture, GLuint &textureID){
	// Definiendo la textura
	texture.loadImage(); // Cargar la textura
	glGenTextures(1, &textureID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(texture.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texture.getChannels() == 3 ? GL_RGB : GL_RGBA, texture.getWidth(), texture.getHeight(), 0,
		texture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	texture.freeImage(); // Liberamos memoria
}

void RenderTextura(GLuint Cesped, GLuint R, GLuint G, GLuint B, GLuint BlendMap){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Cesped);
			shaderTerrain.setInt("backgroundTexture", 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, R);
			shaderTerrain.setInt("rTexture", 1);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, G);
			shaderTerrain.setInt("gTexture", 2);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, B);
			shaderTerrain.setInt("bTexture", 3);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, BlendMap);
			shaderTerrain.setInt("blendMapTexture", 4);
			shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));
}