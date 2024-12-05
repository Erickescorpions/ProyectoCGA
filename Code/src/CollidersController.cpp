#include "CollidersController.h"

CollidersController::CollidersController(Shader *shader) : sphereCollider(Sphere(10, 10))
{
  this->shader = shader;

  this->boxCollider.init();
  this->boxCollider.setShader(shader);
  this->boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

  this->sphereCollider.init();
  this->sphereCollider.setShader(shader);
  this->sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void CollidersController::render()
{
  for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
           collidersOBB.begin();
       it != collidersOBB.end(); it++)
  {
    glm::mat4 matrixCollider = glm::mat4(1.0);
    matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
    matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
    matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
    boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
    boxCollider.enableWireMode();
    boxCollider.render(matrixCollider);
  }

  for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
           collidersSBB.begin();
       it != collidersSBB.end(); it++)
  {
    glm::mat4 matrixCollider = glm::mat4(1.0);
    matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
    matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
    sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
    sphereCollider.enableWireMode();
    sphereCollider.render(matrixCollider);
  }
}

void CollidersController::update(float dt)
{
  // limpiamos las colisiones detectadas anteriormente
  this->collisionDetection.clear();
  this->collisionDetection2.clear();

  this->pruebaColisionesSBBvsSBB();
  this->pruebaColisionesOBBvsOBB();
}

void CollidersController::addOrUpdateCollidersOBB(std::string colliderName, AbstractModel::OBB collider, glm::mat4 modelMatrix)
{
  addOrUpdateColliders(this->collidersOBB, colliderName, collider, modelMatrix);
}

void CollidersController::addOrUpdateCollidersSBB(std::string colliderName, AbstractModel::SBB collider, glm::mat4 modelMatrix)
{
  addOrUpdateColliders(this->collidersSBB, colliderName, collider, modelMatrix);
}

void CollidersController::pruebaColisionesSBBvsSBB()
{
  for (auto it = collidersSBB.begin(); it != collidersSBB.end(); ++it)
  {
    bool isCollision = false;
    for (auto jt = collidersSBB.begin(); jt != collidersSBB.end(); ++jt)
    {
      if (it != jt)
      {
        auto &sbb1 = std::get<0>(it->second);
        auto &sbb2 = std::get<0>(jt->second);
        if (testSphereSphereIntersection(sbb1, sbb2))
        {
          std::cout << "Hay colisión entre " << it->first << " y " << jt->first << std::endl;
          isCollision = true;
          resolveSphereSphereCollision(sbb1, sbb2);
        }
      }
    }
    addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
  }
}

void CollidersController::pruebaColisionesOBBvsOBB()
{
  for (auto it = collidersOBB.begin(); it != collidersOBB.end(); ++it)
  {
    bool isCollision = false;
    for (auto jt = collidersOBB.begin(); jt != collidersOBB.end(); ++jt)
    {
      if (it != jt)
      {
        auto &obb1 = std::get<0>(it->second);
        auto &obb2 = std::get<0>(jt->second);
        if (testOBBOBB(obb1, obb2))
        {
          std::cout << "Hay colisión entre " << it->first << " y " << jt->first << std::endl;
          addOrUpdateCollisionDetection2(collisionDetection2, it->first, jt->first);
          addOrUpdateCollisionDetection2(collisionDetection2, jt->first, it->first);

          isCollision = true;
          resolveOBBOBBCollision(obb1, obb2);
        }
      }
    }
    addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
  }
}

void CollidersController::checkAndResolveCollisions(AbstractModel::SBB &sbb1, AbstractModel::SBB &sbb2)
{
  if (testSphereSphereIntersection(sbb1, sbb2))
  {
    resolveSphereSphereCollision(sbb1, sbb2);
  }
}

void CollidersController::checkAndResolveCollisions(AbstractModel::OBB &obb1, AbstractModel::OBB &obb2)
{
  if (testOBBOBB(obb1, obb2))
  {
    resolveOBBOBBCollision(obb1, obb2);
  }
}

void CollidersController::resolveSphereSphereCollision(AbstractModel::SBB &sbb1, AbstractModel::SBB &sbb2)
{
  glm::vec3 collisionNormal = glm::normalize(sbb2.c - sbb1.c);
  float distance = glm::distance(sbb1.c, sbb2.c);
  float penetrationDepth = (sbb1.ratio + sbb2.ratio) - distance;

  if (penetrationDepth > 0.0f)
  {
    glm::vec3 correction = collisionNormal * (penetrationDepth / 2.0f);

    if (sbb1.c.x < sbb2.c.x)
    {
      sbb1.c -= glm::vec3(correction.x, 0.0f, 0.0f);
      sbb2.c += glm::vec3(correction.x, 0.0f, 0.0f);
    }
    else
    {
      sbb1.c += glm::vec3(correction.x, 0.0f, 0.0f);
      sbb2.c -= glm::vec3(correction.x, 0.0f, 0.0f);
    }

    if (sbb1.c.z < sbb2.c.z)
    {
      sbb1.c -= glm::vec3(0.0f, 0.0f, correction.z);
      sbb2.c += glm::vec3(0.0f, 0.0f, correction.z);
    }
    else
    {
      sbb1.c += glm::vec3(0.0f, 0.0f, correction.z);
      sbb2.c -= glm::vec3(0.0f, 0.0f, correction.z);
    }
  }
}

void CollidersController::resolveOBBOBBCollision(AbstractModel::OBB &obb1, AbstractModel::OBB &obb2)
{
  glm::vec3 collisionNormal = glm::normalize(obb2.c - obb1.c);
  float distance = glm::distance(obb1.c, obb2.c);
  float penetrationDepth = glm::length(obb1.e + obb2.e) - distance;

  if (penetrationDepth > 0.0f)
  {
    glm::vec3 correction = collisionNormal * (penetrationDepth / 2.0f);

    if (obb1.c.x < obb2.c.x)
    {
      obb1.c -= glm::vec3(correction.x, 0.0f, 0.0f);
      obb2.c += glm::vec3(correction.x, 0.0f, 0.0f);
    }
    else
    {
      obb1.c += glm::vec3(correction.x, 0.0f, 0.0f);
      obb2.c -= glm::vec3(correction.x, 0.0f, 0.0f);
    }

    if (obb1.c.z < obb2.c.z)
    {
      obb1.c -= glm::vec3(0.0f, 0.0f, correction.z);
      obb2.c += glm::vec3(0.0f, 0.0f, correction.z);
    }
    else
    {
      obb1.c += glm::vec3(0.0f, 0.0f, correction.z);
      obb2.c -= glm::vec3(0.0f, 0.0f, correction.z);
    }
  }
}

bool CollidersController::verificarColision(std::string colliderName)
{
  auto it = this->collisionDetection.find(colliderName);
  if (it != this->collisionDetection.end())
  {
    return it->second;
  }
  else
  {
    std::cerr << "El colisionador '" << colliderName << "' no existe en el mapa." << std::endl;
    return false;
  }
}

bool CollidersController::verificarColision2(std::string collider1, std::string collider2)
{
  auto it = this->collisionDetection2.find(collider1);

  if (it == this->collisionDetection2.end())
  {
    std::cout << "No se encontró un collider llamado " << collider1 << ".\n";
    return false;
  }

  // Verificamos si colisiono con algun objeto que coincida con collider2
  if (it->second.find(collider2) != std::string::npos)
  {
    return true;
  }

  return false;
}
