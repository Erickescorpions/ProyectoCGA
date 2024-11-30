#include "Cube.h"
// Include Terrain
#include "Headers/Terrain.h"

// Variables globales para animaciones
float timeCounter = 0.0f;   // Lleva un contador del tiempo para la animación
float floatAmplitude = 0.5f;  // Amplitud del movimiento flotante
float floatSpeed = 1.0f;      // Velocidad del movimiento flotante
float rotationSpeed = 50.0f;  // Velocidad de rotación (en grados por segundo)

// Constructor
Cube::Cube(const std::string& modelPath, Shader* shader, glm::vec3 posicionInicial) {
    this->shader = shader;
    this->modelo.loadModel(modelPath);
    this->modelo.setShader(shader);

    // Posición inicial del cubo
    this->modelMatrix = glm::mat4(1.0f);
    this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(0.5f)); // Escala del cubo
    this->modelMatrix = glm::translate(this->modelMatrix, posicionInicial); // Posición inicial

    this->terrain = nullptr; // Asegúrate de que el puntero de terreno esté vacío inicialmente
}

// Destructor
Cube::~Cube() {}

// Método para asignar el terreno
void Cube::setTerrain(Terrain* terrain) {
    this->terrain = terrain;
}

void Cube::update(float dt, glm::vec3 targetPosition) {
    // Incrementa el contador de tiempo
    timeCounter += dt;

    // Calcula la posición flotante en el eje Y
    float floatingOffset = sin(timeCounter * floatSpeed) * floatAmplitude + 2.0f;

    // Extrae la posición actual del cubo
    float posX = modelMatrix[3][0];
    float posZ = modelMatrix[3][2];

    // Reinicia la matriz de modelo y aplica las transformaciones
    modelMatrix = glm::mat4(1.0f); // Reinicia la matriz
    glm::vec3 position(posX, terrain->getHeightTerrain(posX, posZ) + floatingOffset, posZ);
    modelMatrix = glm::translate(modelMatrix, position); // Aplica la traslación

    if (!cuboAgarrado) {
        static float rotationAngle = 0.0f; // Ángulo acumulativo de rotación
        rotationAngle += rotationSpeed * dt; // Incrementa el ángulo
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación
    }

    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f)); // Aplica la escala al final
}

// Método de renderizado
void Cube::render() {
    this->modelo.render(this->modelMatrix);
}
