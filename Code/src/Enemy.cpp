#include "Enemy.h"

Enemy::Enemy(std::string modelPath, Shader* shader, glm::vec3 position, float radius)
  : posicionInicial(position), radioDeteccion(radius) {
    
  this->position = position; // Posición inicial actual
  this->modelo.loadModel(modelPath);
  this->modelo.setShader(shader);
  this->radioGolpe = 5.0f;

  this->modelMatrix = glm::translate(glm::mat4(1.0f), position);
  this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(0.035f));
}

Enemy::~Enemy() {}

void Enemy::update(float dt, glm::vec3 posicionObjetivo) {
  float speed = 10.0f;

  if (!objetivoEstaEnElArea(posicionObjetivo)) {
    // animacion acostado
    modelo.setAnimationIndex(3);
    this->empiezaPersecucion = false;
  } else {
    if(! this->empiezaPersecucion) {
      this->inicioPersecucion = std::chrono::steady_clock::now();
      // animacion levantandose
      this->modelo.setAnimationIndex(1);
    }

    this->empiezaPersecucion = true;

    auto ahora = std::chrono::steady_clock::now();
    auto tiempoTranscurrido = std::chrono::duration_cast<std::chrono::seconds>(ahora - inicioPersecucion).count();

    if(tiempoTranscurrido >= 3) {

      if(this->golpearObjetivo(posicionObjetivo)) {
        // animacion de golpe
        this->modelo.setAnimationIndex(0);
      } else {
        // animacion corriendo
        this->modelo.setAnimationIndex(2);
      }

      this->seguirObjetivo(posicionObjetivo, speed, dt);
    }
  }
}

void Enemy::render() {
  this->modelo.render(this->modelMatrix);
}

void Enemy::seguirObjetivo(glm::vec3 targetPosition, float speed, float dt) {
  // Obtener la posición actual del enemigo
  glm::vec3 currentPosition = glm::vec3(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

  // Dirección hacia el objetivo
  glm::vec3 direction = glm::normalize(targetPosition - currentPosition);

  // Movimiento en esa dirección
  glm::vec3 movement = direction * speed * dt;

  // Actualizar la posición del enemigo
  currentPosition += movement;
  position = currentPosition;

  // Calcular el ángulo de rotación en Y (para que mire al objetivo)
  float angle = atan2(direction.x, direction.z);

  // Crear una matriz de rotación en torno al eje Y
  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

  // Actualizar la matriz de modelo con la rotación y la traslación
  this->modelMatrix = glm::translate(glm::mat4(1.0f), currentPosition) * rotationMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(0.035f));
}

bool Enemy::objetivoEstaEnElArea(glm::vec3 targetPosition) {
  // Distancia euclidiana entre el objetivo y la posición original
  float distancia = glm::length(targetPosition - this->position);

  std::cout << "Distancia calculada: " << distancia << std::endl;
  return distancia <= this->radioDeteccion;
}

bool Enemy::golpearObjetivo(glm::vec3 posicionObjetivo) {
  float distancia = glm::length(posicionObjetivo - this->position);
  return distancia <= this->radioGolpe;
}