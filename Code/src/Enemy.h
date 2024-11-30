#ifndef ENEMY_H
#define ENEMY_H

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include loader Model class
#include "Headers/Model.h"
#include "Headers/Shader.h"
#include <string>
#include <iostream>
#include <chrono>

class Enemy
{
public:

Model modelo;
glm::mat4 modelMatrix;
glm::vec3 position;
Shader* shader;
std::string modelPath;

// Constructor
Enemy(std::string modelPath, Shader* shader, glm::vec3 position, float radius);

// Destructor
~Enemy();

void update(float dt, glm::vec3 posicionObjetivo);
void render();
void seguirObjetivo(glm::vec3 targetPosition, float speed, float dt);
bool objetivoEstaEnElArea(glm::vec3 targetPosition);
bool golpearObjetivo(glm::vec3 posicionObjetivo);

private:
glm::vec3 posicionInicial;
float radioDeteccion;
float radioGolpe;
bool empiezaPersecucion; 
std::chrono::steady_clock::time_point inicioPersecucion;
};


#endif // ENEMY_H