#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Cube::Cube(const std::string& modelPath, Shader* shader, CollidersController* cc, glm::vec3 posicionInicial, Player* jugador)
    : shader(shader), cc(cc), cuboAgarrado(false), jugador(jugador) { 
    this->modelo.loadModel(modelPath);
    this->modelo.setShader(shader);
    this->modelMatrix = glm::mat4(1.0f);
    this->modelMatrix = glm::translate(this->modelMatrix, posicionInicial);
    this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(0.5f));
    this->terrain = nullptr;

    // Generar un nombre único para el collider
    this->colliderName = "cubo" + std::to_string(rand());
}

Cube::~Cube() {}

void Cube::setTerrain(Terrain* terrain) {
    this->terrain = terrain;
}

bool Cube::update(float dt, glm::vec3 targetPosition, float proximidadUmbral) {
    // Animación flotante
    static float timeCounter = 0.0f;
    timeCounter += dt;
    float floatingOffset = sin(timeCounter * 2.0f) * 0.5f + 2.0f;

    float posX = modelMatrix[3][0];
    float posZ = modelMatrix[3][2];
    glm::vec3 position(posX, terrain->getHeightTerrain(posX, posZ) + floatingOffset, posZ);

    // Verificar proximidad con el jugador
    if (glm::distance(targetPosition, position) < proximidadUmbral) {
        cuboAgarrado = true;

        if (jugador) {
            jugador->recogerCubo();
        }

        return true; 
    }

    // Actualizar posición del modelo
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(timeCounter * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

    addOrUpdateColliders();

    // Verificar colisiones
    if (cc->verificarColision(colliderName)) {
        cuboAgarrado = true;

        // Notificar al jugador que recogió el cubo
        if (jugador) {
            jugador->recogerCubo();
        }

        return true; // El cubo fue recogido debido a colisión
    }

    return false;
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

    modelMatrixCollider = glm::mat4(1.0f);
    modelMatrixCollider = glm::translate(modelMatrixCollider, glm::vec3(modelMatrix[3]));
    modelMatrixCollider = glm::scale(modelMatrixCollider, glm::vec3(0.5f));
    collider.u = glm::quat_cast(modelMatrixCollider);
    collider.e = modelo.getObb().e * glm::vec3(0.5f);
    collider.c = glm::vec3(modelMatrixCollider[3]);

    cc->addOrUpdateCollidersOBB(colliderName, collider, modelMatrixCollider);
}
