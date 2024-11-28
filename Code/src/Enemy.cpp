#include "Enemy.h"

Enemy::Enemy(std::string modelPath, Shader* shader) {
  this->position = glm::vec3(1.0f);
  this->modelMatrix = glm::mat4(1.0f);
  this->modelo.loadModel(modelPath);
  this->modelo.setShader(shader);

  this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(20.0f, 0.1f, 0.0f));
  this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(0.035f));
}

Enemy::~Enemy() {}

void Enemy::update(float dt, glm::vec3 posicionObjetivo) {
  float speed = 5.0f;
  this->followTarget(posicionObjetivo, speed, dt);
}

void Enemy::render() {
  this->modelo.render(this->modelMatrix);
}

void Enemy::followTarget(glm::vec3 targetPosition, float speed, float dt) {
  // Obtener la posición actual del enemigo
  glm::vec3 currentPosition = glm::vec3(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

  // Dirección hacia el objetivo
  glm::vec3 direction = glm::normalize(targetPosition - currentPosition);

  // Movimiento en esa dirección
  glm::vec3 movement = direction * speed * dt;

  // Actualizar la posición del enemigo
  currentPosition += movement;

  // Calcular el ángulo de rotación en Y (para que mire al objetivo)
  float angle = atan2(direction.x, direction.z);

  // Crear una matriz de rotación en torno al eje Y
  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

  // Actualizar la matriz de modelo con la rotación y la traslación
  this->modelMatrix = glm::translate(glm::mat4(1.0f), currentPosition) * rotationMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(0.035f));
}