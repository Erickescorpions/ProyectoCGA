#ifndef ENEMY_H
#define ENEMY_H

// GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include loader Model class
#include "Headers/Model.h"
#include "Headers/Shader.h"
#include "Headers/Terrain.h"
#include <string>
#include <iostream>
#include <chrono>
#include "Headers/AbstractModel.h"
#include "CollidersController.h"

class Enemy
{
public:
  Model modelo;
  glm::mat4 modelMatrix;
  glm::vec3 position;
  Shader *shader;
  std::string modelPath;
  int ataque = 1;
  AbstractModel::OBB collider;

  // Constructor
  Enemy(std::string modelPath, Shader *shader, CollidersController* cc, glm::vec3 position, float radius);

  // Destructor
  ~Enemy();

  void update(float dt, glm::vec3 posicionObjetivo);
  void render();
  void setTerrain(Terrain *terrain);

private:
  glm::vec3 posicionInicial;
  float radioDeteccion;
  float radioGolpe;
  bool empiezaPersecucion;
  std::chrono::steady_clock::time_point inicioPersecucion;
  Terrain *terrain;
  float angulo;
  glm::mat4 modelMatrixCollider;
  CollidersController* cc;
  float scaleFactor;
  float velocidad;

  void seguirObjetivo(glm::vec3 posicionObjetivo, float speed, float dt);
  bool objetivoEstaEnElArea(glm::vec3 posicionObjetivo);
  bool golpearObjetivo(glm::vec3 posicionObjetivo);
  void addOrUpdateColliders();
};

#endif // ENEMY_H