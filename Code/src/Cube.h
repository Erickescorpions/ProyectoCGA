#ifndef CUBE_H
#define CUBE_H

// Incluye GLM y el sistema de shaders
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Headers/Model.h"
#include "Headers/Shader.h"
#include "Headers/Terrain.h"

class Cube {
public:
    Model modelo;
    glm::mat4 modelMatrix;
    Shader* shader;
    Terrain* terrain;  // Puntero al terreno
    bool cuboAgarrado = false;

    // Constructor
    Cube(const std::string& modelPath, Shader* shader, glm::vec3 posicionInicial);

    // Destructor
    ~Cube();

    // Método para establecer el terreno
    void setTerrain(Terrain* terrain);

    // Métodos para actualizar y renderizar
    void update(float dt, glm::vec3 targetPosition);
    void render();
};

#endif // CUBE_H
