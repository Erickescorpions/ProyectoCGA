#ifndef COLLIDERS_CONTROLLER_H
#define COLLIDERS_CONTROLLER_H

#include "Headers/Box.h"
#include "Headers/Sphere.h"
#include "Headers/Shader.h"
#include "Colisiones.h"

#include <map>
#include <string>

enum TipoColision 
{
  SBB = 1,
  OBB
};

class CollidersController
{
public:
  CollidersController(Shader *shader);
  void update(float dt);
  void render();
  
  void addOrUpdateCollidersOBB(std::string colliderName, AbstractModel::OBB collider, glm::mat4 modelMatrix);
  void addOrUpdateCollidersSBB(std::string colliderName, AbstractModel::SBB collider, glm::mat4 modelMatrix);
  void removeCollidersOBB(std::string colliderName);
  void removeCollidersSBB(std::string colliderName);

  void pruebaColisionesSBBvsSBB();
  void pruebaColisionesOBBvsOBB();
  bool modeloColisiono(std::string colliderName, TipoColision type);
  void checkAndResolveCollisions(AbstractModel::SBB &sbb1, AbstractModel::SBB &sbb2);
  void checkAndResolveCollisions(AbstractModel::OBB &obb1, AbstractModel::OBB &obb2);
  void resolveSphereSphereCollision(AbstractModel::SBB &sbb1, AbstractModel::SBB &sbb2);
  void resolveOBBOBBCollision(AbstractModel::OBB &obb1, AbstractModel::OBB &obb2);
  bool verificarColision(std::string colliderName);
  bool verificarColision2(std::string collider1, std::string collider2);

private:
  Box boxCollider;
  Sphere sphereCollider;
  Shader* shader;
  std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>> collidersOBB;
  std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>> collidersSBB;
  std::map<std::string, bool> collisionDetection;
  std::map<std::string, std::string> collisionDetection2;
};

#endif // COLLIDERS_CONTROLLER_H
