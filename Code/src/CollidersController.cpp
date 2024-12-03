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
  for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
           collidersSBB.begin();
       it != collidersSBB.end(); it++)
  {
    bool isCollision = false;
    for (std::map<std::string,
                  std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt =
             collidersSBB.begin();
         jt != collidersSBB.end(); jt++)
    {
      if (it != jt && testSphereSphereIntersection(
                          std::get<0>(it->second), std::get<0>(jt->second)))
      {
        std::cout << "Hay collision entre " << it->first << " y el modelo " << jt->first << std::endl;
        isCollision = true;
      }
    }

    addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
  }
}

void CollidersController::pruebaColisionesOBBvsOBB()
{
  for (std::map<std::string,
                std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
           collidersOBB.begin();
       it != collidersOBB.end(); it++)
  {
    bool isColision = false;
    for (std::map<std::string,
                  std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
             collidersOBB.begin();
         jt != collidersOBB.end(); jt++)
    {
      if (it != jt &&
          testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second)))
      {
        std::cout << "Hay colision entre " << it->first << " y el modelo" << jt->first << std::endl;
        isColision = true;
      }
    }
    addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
  }
}