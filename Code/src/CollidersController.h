#ifndef COLLIDERS_CONTROLLER_H
#define COLLIDERS_CONTROLLER_H

#include "Headers/Box.h"
#include "Headers/Sphere.h"
#include "Headers/Shader.h"
#include "Colisiones.h"

#include <map>
#include <string>

class CollidersController
{
public:
  CollidersController(Shader *shader);
  void render();
  
  // Declaración de las funciones de actualización de colisionadores
  void addOrUpdateCollidersOBB(std::string colliderName, AbstractModel::OBB collider, glm::mat4 modelMatrix);
  void addOrUpdateCollidersSBB(std::string colliderName, AbstractModel::SBB collider, glm::mat4 modelMatrix);

private:
  Box boxCollider;
  Sphere sphereCollider;
  Shader* shader;
  std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>> collidersOBB;
  std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>> collidersSBB;
};

#endif // COLLIDERS_CONTROLLER_H
