#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Cube::Cube(const std::string& modelPath, Shader* shader, CollidersController* cc, glm::vec3 posicionInicial, Contador* c)
    : shader(shader), cc(cc), cuboAgarrado(false) { 
    this->modelo.loadModel(modelPath);
    this->modelo.setShader(shader);
    this->modelMatrix = glm::mat4(1.0f);
    this->posicion = posicionInicial;

    this->terrain = nullptr;
    this->contador = c;

    // Generar un nombre único para el collider
    this->colliderName = "cubo" + std::to_string(rand());
}

Cube::~Cube() {}

void Cube::setTerrain(Terrain* terrain) {
    this->terrain = terrain;
}

void Cube::update(float dt, glm::vec3 targetPosition) {
    // Animación flotante
    static float timeCounter = 0.0f;
    timeCounter += dt;
    float floatingOffset = sin(timeCounter * 2.0f) * 0.5f + 2.0f;

    float posX = posicion.x;
    float posZ = posicion.z;
    this->posicion.y = terrain->getHeightTerrain(posX, posZ) + floatingOffset;

    // Actualizar posición del modelo
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, this->posicion);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(timeCounter * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

    addOrUpdateColliders();

    // Verificar colisiones
    if (cc->verificarColision2(colliderName, "jugador") && !cuboAgarrado) {
        cuboAgarrado = true;
        this->contador->incrementarContador();
    }
}

void Cube::render() {
    if (!cuboAgarrado) {
        modelo.render(modelMatrix);
    }
}

void Cube::addOrUpdateColliders() {
    if(cuboAgarrado) {
        cc->removeCollidersOBB(colliderName);
        return;
    }

    modelMatrixCollider = glm::mat4(this->modelMatrix);
    //modelMatrixCollider = glm::translate(modelMatrixCollider, glm::vec3(modelMatrix[3]));
    //modelMatrixCollider = glm::scale(modelMatrixCollider, glm::vec3(0.5f));
    collider.u = glm::quat_cast(modelMatrixCollider);
    collider.e = modelo.getObb().e * glm::vec3(0.5f);
    collider.c = glm::vec3(modelMatrixCollider[3]);

    cc->addOrUpdateCollidersOBB(colliderName, collider, modelMatrixCollider);
}

void Cube::removeCollider()
{
    cc->removeCollidersOBB(colliderName);
}