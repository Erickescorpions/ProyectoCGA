#ifndef CUBE_H
#define CUBE_H

// Incluye GLM y el sistema de shaders
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Headers/Model.h"
#include "Headers/Shader.h"
#include "Headers/Terrain.h"
#include "CollidersController.h"
#include "Headers/FontTypeRendering.h"
#include "Player.h" 

class Cube {
private:
    Model modelo;                      
    Shader* shader;                    
    glm::mat4 modelMatrix;             
    glm::mat4 modelMatrixCollider;    
    Terrain* terrain;                 
    CollidersController* cc;           
    AbstractModel::OBB collider;       
    std::string colliderName;          
    bool cuboAgarrado;             
    Player* jugador;                   

public:
    Cube(const std::string& modelPath, Shader* shader, CollidersController* cc, glm::vec3 posicionInicial, Player* jugador); 
    ~Cube();

    void setTerrain(Terrain* terrain); 
    bool update(float dt, glm::vec3 targetPosition, float proximidadUmbral); 
    void render();                     
    void addOrUpdateColliders();       
    glm::mat4 getModelMatrix() const { return modelMatrix; }
    glm::vec3 getPosition() const { return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]); }
};

#endif // CUBE_H
