#include "Cube.h"
// Include Terrain
#include "Headers/Terrain.h"

// Variables globales para animaciones
float timeCounter = 0.0f;   // Lleva un contador del tiempo para la animación
float floatAmplitude = 0.5f;  // Amplitud del movimiento flotante
float floatSpeed = 1.0f;      // Velocidad del movimiento flotante
float rotationSpeed = 50.0f;  // Velocidad de rotación (en grados por segundo)

// Constructor
Cube::Cube(const std::string& modelPath, Shader* shader) {
    this->shader = shader;
    this->modelo.loadModel(modelPath);
    this->modelo.setShader(shader);

    // Posición inicial del cubo
    this->modelMatrix = glm::mat4(1.0f);
   // this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f)); // Posición inicial
    //this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(0.5f)); // Escala del cubo

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
    float floatingOffset = sin(timeCounter * floatSpeed) * floatAmplitude;

    // Extrae la posición X y Z de la matriz del cubo
    float posX = modelMatrix[3][0];
    float posZ = modelMatrix[3][2];

    // Calcula la nueva posición Y basada en la altura del terreno y el offset flotante
    float posY = terrain->getHeightTerrain(posX, posZ) + floatingOffset;

    // Reinicia la matriz del cubo y aplica las transformaciones
    modelMatrix = glm::mat4(1.0f); // Reinicia la matriz
    // Ahora usamos glm::vec3 para crear una vec4 correctamente al trasladar
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, posY, posZ)); // Traslación

    // Si el cubo no ha sido agarrado, aplica la rotación
    if (!cuboAgarrado) {
        float angle = rotationSpeed * timeCounter; // Calcula el ángulo de rotación
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    // Si el cubo ha sido agarrado, no aplicamos rotación adicional, pero lo dejamos en la misma posición.
}

// Método de renderizado
void Cube::render() {
    this->modelo.render(this->modelMatrix);
}
