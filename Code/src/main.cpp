#define _USE_MATH_DEFINES
#define TIEMPO_CARGA_INTRO 0.1

#include <cmath>
// glew include
#include <GL/glew.h>

// std includes
#include <string>
#include <iostream>
#include <random>

// glfw include
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

// GLM include
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

#include <AL/al.h>
#include <AL/alc.h>
#include <fstream>
#include <vector>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

// Font rendering include
#include "Headers/FontTypeRendering.h"

#include "Enemy.h"
#include "Cube.h"
#include "Player.h"
#include "AudioManager.h"
#include "Contador.h"
#include "Constants.h"

#include "CollidersController.h"

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
// Shader con skybox
Shader shaderSkybox;
// Shader con multiples luces
Shader shaderMulLighting;
// Shader para el terreno
Shader shaderTerrain;
// Shader para el jugador
Shader shaderJugador;
// Shader para el agua
Shader shaderAgua;

// Variables para la introducción
bool iniciaPartida = false, presionarOpcion = false;
GLuint textureActivaID, textureInit1ID, textureInit2ID, textureInit3ID, textureLoseID, textureWinID;
GLuint texturaBarraVidaID;

//=========================Variables para el conteno de cubos=====================================
GLuint textureCuboID;
// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>> collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>> collidersSBB;

// Modelo para renderizar la pantalla de introducción
Box boxIntro;
Box boxBarraVida;
Box Conteo;
Shader shaderTexture;

std::shared_ptr<ThirdPersonCamera> camera(new ThirdPersonCamera());
float distanceFromPlayer = 6.5f;
float angleTarget = 0.0f;
glm::vec3 positionTarget;

std::shared_ptr<FirstPersonCamera> firstPersonCamera(new FirstPersonCamera()); // Cámara en primera persona
bool isFirstPersonActive = false;																							 // Para controlar qué cámara está activa

Sphere skyboxSphere(20, 20);

// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;

// Teletransporte
Model modelCirculoMagico;

// Modelos Isla2
Model modelCasa1, modelCasa2, modelCasa3, modelCasa4, modelCasa5, modelCasa6, modelCasa7, modelCasa8, modelCasa9, modelCasa10;
Model modelArbol1I1, modelArbol2I1, modelArbol3I1, modelArbol4I1, modelArbol5I1;
Model modelArbol1I2, modelArbol2I2, modelArbol3I2, modelArbol4I2, modelArbol5I2, modelArbol6I2;
Model modelPuente;

// Terrain model instance
Terrain island1(-1, -1, 200, 5, "../textures/islas/heightmap1.png");
Terrain island2(-1, -1, 200, 100, "../textures/islas/heightmap2.png");
Terrain island3(-1, -1, 200, 10, "../textures/islas/heightmap3.png");

// Definir texturas de islas
GLuint textureCespedID1, textureCespedID2, textureCespedID3;
GLuint textureIsland1RID, textureIsland1GID, textureIsland1BID, textureIsland1BlendMapID;
GLuint textureIsland2RID, textureIsland2GID, textureIsland2BID, textureIsland2BlendMapID;
GLuint textureIsland3RID, textureIsland3GID, textureIsland3BID, textureIsland3BlendMapID;
GLuint skyboxTextureID1, skyboxTextureID2, skyboxTextureID3;

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Definir matrices para modelos
glm::mat4 matrixModelCirculo1 = glm::mat4(1.0);
glm::mat4 matrixModelCirculo2 = glm::mat4(1.0);
glm::mat4 matrixModelCirculo3 = glm::mat4(1.0);
glm::mat4 matrixModelCasa1 = glm::mat4(1.0);
glm::mat4 matrixModelCasa2 = glm::mat4(1.0);
glm::mat4 matrixModelCasa3 = glm::mat4(1.0);
glm::mat4 matrixModelCasa4 = glm::mat4(1.0);
glm::mat4 matrixModelCasa5 = glm::mat4(1.0);
glm::mat4 matrixModelCasa6 = glm::mat4(1.0);
glm::mat4 matrixModelCasa7 = glm::mat4(1.0);
glm::mat4 matrixModelCasa8 = glm::mat4(1.0);
glm::mat4 matrixModelCasa9 = glm::mat4(1.0);
glm::mat4 matrixModelCasa10 = glm::mat4(1.0);
glm::mat4 matrixModelCasa11 = glm::mat4(1.0);
glm::mat4 matrixModelCasa12 = glm::mat4(1.0);
glm::mat4 matrixModelCasa13 = glm::mat4(1.0);
glm::mat4 matrixModelCasa14 = glm::mat4(1.0);
glm::mat4 matrixModelCasa15 = glm::mat4(1.0);
glm::mat4 matrixModelPuente = glm::mat4(1.0);

bool enableCountSelected = true;

// Posicion de las lamparas
std::vector<glm::vec3> lamp1Position = {
		glm::vec3(0, 0, -19.14),
		glm::vec3(24.41, 0, -34.57),
		glm::vec3(-10.15, 0, -54.1)};
std::vector<float> lamp1Orientation = {
		-17.0, -82.67, 23.70};

// Posición de árboles ISLA 1
#include <random>

// Generar bosque en isla 1
std::vector<glm::vec3> generarBosque(int cantidadArboles, float minX, float maxX, float minZ, float maxZ) {
    std::vector<glm::vec3> posiciones;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distZ(minZ, maxZ);

    for (int i = 0; i < cantidadArboles; ++i) {
        float x = distX(gen);
        float z = distZ(gen);
        // Asignar la altura del terreno
        float y = island1.getHeightTerrain(x, z);
        posiciones.push_back(glm::vec3(x, y, z));
    }
    return posiciones;
}

// Generar bosque para los árboles de la isla 1
std::vector<glm::vec3> arbol1_Isla1_Position = generarBosque(10, -90.0f, 90.0f, -90.0f, 90.0f);
std::vector<float> arbol1_Isla1_Orientation = {15.0, 25.0, 35.0, 45.0, 55.0, 65.0};
std::vector<glm::vec3> arbol2_Isla1_Position = generarBosque(10, -90.0f, 90.0f, -90.0f, 90.0f);
std::vector<float> arbol2_Isla1_Orientation = {15.0, 25.0, 35.0, 45.0, 55.0, 65.0};
std::vector<glm::vec3> arbol3_Isla1_Position = generarBosque(10, -90.0f, 90.0f, -90.0f, 90.0f);
std::vector<float> arbol3_Isla1_Orientation = {15.0, 25.0, 35.0, 45.0, 55.0, 65.0};
std::vector<glm::vec3> arbol4_Isla1_Position = generarBosque(10, -90.0f, 90.0f, -90.0f, 90.0f);
std::vector<float> arbol4_Isla1_Orientation = {15.0, 25.0, 35.0, 45.0, 55.0, 65.0};
std::vector<glm::vec3> arbol5_Isla1_Position = generarBosque(10, -90.0f, 90.0f, -90.0f, 90.0f);
std::vector<float> arbol5_Isla1_Orientation = {15.0, 25.0, 35.0, 45.0, 55.0, 65.0};

// Posición de árboles ISLA 2
std::vector<glm::vec3> arbol1_Isla2_Position = {
		glm::vec3(15.0, 0.0, -25.0),
		glm::vec3(85.0, 0.0, -30.0),
		glm::vec3(34.0, 0.0, -35.0),
		glm::vec3(71.0, 0.0, -50.0),
		glm::vec3(62.0, 0.0, -45.0),
		glm::vec3(43.0, 0.0, -60.0)};
std::vector<float> arbol1_Isla2_Orientation = {15.0, 25.0, 35.0, 45.0, 55.0, 65.0};

std::vector<glm::vec3> arbol2_Isla2_Position = {
		glm::vec3(21.0, 0.0, -28.0),
		glm::vec3(80.0, 0.0, -35.0),
		glm::vec3(30.0, 0.0, -42.0),
		glm::vec3(40.0, 0.0, -48.0),
		glm::vec3(35.0, 0.0, -65.0),
		glm::vec3(45.0, 0.0, -75.0)};
std::vector<float> arbol2_Isla2_Orientation = {20.0, 30.0, 40.0, 50.0, 60.0, 70.0};

std::vector<glm::vec3> arbol3_Isla2_Position = {
		glm::vec3(50.0, 0.0, -25.0),
		glm::vec3(60.0, 0.0, -40.0),
		glm::vec3(70.0, 0.0, -55.0),
		glm::vec3(66.0, 0.0, -47.0),
		glm::vec3(75.0, 0.0, -63.0),
		glm::vec3(80.0, 0.0, -70.0)};
std::vector<float> arbol3_Isla2_Orientation = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0};

std::vector<glm::vec3> arbol4_Isla2_Position = {
		glm::vec3(31.0, 0.0, -65.0),
		glm::vec3(42.0, 0.0, -69.0),
		glm::vec3(53.0, 0.0, -74.0),
		glm::vec3(64.0, 0.0, -23.0),
		glm::vec3(75.0, 0.0, -37.0),
		glm::vec3(86.0, 0.0, -84.0)};
std::vector<float> arbol4_Isla2_Orientation = {0.0, 15.0, 30.0, 45.0, 60.0, 75.0};

std::vector<glm::vec3> arbol5_Isla2_Position = {
		glm::vec3(28.0, 0.0, -24.0),
		glm::vec3(47.0, 0.0, -38.0),
		glm::vec3(50.0, 0.0, -20.0),
		glm::vec3(60.0, 0.0, -55.0),
		glm::vec3(20.0, 0.0, -35.0),
		glm::vec3(31.0, 0.0, -80.0)};
std::vector<float> arbol5_Isla2_Orientation = {12.0, 24.0, 36.0, 48.0, 60.0, 72.0};

std::vector<glm::vec3> arbol6_Isla2_Position = {glm::vec3(-7.03, 0.0, -19.14)};
std::vector<float> arbol6_Isla2_Orientation = {21.37};

double deltaTime;
double currTime, lastTime;

// Modelo para el render del texto
FontTypeRendering::FontTypeRendering *modelText;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);

// Firma del metodo para usar el scroll
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true, Player *jugador = nullptr);
bool processInputController(bool continueApplication, Player *jugador = nullptr);
void GenerarTextura(Texture texture, GLuint &textureID);
void RenderTextura(GLuint Cesped, GLuint R, GLuint G, GLuint B, GLuint BlendMap);
bool checkCollision(const AbstractModel::OBB &box1, const AbstractModel::OBB &box2);
void luces(const Player &jugador, glm::mat4 projection, glm::mat4 view);
void changeIsland(int targetIsland, Player &jugador, Enemy &enemigo);
void loadModels();
void renderModel(Model &model, const glm::vec3 &position, const glm::vec3 &scale, float rotation, Terrain *terrain = nullptr);
void renderArboles(Model &model, std::vector<glm::vec3> &positions, const std::vector<float> &orientations, Terrain &terrain);
GLuint loadCubemapTextures(std::string fileNames[6]);
void enforceMapLimits(glm::mat4 &modelMatrix);

// Isla Activa inicialmente
bool isIsland1Active = true;
bool isIsland2Active = false;
bool isIsland3Active = false;

int activeSkybox = 1; // 1 para isla1, 2 para isla2, 3 para isla3

const float MAP_MIN_X = -95.0f; // Límite mínimo en X
const float MAP_MAX_X = 95.0f;	// Límite máximo en X
const float MAP_MIN_Z = -95.0f; // Límite mínimo en Z
const float MAP_MAX_Z = 95.0f;	// Límite máximo en Z

GLuint introTextures[5];
int currentIntroImage = 0;
double introStartTime = 0.0;
bool showIntro = true;
bool seleccionPersonaje = false;

double countdownStartTime = 0.0;
double countdownDuration = 600.0f; // 5 minutos

bool gameOver = false;
bool gameOverMusicPlayed = false;                // Para saber si el juego terminó
float gameOverDuration = 8.0f;       // Duración de la pantalla final en segundos
float gameOverTimer = 0.0f;          // Temporizador para la pantalla final
bool gameWon = false; 

// float teleportCooldown = 0.0f;
// float teleportCooldownTime = 2.0f; // 2 segundos de enfriamiento después de teletransportarse

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr, nullptr);

	if (window == nullptr)
	{
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
	if (GLEW_OK != err)
	{
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
	shaderJugador.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLightsPlayer.fs");
	shaderAgua.initialize("../Shaders/agua.vs", "../Shaders/agua.fs");

	// Inicializando modelos
	loadModels();

	// Definiendo posicion inicial de las camaras
	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	firstPersonCamera->setPosition(glm::vec3(0.0f, 6.0f, 8.0f));

	// Cargar skybox para isla 1
	std::string fileNames1[6] = {
			"../skybox_proyecto/isla1/front.png",
			"../skybox_proyecto/isla1/back.png",
			"../skybox_proyecto/isla1/top.png",
			"../skybox_proyecto/isla1/bottom.png",
			"../skybox_proyecto/isla1/right.png",
			"../skybox_proyecto/isla1/left.png"};
	skyboxTextureID1 = loadCubemapTextures(fileNames1);

	// Cargar skybox para isla 2
	std::string fileNames2[6] = {
			"../skybox_proyecto/isla2/front.png",
			"../skybox_proyecto/isla2/back.png",
			"../skybox_proyecto/isla2/up.png",
			"../skybox_proyecto/isla2/down.png",
			"../skybox_proyecto/isla2/right.png",
			"../skybox_proyecto/isla2/left.png"};
	skyboxTextureID2 = loadCubemapTextures(fileNames2);

	// Cargar skybox para isla 3
	std::string fileNames3[6] = {
			"../skybox_proyecto/isla3/front.png",
			"../skybox_proyecto/isla3/back.png",
			"../skybox_proyecto/isla3/up.png",
			"../skybox_proyecto/isla3/down.png",
			"../skybox_proyecto/isla3/right.png",
			"../skybox_proyecto/isla3/left.png"};
	skyboxTextureID3 = loadCubemapTextures(fileNames3);

	// Island1
	Texture textureCesped1("../textures/islas/pasto1.png");
	GenerarTextura(textureCesped1, textureCespedID1);
	Texture texture1R("../textures/islas/tierra1.png");
	GenerarTextura(texture1R, textureIsland1RID);
	Texture texture1G("../textures/islas/flores1.png");
	GenerarTextura(texture1G, textureIsland1GID);
	Texture texture1B("../textures/islas/pasto1.png");
	GenerarTextura(texture1B, textureIsland1BID);
	Texture texture1BlendMap("../textures/islas/blendMap1.png");
	GenerarTextura(texture1BlendMap, textureIsland1BlendMapID);

	// Island2
	Texture textureCesped2("../textures/islas/pasto2.png");
	GenerarTextura(textureCesped2, textureCespedID2);
	Texture texture2R("../textures/islas/tierra2.png");
	GenerarTextura(texture2R, textureIsland2RID);
	Texture texture2G("../textures/islas/agua.png");
	GenerarTextura(texture2G, textureIsland2GID);
	Texture texture2B("../textures/islas/camino2.png");
	GenerarTextura(texture2B, textureIsland2BID);
	Texture texture2BlendMap("../textures/islas/blendMap2.png");
	GenerarTextura(texture2BlendMap, textureIsland2BlendMapID);

	// Island3
	Texture textureCesped3("../textures/islas/pasto3.png");
	GenerarTextura(textureCesped3, textureCespedID3);
	Texture texture3R("../textures/islas/tierra3.png");
	GenerarTextura(texture3R, textureIsland3RID);
	Texture texture3G("../textures/islas/pasto3.png");
	GenerarTextura(texture3G, textureIsland3GID);
	Texture texture3B("../textures/islas/camino3.png");
	GenerarTextura(texture3B, textureIsland3BID);
	Texture texture3BlendMap("../textures/islas/blendMap3.png");
	GenerarTextura(texture3BlendMap, textureIsland3BlendMapID);

	// Seleccionar Personaje
	Texture textureIntro1("../textures/kakashi.png");
	GenerarTextura(textureIntro1, textureInit1ID);
	Texture textureIntro2("../textures/kratos.png");
	GenerarTextura(textureIntro2, textureInit2ID);
	Texture textureIntro3("../textures/naruto.png");
	GenerarTextura(textureIntro3, textureInit3ID);
	textureActivaID = textureInit1ID;

	Texture textureCubo("../textures/cubo.png");
	GenerarTextura(textureCubo, textureCuboID);

	// Texturas para fotos de intro
	Texture intro1("../textures/intro1.png");
	GenerarTextura(intro1, introTextures[0]);
	Texture intro2("../textures/intro2.png");
	GenerarTextura(intro2, introTextures[1]);
	Texture intro3("../textures/intro3.png");
	GenerarTextura(intro3, introTextures[2]);
	Texture intro4("../textures/intro4.png");
	GenerarTextura(intro4, introTextures[3]);
	Texture intro5("../textures/intro5.png");
	GenerarTextura(intro5, introTextures[4]);

	Texture lose("../textures/moriste.png");
	GenerarTextura(lose, textureLoseID);

	Texture win("../textures/ganaste.png");
	GenerarTextura(win, textureWinID);

	// Enlazar la textura
	glBindTexture(GL_TEXTURE_2D, textureCuboID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			// Ajustes de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			// Ajustes de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtro de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtro de maximización

	Texture texturaVida("../textures/BarraVida.png");

	GenerarTextura(texturaVida, texturaBarraVidaID);

	// Se inicializa el model de render text
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();
}

// Verificar si dos AABBs se superponen
bool checkCollision(const AbstractModel::OBB &box1, const AbstractModel::OBB &box2)
{
	// Comprueba si las cajas no se superponen en cualquiera de los ejes
	if (std::abs(box1.c.x - box2.c.x) > (box1.e.x + box2.e.x))
		return false;
	if (std::abs(box1.c.y - box2.c.y) > (box1.e.y + box2.e.y))
		return false;
	if (std::abs(box1.c.z - box2.c.z) > (box1.e.z + box2.e.z))
		return false;

	return true; // Las cajas se superponen
}

void destroy()
{
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
	boxBarraVida.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();

	// Custom objects Delete
	modelLamp1.destroy();
	modelCirculoMagico.destroy();
	modelCasa1.destroy();
	modelCasa2.destroy();
	modelCasa3.destroy();
	modelCasa4.destroy();
	modelCasa5.destroy();
	modelCasa6.destroy();
	modelCasa7.destroy();
	modelCasa8.destroy();
	modelCasa9.destroy();
	modelCasa10.destroy();
	modelArbol1I1.destroy();
	modelArbol2I1.destroy();
	modelArbol3I1.destroy();
	modelArbol4I1.destroy();
	modelArbol5I1.destroy();
	modelArbol1I2.destroy();
	modelArbol2I2.destroy();
	modelArbol3I2.destroy();
	modelArbol4I2.destroy();
	modelArbol5I2.destroy();
	modelArbol6I2.destroy();
	modelPuente.destroy();

	// Terrains objects Delete
	island1.destroy();
	island2.destroy();
	island3.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID1);
	glDeleteTextures(1, &textureCespedID2);
	glDeleteTextures(1, &textureCespedID3);
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
	glDeleteTextures(1, &skyboxTextureID1);
	glDeleteTextures(1, &skyboxTextureID2);
	glDeleteTextures(1, &skyboxTextureID3);

	// Texturas de la introducción
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureInit1ID);
	glDeleteTextures(1, &textureInit2ID);
	glDeleteTextures(1, &textureInit3ID);
	glDeleteTextures(1, &textureLoseID);
	glDeleteTextures(1, &textureWinID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes)
{
	float aspectRatio = 16.0f / 9.0f;
	int newWidth = widthRes;
	int newHeight = static_cast<int>(widthRes / aspectRatio);

	if (newHeight > heightRes)
	{
		newHeight = heightRes;
		newWidth = static_cast<int>(heightRes * aspectRatio);
	}

	glViewport((widthRes - newWidth) / 2, (heightRes - newHeight) / 2, newWidth, newHeight);
	screenWidth = newWidth;
	screenHeight = newHeight;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		// Alternar entre la cámara en tercera persona y la cámara en primera persona
		case GLFW_KEY_K:
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			{
				isFirstPersonActive = !isFirstPersonActive; // Apagar, encender la cámara
			}
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	static bool firstMouse = true;
	if (firstMouse)
	{
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

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod)
{
	if (state == GLFW_PRESS)
	{
		switch (button)
		{
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

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	distanceFromPlayer -= yoffset;
	camera->setDistanceFromTarget(distanceFromPlayer);
}

bool processInput(bool continueApplication, Player *jugador)
{
	if (exitApp || glfwWindowShouldClose(window) != 0)
	{
		return false;
	}

	if (!iniciaPartida)
	{
		bool presionarEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if (presionarEnter)
		{
			iniciaPartida = true;
			if (textureActivaID == textureInit1ID)
			{
				jugador->setJugador(Personaje::KAKASHI);
			}
			else if (textureActivaID == textureInit2ID)
			{
				jugador->setJugador(Personaje::KRATOS);
			}
			else if (textureActivaID == textureInit3ID)
			{
				jugador->setJugador(Personaje::NARUTO);
			}
			textureActivaID = textureCuboID;
		}
		else if (!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			presionarOpcion = true;
			if (textureActivaID == textureInit1ID)
			{
				textureActivaID = textureInit2ID;
			}
			else if (textureActivaID == textureInit2ID)
			{
				textureActivaID = textureInit3ID;
			}
			else if (textureActivaID == textureInit3ID)
			{
				textureActivaID = textureInit1ID;
			}
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
			presionarOpcion = false;

		// Important to return here to prevent other inputs from interfering
		return continueApplication;
	}

	// Si la cámara en primera persona está activa
	if (isFirstPersonActive)
	{
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

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0f, deltaTime);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0f, offsetY, deltaTime);

	offsetX = 0;
	offsetY = 0;

	// Controles para mover izquierda y a la derecha
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		angleTarget += 0.02f;
		jugador->anguloOrientacion += 0.02f;
	}

	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		angleTarget -= 0.02f;
		jugador->anguloOrientacion -= 0.02f;
	}

	// Controles para ir adelante y atras
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			jugador->setAccion(AccionJugador::CORRIENDO);
		}
		else
		{
			jugador->setAccion(AccionJugador::CAMINANDO);
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		jugador->setAccion(AccionJugador::REVERSA);
	}
	else
	{
		jugador->setAccion(AccionJugador::QUIETO);
	}

	enforceMapLimits(jugador->modelMatrix);

	glfwPollEvents();
	return continueApplication;
}

bool processInputController(bool continueApplication, Player *jugador)
{
	if (glfwJoystickPresent(GLFW_JOYSTICK_1) != GL_TRUE)
	{
		std::cout << "No se detecto el control" << std::endl;
		glfwPollEvents();
		return continueApplication;
	}

	int count;
	const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

	for (int i = 0; i < count; i++)
	{
		std::cout << "Botón " << i << ": " << (int)buttons[i] << std::endl;
	}

	if (!buttons)
	{
		std::cout << "No se detecto el control\n";
		glfwPollEvents();
		return continueApplication;
	}

	const int DPAD_UP = 10;		 // Flecha arriba
	const int DPAD_RIGHT = 11; // Flecha derecha
	const int DPAD_LEFT = 12;	 // Flecha izquierda
	const int DPAD_DOWN = 13;	 // Flecha abajo

	if (buttons[DPAD_LEFT] == GLFW_PRESS)
	{
		angleTarget += 0.02f; // Girar a la izquierda
		jugador->anguloOrientacion += 0.02f;
	}
	if (buttons[DPAD_RIGHT] == GLFW_PRESS)
	{
		angleTarget -= 0.02f; // Girar a la derecha
		jugador->anguloOrientacion -= 0.02f;
	}
	if (buttons[DPAD_UP] == GLFW_PRESS)
	{
		jugador->setAccion(AccionJugador::CAMINANDO); // Avanzar
	}
	if (buttons[DPAD_DOWN] == GLFW_PRESS)
	{
		jugador->setAccion(AccionJugador::REVERSA); // Retroceder
	}

	if (!(buttons[DPAD_UP] == GLFW_PRESS ||
				buttons[DPAD_DOWN] == GLFW_PRESS))
	{
		jugador->setAccion(AccionJugador::QUIETO);
	}

	// Asegurarse de que el jugador no salga de los límites
	enforceMapLimits(jugador->modelMatrix);

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop()
{
	bool psi = true;
	bool psic = true;

	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	lastTime = TimeManager::Instance().GetTime();

	// Inicializacoin de valores de la camara
	camera->setSensitivity(1.2f);
	camera->setDistanceFromTarget(distanceFromPlayer);

	CollidersController *cc = new CollidersController(&shader);
	Contador *contador = new Contador(&shaderTexture, modelText, textureCuboID);
	AudioManager::playBackgroundMusic("../sounds/intro.wav");

	Player jugador = Player(&shaderJugador, cc);
	jugador.setTerrain(&island1);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-100.0f, 100.0f);

	Cube cubo = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(10.0f, 0.0f, 10.0f), contador);
	cubo.setTerrain(&island1);

	Cube cubo2 = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(dist(gen), 0.0f, dist(gen)), contador);
	cubo2.setTerrain(&island1);

	Cube cubo3 = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(dist(gen), 0.0f, dist(gen)), contador);
	cubo3.setTerrain(&island1);

	Cube cubo4 = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(dist(gen), 0.0f, dist(gen)), contador);
	cubo4.setTerrain(&island2);

	Cube cubo5 = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(dist(gen), 0.0f, dist(gen)), contador);
	cubo5.setTerrain(&island2);

	Cube cubo6 = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(dist(gen), 0.0f, dist(gen)), contador);
	cubo6.setTerrain(&island2);

	Cube cubo7 = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(dist(gen), 0.0f, dist(gen)), contador);
	cubo7.setTerrain(&island3);

	Cube cubo8 = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(dist(gen), 0.0f, dist(gen)), contador);
	cubo8.setTerrain(&island3);

	Cube cubo9 = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(dist(gen), 0.0f, dist(gen)), contador);
	cubo9.setTerrain(&island3);

	Cube cubo10 = Cube("../models/cubo/cubo.fbx", &shaderMulLighting, cc, glm::vec3(dist(gen), 0.0f, dist(gen)), contador);
	cubo10.setTerrain(&island3);

	std::uniform_real_distribution<float> dist2(10.0f, 30.0f);
	std::mt19937 genRadioDeteccion(rd());

	// Creamos un enemigo
	glm::vec3 posicionEnemigo = glm::vec3(0.0f, 3.0f, 0.0f);
	Enemy enemigo = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, 30.0f);
	enemigo.setTerrain(&island1);

	posicionEnemigo = glm::vec3(dist(gen), 3.0f, dist(gen));
	Enemy enemigo2 = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, dist2(genRadioDeteccion));
	enemigo2.setTerrain(&island1);

	posicionEnemigo = glm::vec3(dist(gen), 3.0f, dist(gen));
	Enemy enemigo3 = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, dist2(genRadioDeteccion));
	enemigo3.setTerrain(&island1);

	posicionEnemigo = glm::vec3(dist(gen), 3.0f, dist(gen));
	Enemy enemigo4 = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, dist2(genRadioDeteccion));
	enemigo4.setTerrain(&island1);

	posicionEnemigo = glm::vec3(dist(gen), 3.0f, dist(gen));
	Enemy enemigo5 = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, dist2(genRadioDeteccion));
	enemigo5.setTerrain(&island2);

	posicionEnemigo = glm::vec3(dist(gen), 3.0f, dist(gen));
	Enemy enemigo6 = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, dist2(genRadioDeteccion));
	enemigo6.setTerrain(&island2);

	posicionEnemigo = glm::vec3(dist(gen), 3.0f, dist(gen));
	Enemy enemigo7 = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, dist2(genRadioDeteccion));
	enemigo7.setTerrain(&island2);

	posicionEnemigo = glm::vec3(dist(gen), 3.0f, dist(gen));
	Enemy enemigo8 = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, dist2(genRadioDeteccion));
	enemigo8.setTerrain(&island3);

	posicionEnemigo = glm::vec3(dist(gen), 3.0f, dist(gen));
	Enemy enemigo9 = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, dist2(genRadioDeteccion));
	enemigo9.setTerrain(&island3);

	posicionEnemigo = glm::vec3(dist(gen), 3.0f, dist(gen));
	Enemy enemigo10 = Enemy("../models/enemy/zombie.fbx", &shaderMulLighting, cc, posicionEnemigo, dist2(genRadioDeteccion));
	enemigo10.setTerrain(&island3);

	countdownStartTime = TimeManager::Instance().GetTime();

	while (psi && psic)
	{
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667)
		{
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(false);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true, &jugador);
		// psic = processInputController(true, &jugador);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Mostrar la introducción
		if (showIntro)
		{
			double currentTime = glfwGetTime();
			if (introStartTime == 0.0)
			{
				introStartTime = currentTime;
			}

			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));

			// Renderiza la textura de introducción actual
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, introTextures[currentIntroImage]);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();

			// Cambiar de imagen cada 7 segundos
			if (currentTime - introStartTime > TIEMPO_CARGA_INTRO)
			{
				currentIntroImage++;
				introStartTime = currentTime;
			}

			// Terminar la introducción después de la última imagen
			if (currentIntroImage >= 5)
			{
				showIntro = false;
				seleccionPersonaje = true; // Pasar a selección de personaje
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
			continue;
		}

		// Mostrar la selección de personaje
		if (seleccionPersonaje)
		{
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();

			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			{
				seleccionPersonaje = false;
				iniciaPartida = true;

				if (textureActivaID == textureInit1ID)
					jugador.setJugador(Personaje::KAKASHI);
				else if (textureActivaID == textureInit2ID)
					jugador.setJugador(Personaje::KRATOS);
				else if (textureActivaID == textureInit3ID)
					jugador.setJugador(Personaje::NARUTO);

				countdownStartTime = TimeManager::Instance().GetTime();
				// Audio para cuando inicie la partida
				AudioManager::playBackgroundMusic("../sounds/isla1.wav");
				AudioManager::playBackgroundMusic("../sounds/isla1.wav");
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
			continue;
		}

		// Calcular tiempo restante
		double currentTime = TimeManager::Instance().GetTime();
		double timeElapsed = currentTime - countdownStartTime;
		double timeRemaining = countdownDuration - timeElapsed;

		if (timeRemaining < 0)
			timeRemaining = 0;

		// Convertir a días, horas, minutos y segundos
		int minutes = static_cast<int>(timeRemaining) / 60;
		int seconds = static_cast<int>(timeRemaining) % 60;

		// Crear texto de la cuenta regresiva
		std::string countdownText =
				(minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
				(seconds < 10 ? "0" : "") + std::to_string(seconds);

		if ((timeRemaining <= 0 || jugador.getVida() <= 0) && !gameOver) {
				gameOver = true;

				// Configurar proyección y vista para pantalla completa
				shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
				shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureLoseID);
				shaderTexture.setInt("outTexture", 0);
		}

		if (contador->contador >= 10 && !gameWon) {
				gameWon = true;
				gameOver = true; // Reutilizamos la mecánica de gameOver

				// Configurar proyección y vista para pantalla completa
				shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
				shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureWinID);
				shaderTexture.setInt("outTexture", 0);
		}

		if (gameOver) {
				// Renderizar la pantalla de fin de juego
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Cambiar textura dependiendo del estado del juego
				if (gameWon) {
						glBindTexture(GL_TEXTURE_2D, textureWinID);
				} else {
						glBindTexture(GL_TEXTURE_2D, textureLoseID);
				}
				boxIntro.render();

				// Reproducir música correspondiente
				if (!gameOverMusicPlayed) {
					gameOverMusicPlayed = true;
					if (gameWon) {
							gameOverDuration = 58;
							AudioManager::playBackgroundMusic("../sounds/ganaste.wav");
							AudioManager::playBackgroundMusic("../sounds/ganaste.wav");
					} else {
							AudioManager::playBackgroundMusic("../sounds/moriste.wav");
							AudioManager::playBackgroundMusic("../sounds/moriste.wav");
					}
				}

				// Incrementar el temporizador de la pantalla final
				gameOverTimer += deltaTime;

				// Salir del bucle principal después de mostrar la pantalla final durante cierto tiempo
				if (gameOverTimer >= gameOverDuration || glfwWindowShouldClose(window)) {
						break; // Salir del programa
				}

				// Procesar eventos de la ventana
				glfwSwapBuffers(window);
				glfwPollEvents();

				// Continuar sin procesar lógica del juego
				continue;
		}


		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 1000.0f);

		positionTarget = jugador.modelMatrix[3];
		
		if (isIsland1Active)
			positionTarget.y = island1.getHeightTerrain(positionTarget.x, positionTarget.z) + 3.0f; // Ajuste de altura
		else if (isIsland2Active)
			positionTarget.y = island2.getHeightTerrain(positionTarget.x, positionTarget.z) + 3.0f; // Ajuste de altura
		else if (isIsland3Active)
			positionTarget.y = island3.getHeightTerrain(positionTarget.x, positionTarget.z) + 3.0f; // Ajuste de altura

		glm::mat4 view;
		if (isFirstPersonActive)
		{
			view = firstPersonCamera->getViewMatrix(); // Usar cámara en primera persona
		}
		else
		{
			camera->setCameraTarget(positionTarget);
			camera->setAngleTarget(angleTarget);
			camera->updateCamera();
			view = camera->getViewMatrix();
		}

		// Después de actualizar la cámara
		float angleAroundTarget = camera->getAngleAroundTarget();
		float maxAngleDifference = glm::radians(60.0f); // Ángulo máximo antes de rotar el personaje
		float characterRotation = 0.0f;

		if (angleAroundTarget > maxAngleDifference)
		{
			characterRotation = angleAroundTarget - maxAngleDifference;
		}
		else if (angleAroundTarget < -maxAngleDifference)
		{
			characterRotation = angleAroundTarget + maxAngleDifference;
		}

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false, glm::value_ptr(view));

		shaderJugador.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderJugador.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Render de Islas
		 *******************************************/
		glDisable(GL_CULL_FACE);

		if (isIsland1Active)
		{
			island1.setPosition(glm::vec3(100, 0, 100));
			RenderTextura(textureCespedID1, textureIsland1RID, textureIsland1GID, textureIsland1BID, textureIsland1BlendMapID);
			island1.render();

			/*******************************************
			 * Render de Circulos de Transporte
			 *******************************************/
			matrixModelCirculo1 = glm::mat4(1.0f);
			matrixModelCirculo1 = glm::translate(matrixModelCirculo1, glm::vec3(40.0, 0.0, 30.0));
			matrixModelCirculo1[3][1] = island1.getHeightTerrain(matrixModelCirculo1[3][0], matrixModelCirculo1[3][2]);
			matrixModelCirculo1[3][1] += 1.0f;
			modelCirculoMagico.render(matrixModelCirculo1);

			AbstractModel::SBB circle1Collider;
			circle1Collider.c = glm::vec3(matrixModelCirculo1[3]);
			circle1Collider.ratio = 2.5f; // Ajusta el radio según el tamaño del círculo
			AbstractModel::SBB playerCollider;
			playerCollider.c = glm::vec3(jugador.modelMatrix[3]);
			playerCollider.ratio = 1.0f; // Ajusta el radio según el tamaño del personaje

			if (testSphereSphereIntersection(playerCollider, circle1Collider))
			{
				std::cout << "Colisión con el círculo 1 detectada." << std::endl;
				changeIsland(2, jugador, enemigo); // Cambiar a isla 2
			}

			/*******************************************
			 * Render de Arboles
			 *******************************************/
			renderArboles(modelArbol1I1, arbol1_Isla1_Position, arbol1_Isla1_Orientation, island1);
			renderArboles(modelArbol2I1, arbol2_Isla1_Position, arbol2_Isla1_Orientation, island1);
			renderArboles(modelArbol3I1, arbol3_Isla1_Position, arbol3_Isla1_Orientation, island1);
			renderArboles(modelArbol4I1, arbol4_Isla1_Position, arbol4_Isla1_Orientation, island1);
			renderArboles(modelArbol5I1, arbol5_Isla1_Position, arbol5_Isla1_Orientation, island1);
		}

		if (isIsland2Active)
		{
			island2.setPosition(glm::vec3(100, 0, 100));
			RenderTextura(textureCespedID2, textureIsland2RID, textureIsland2GID, textureIsland2BID, textureIsland2BlendMapID);
			island2.render();

			/*******************************************
			 * Render de Circulos de Transporte
			 *******************************************/
			matrixModelCirculo2 = glm::mat4(1.0f);
			matrixModelCirculo2 = glm::translate(matrixModelCirculo2, glm::vec3(10.0, 0.0, 0.0));
			matrixModelCirculo2[3][1] = island2.getHeightTerrain(matrixModelCirculo2[3][0], matrixModelCirculo2[3][2]);
			modelCirculoMagico.render(matrixModelCirculo2);

			AbstractModel::SBB circle2Collider;
			circle2Collider.c = glm::vec3(matrixModelCirculo2[3]);
			circle2Collider.ratio = 2.5f;
			AbstractModel::SBB playerCollider;
			playerCollider.c = glm::vec3(jugador.modelMatrix[3]);
			playerCollider.ratio = 1.0f; // Ajusta el radio según el tamaño del personaje

			if (testSphereSphereIntersection(playerCollider, circle2Collider))
			{
				std::cout << "Colisión con el círculo 2 detectada." << std::endl;
				changeIsland(3, jugador, enemigo); // Cambiar a isla 3
			}

			/*******************************************
			 * Render de Objetos
			 *******************************************/
			renderModel(modelCasa1, glm::vec3(85.0f, 0.0f, 70.0f), glm::vec3(1.0f), 180.0f, &island2);
			renderModel(modelCasa2, glm::vec3(68.0f, 0.0f, 80.0f), glm::vec3(1.0f), 10.0f, &island2);
			renderModel(modelCasa3, glm::vec3(60.0f, 0.0f, 82.0f), glm::vec3(1.0f), 10.0f, &island2);
			renderModel(modelCasa4, glm::vec3(0.0f, 0.0f, 85.0f), glm::vec3(1.0f), -2.5f, &island2);
			renderModel(modelCasa5, glm::vec3(-65.0f, 0.0f, 80.0f), glm::vec3(1.0f), 0.0f, &island2);
			renderModel(modelCasa6, glm::vec3(-72.0f, 0.0f, 55.0f), glm::vec3(1.0f), 0.0f, &island2);
			renderModel(modelCasa7, glm::vec3(-25.0f, 0.0f, 50.0f), glm::vec3(1.0f), -180.0f, &island2);
			renderModel(modelCasa8, glm::vec3(30.0f, 0.0f, 55.0f), glm::vec3(1.0f), 180.0f, &island2);
			renderModel(modelCasa9, glm::vec3(63.0f, 0.0f, 25.0f), glm::vec3(1.0f), 10.0f, &island2);
			renderModel(modelCasa10, glm::vec3(-50.0f, 0.0f, -60.0f), glm::vec3(1.0f), 0.0f, &island2);
			renderModel(modelCasa5, glm::vec3(30.0f, 0.0f, 30.0f), glm::vec3(1.0f), 270.0f, &island2);
			renderModel(modelCasa6, glm::vec3(30.0f, 0.0f, 85.0f), glm::vec3(1.0f), -270.0f, &island2);
			renderModel(modelCasa7, glm::vec3(90.0f, 0.0f, 45.0f), glm::vec3(1.0f), 180.0f, &island2);
			renderModel(modelCasa8, glm::vec3(-40.0f, 0.0f, 30.0f), glm::vec3(1.0f), 100.0f, &island2);
			renderModel(modelCasa9, glm::vec3(-50.0f, 0.0f, 80.0f), glm::vec3(1.0f), 225.0f, &island2);
			renderModel(modelPuente, glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(1.0f), 0.0f, &island2);

			/*******************************************
			 * Render de Arboles
			 *******************************************/
			renderArboles(modelArbol1I2, arbol1_Isla2_Position, arbol1_Isla2_Orientation, island2);
			renderArboles(modelArbol2I2, arbol2_Isla2_Position, arbol2_Isla2_Orientation, island2);
			renderArboles(modelArbol3I2, arbol3_Isla2_Position, arbol3_Isla2_Orientation, island2);
			renderArboles(modelArbol4I2, arbol4_Isla2_Position, arbol4_Isla2_Orientation, island2);
			renderArboles(modelArbol5I2, arbol5_Isla2_Position, arbol5_Isla2_Orientation, island2);

			/*******************************************
			 * Render de Lamparas
			 *******************************************/
			for (int i = 0; i < lamp1Position.size(); i++)
			{
				lamp1Position[i].y = island2.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z);
				modelLamp1.setPosition(lamp1Position[i]);
				modelLamp1.setScale(glm::vec3(0.5));
				modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
				modelLamp1.render();
			}
		}

		if (isIsland3Active)
		{
			island3.setPosition(glm::vec3(100, 0, 100));
			RenderTextura(textureCespedID3, textureIsland3RID, textureIsland3GID, textureIsland3BID, textureIsland3BlendMapID);
			island3.render();

			/*******************************************
			 * Render de Circulos de Transporte
			 *******************************************/
			matrixModelCirculo3 = glm::mat4(1.0f);
			matrixModelCirculo3 = glm::translate(matrixModelCirculo3, glm::vec3(15.0, 0.0, -30.0));
			matrixModelCirculo3[3][1] = island3.getHeightTerrain(matrixModelCirculo3[3][0], matrixModelCirculo3[3][2]);
			modelCirculoMagico.render(matrixModelCirculo3);

			AbstractModel::SBB circle3Collider;
			circle3Collider.c = glm::vec3(matrixModelCirculo3[3]);
			circle3Collider.ratio = 2.5f;
			AbstractModel::SBB playerCollider;
			playerCollider.c = glm::vec3(jugador.modelMatrix[3]);
			playerCollider.ratio = 1.0f; // Ajusta el radio según el tamaño del personaje

			if (testSphereSphereIntersection(playerCollider, circle3Collider))
			{
				std::cout << "Colisión con el círculo 3 detectada." << std::endl;
				changeIsland(1, jugador, enemigo); // Cambiar a isla 1
			}
		}
		glEnable(GL_CULL_FACE);

		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);

		/*	=========================================
				===== Render Elementos del Juego ========
				=========================================
		*/

		cc->update(deltaTime);
		jugador.update(deltaTime);
		enemigo.update(deltaTime, jugador.posicion);
		if (isIsland1Active)
		{
			enemigo2.update(deltaTime, jugador.posicion);
			enemigo3.update(deltaTime, jugador.posicion);
			enemigo4.update(deltaTime, jugador.posicion);
			cubo.update(deltaTime, jugador.posicion);
			cubo2.update(deltaTime, jugador.posicion);
			cubo3.update(deltaTime, jugador.posicion);
		}

		if (isIsland2Active)
		{
			enemigo5.update(deltaTime, jugador.posicion);
			enemigo6.update(deltaTime, jugador.posicion);
			enemigo7.update(deltaTime, jugador.posicion);
			cubo4.update(deltaTime, jugador.posicion);
			cubo5.update(deltaTime, jugador.posicion);
			cubo6.update(deltaTime, jugador.posicion);
		}

		if (isIsland3Active)
		{
			enemigo8.update(deltaTime, jugador.posicion);
			enemigo9.update(deltaTime, jugador.posicion);
			enemigo10.update(deltaTime, jugador.posicion);
			cubo7.update(deltaTime, jugador.posicion);
			cubo8.update(deltaTime, jugador.posicion);
			cubo9.update(deltaTime, jugador.posicion);
			cubo10.update(deltaTime, jugador.posicion);
		}

		// Renderizamos al jugador
		jugador.render();
		enemigo.render();

		if (isIsland1Active)
		{
			enemigo8.removeCollider();
			enemigo9.removeCollider();
			enemigo10.removeCollider();
			cubo7.removeCollider();
			cubo8.removeCollider();
			cubo9.removeCollider();
			cubo10.removeCollider();

			enemigo2.render();
			enemigo3.render();
			enemigo4.render();
			cubo.render();
			cubo2.render();
			cubo3.render();
		}

		if (isIsland2Active)
		{
			enemigo2.removeCollider();
			enemigo3.removeCollider();
			enemigo4.removeCollider();
			cubo.removeCollider();
			cubo2.removeCollider();
			cubo3.removeCollider();

			enemigo5.render();
			enemigo6.render();
			enemigo7.render();
			cubo4.render();
			cubo5.render();
			cubo6.render();
		}

		if (isIsland3Active)
		{
			enemigo5.removeCollider();
			enemigo6.removeCollider();
			enemigo7.removeCollider();
			cubo4.removeCollider();
			cubo5.removeCollider();
			cubo6.removeCollider();

			enemigo8.render();
			enemigo9.render();
			enemigo10.render();
			cubo7.render();
			cubo8.render();
			cubo9.render();
			cubo10.render();
		}

		// Para mostrar las colisiones
		cc->render();

		luces(jugador, projection, view);

		// Enlaza el skybox activo
		// Seleccionar el skybox activo antes de renderizar
		GLuint currentSkyboxTextureID;
		if (activeSkybox == 1)
		{
			currentSkyboxTextureID = skyboxTextureID1;
		}
		else if (activeSkybox == 2)
		{
			currentSkyboxTextureID = skyboxTextureID2;
		}
		else if (activeSkybox == 3)
		{
			currentSkyboxTextureID = skyboxTextureID3;
		}

		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, currentSkyboxTextureID);
		shaderSkybox.setInt("skybox", 0);

		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		// =======================================
		// ======== OBJETOS TRANSPARENTES ========
		// =======================================

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);

		// scaleUV = 1.0f / numEstados;
		float scaleUV = 1.0f / (MAXIMA_VIDA + 1);
		// offsetY = estadoActual * scaleUV;
		float offsetY = jugador.getVida() * scaleUV;

		shaderTexture.setFloat("scaleUV", scaleUV);
		shaderTexture.setFloat("offsetY", offsetY);
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturaBarraVidaID);
		shaderTexture.setInt("outTexture", 0);
		glm::mat4 boxMatrixModel = glm::mat4(1.0f);
		boxMatrixModel = glm::translate(boxMatrixModel, glm::vec3(-0.7f, -0.85f, 0.0f));
		boxBarraVida.render(boxMatrixModel);

		contador->render();
		// Renderizar el texto con estilo más grande
		modelText->render("tiempo", 0.0f, 0.9f, 1.5f);			// Título
		modelText->render(countdownText, 0.0f, 0.8f, 1.5f); // Cuenta regresiva

		glDisable(GL_BLEND);

		glfwSwapBuffers(window);
	}

	delete cc;
}

int main(int argc, char **argv)
{
	try
	{
		// Establece la relación de aspecto 16:9
		int width = 1200;						 // Ejemplo de ancho
		int height = width * 9 / 16; // Calcula el alto basado en 16:9
		init(width, height, "Window GLFW", false);
		AudioManager::init();
		applicationLoop();
		destroy();
		AudioManager::cleanup();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Excepción capturada: " << e.what() << std::endl;
		return -1;
	}
	return 1;
}

void loadModels()
{
	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	// Models Lamparas
	modelLamp1.loadModel("../models/isla2/lampara/lampara.obj");
	modelLamp1.setShader(&shaderMulLighting);

	// Circulo de transporte
	modelCirculoMagico.loadModel("../models/teletransportador/circulo_magico.obj");
	modelCirculoMagico.setShader(&shaderMulLighting);

	// OBJETOS ISLA 2
	modelCasa1.loadModel("../models/isla2/casas/casa1.obj");
	modelCasa1.setShader(&shaderMulLighting);
	modelCasa2.loadModel("../models/isla2/casas/casa2.obj");
	modelCasa2.setShader(&shaderMulLighting);
	modelCasa3.loadModel("../models/isla2/casas/casa3.obj");
	modelCasa3.setShader(&shaderMulLighting);
	modelCasa4.loadModel("../models/isla2/casas/casa4.obj");
	modelCasa4.setShader(&shaderMulLighting);
	modelCasa5.loadModel("../models/isla2/casas/casa5.obj");
	modelCasa5.setShader(&shaderMulLighting);
	modelCasa6.loadModel("../models/isla2/casas/casa6.obj");
	modelCasa6.setShader(&shaderMulLighting);
	modelCasa7.loadModel("../models/isla2/casas/casa7.obj");
	modelCasa7.setShader(&shaderMulLighting);
	modelCasa8.loadModel("../models/isla2/casas/casa8.obj");
	modelCasa8.setShader(&shaderMulLighting);
	modelCasa9.loadModel("../models/isla2/casas/casa9.obj");
	modelCasa9.setShader(&shaderMulLighting);
	modelCasa10.loadModel("../models/isla2/casas/casa_japonesa.obj");
	modelCasa10.setShader(&shaderMulLighting);
	modelArbol1I1.loadModel("../models/isla1/arboles/arbol1.obj");
	modelArbol1I1.setShader(&shaderMulLighting);
	modelArbol2I1.loadModel("../models/isla1/arboles/arbol2.obj");
	modelArbol2I1.setShader(&shaderMulLighting);
	modelArbol3I1.loadModel("../models/isla1/arboles/arbol3.obj");
	modelArbol3I1.setShader(&shaderMulLighting);
	modelArbol4I1.loadModel("../models/isla1/arboles/arbol4.obj");
	modelArbol4I1.setShader(&shaderMulLighting);
	modelArbol5I1.loadModel("../models/isla1/arboles/arbol5.obj");
	modelArbol5I1.setShader(&shaderMulLighting);
	modelArbol1I2.loadModel("../models/isla2/arboles/arbol1.obj");
	modelArbol1I2.setShader(&shaderMulLighting);
	modelArbol2I2.loadModel("../models/isla2/arboles/arbol2.obj");
	modelArbol2I2.setShader(&shaderMulLighting);
	modelArbol3I2.loadModel("../models/isla2/arboles/arbol3.obj");
	modelArbol3I2.setShader(&shaderMulLighting);
	modelArbol4I2.loadModel("../models/isla2/arboles/arbol4.obj");
	modelArbol4I2.setShader(&shaderMulLighting);
	modelArbol5I2.loadModel("../models/isla2/arboles/arbol5.obj");
	modelArbol5I2.setShader(&shaderMulLighting);
	modelArbol6I2.loadModel("../models/isla2/arboles/arbol6.obj");
	modelArbol6I2.setShader(&shaderMulLighting);
	modelPuente.loadModel("../models/isla2/puente.obj");
	modelPuente.setShader(&shaderMulLighting);

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

	// Box barra vida
	boxBarraVida.init();
	boxBarraVida.setShader(&shaderTexture);
	boxBarraVida.setScale(glm::vec3(0.5f, 0.15f, 0.00001f));
}

void renderModel(Model &model, const glm::vec3 &position, const glm::vec3 &scale, float rotation, Terrain *terrain)
{
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, scale);
	if (terrain)
	{
		// Ajusta la posición en función del terreno
		modelMatrix[3][1] = terrain->getHeightTerrain(modelMatrix[3][0], modelMatrix[3][2]);
	}
	model.render(modelMatrix);
}

GLuint loadCubemapTextures(std::string fileNames[6])
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	GLenum types[6] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_X, // Right
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // Left
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // Top
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // Bottom
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // Front
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	// Back
	};

	for (int i = 0; i < 6; i++)
	{
		Texture skyboxTexture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData())
		{
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA,
									 skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
									 skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		}
		else
		{
			std::cout << "Failed to load skybox texture: " << fileNames[i] << std::endl;
		}
		skyboxTexture.freeImage();
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}

void GenerarTextura(Texture texture, GLuint &textureID)
{
	// Definiendo la textura
	texture.loadImage();																							// Cargar la textura
	glGenTextures(1, &textureID);																			// Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureID);													// Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			// Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			// Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (texture.getData())
	{
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texture.getChannels() == 3 ? GL_RGB : GL_RGBA, texture.getWidth(), texture.getHeight(), 0,
								 texture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	texture.freeImage(); // Liberamos memoria
}

void RenderTextura(GLuint Cesped, GLuint R, GLuint G, GLuint B, GLuint BlendMap)
{
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

void luces(const Player &jugador, glm::mat4 projection, glm::mat4 view)
{
	/*******************************************
	 * Propiedades Luz direccional
	 *******************************************/
	shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
	shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
	shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

	shaderJugador.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
	shaderJugador.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
	shaderJugador.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
	shaderJugador.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
	shaderJugador.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

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
	shaderMulLighting.setInt("pointLightCount", lamp1Position.size()); // + lamp2Position.size());
	shaderTerrain.setInt("pointLightCount", lamp1Position.size());		 // + lamp2Position.size());
	for (int i = 0; i < lamp1Position.size(); i++)
	{
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
	/* for (int i = 0; i < lamp2Position.size(); i++)
	{
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
	} */
}

void changeIsland(int targetIsland, Player &jugador, Enemy &enemigo)
{
	// Desactivar todas las islas
	isIsland1Active = false;
	isIsland2Active = false;
	isIsland3Active = false;

	// Variables para almacenar el terreno actual y la nueva posición del jugador
	Terrain *currentTerrain = nullptr;
	glm::vec3 newPosition;

	// Activar la isla y definir su terreno y posición inicial
	if (targetIsland == 1)
	{
		AudioManager::playBackgroundMusic("../sounds/isla1.wav");
		AudioManager::playBackgroundMusic("../sounds/isla1.wav");
		isIsland1Active = true;
		activeSkybox = 1;
		currentTerrain = &island1;
		newPosition = glm::vec3(5.0f, 0.0f, 0.0f); // Posición específica en isla 1
	}
	else if (targetIsland == 2)
	{
		AudioManager::playBackgroundMusic("../sounds/isla2.wav");
		AudioManager::playBackgroundMusic("../sounds/isla2.wav");
		isIsland2Active = true;
		activeSkybox = 2;
		currentTerrain = &island2;
		newPosition = glm::vec3(15.0f, 0.0f, 0.0f); // Posición específica en isla 2
	}
	else if (targetIsland == 3)
	{
		AudioManager::playBackgroundMusic("../sounds/isla3.wav");
		AudioManager::playBackgroundMusic("../sounds/isla3.wav");
		isIsland3Active = true;
		activeSkybox = 3;
		currentTerrain = &island3;
		newPosition = glm::vec3(20.0f, 0.0f, 0.0f); // Posición específica en isla 3
	}

	jugador.modelMatrix = glm::mat4(1.0f);

	jugador.setTerrain(currentTerrain);
	enemigo.setTerrain(currentTerrain);

	jugador.modelMatrix = glm::translate(glm::mat4(1.0f), newPosition);
	jugador.modelMatrix = glm::scale(jugador.modelMatrix, glm::vec3(0.01f));

	// Ajustar la altura del jugador para que esté sobre el terreno
	float terrainHeight = currentTerrain->getHeightTerrain(newPosition.x, newPosition.z);
	jugador.modelMatrix[3][1] = terrainHeight;

	// Reiniciar angleTarget
	angleTarget = 0.0f;

	// Actualizar la posición y objetivo de la cámara
	if (!isFirstPersonActive)
	{
		positionTarget = jugador.modelMatrix[3];
		positionTarget.y += 1.5f;
		camera->setCameraTarget(positionTarget);
		camera->setDistanceFromTarget(distanceFromPlayer);
		camera->updateCamera();
	}
}

void renderArboles(Model &model, std::vector<glm::vec3> &positions, const std::vector<float> &orientations, Terrain &terrain)
{
	for (size_t i = 0; i < positions.size(); i++)
	{
		// Ajusta la altura de cada posición según el terreno
		positions[i].y = terrain.getHeightTerrain(positions[i].x, positions[i].z);

		// Configura el modelo del árbol
		model.setPosition(positions[i]);
		model.setScale(glm::vec3(1.0f)); // Escala fija, puedes hacerlo un parámetro si deseas flexibilidad
		model.setOrientation(glm::vec3(0, orientations[i], 0));

		// Renderiza el árbol
		model.render();
	}
}

void enforceMapLimits(glm::mat4 &modelMatrix)
{
	glm::vec3 position = modelMatrix[3]; // Obtener la posición actual del jugador

	// Verificar límites y ajustar posición
	if (position.x < MAP_MIN_X)
		position.x = MAP_MIN_X;
	if (position.x > MAP_MAX_X)
		position.x = MAP_MAX_X;
	if (position.z < MAP_MIN_Z)
		position.z = MAP_MIN_Z;
	if (position.z > MAP_MAX_Z)
		position.z = MAP_MAX_Z;

	// Actualizar la matriz del modelo con la nueva posición
	modelMatrix[3] = glm::vec4(position, 1.0f);
}
