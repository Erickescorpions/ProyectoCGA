#ifndef CUBE_H
#define CUBE_H

// Incluye GLM y el sistema de shaders
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Headers/Model.h"
#include "Headers/Shader.h"

class Cube {
public:
    Model modelo;
    glm::mat4 modelMatrix;
    Shader* shader;

    // Constructor
    Cube(const std::string& modelPath, Shader* shader);

    // Destructor
    ~Cube();

    void update(float dt, glm::vec3 targetPosition);
    void render();
};

#endif // CUBE_H
