#include "Cube.h"

Cube::Cube(const std::string& modelPath, Shader* shader) {
    this->shader = shader;
    this->modelo.loadModel(modelPath);
    this->modelo.setShader(shader);

    // Posición inicial del cubo
    this->modelMatrix = glm::mat4(1.0f);
    this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
    this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(0.5f)); // Escala del cubo
}

Cube::~Cube() {}

void Cube::update(float dt, glm::vec3 targetPosition) {
    // Opcional: lógica de movimiento hacia el target, si es necesario.
    // Aquí dejamos el cubo estático, pero se puede implementar similar al enemigo.
}

void Cube::render() {
    this->modelo.render(this->modelMatrix);
}
