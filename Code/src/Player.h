#ifndef PLAYER_H
#define PLAYER_H

// GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Model.h"
#include "Headers/Shader.h"
#include "Headers/Terrain.h"
#include "Headers/AbstractModel.h"
#include "CollidersController.h"

#include <iostream>
#include <map>
#include <string>

#define MAXIMA_VIDA 4
#define TIEMPO_RECUPERACION 5

enum AccionJugador
{
    CAMINANDO = 0,
    CORRIENDO,
    QUIETO,
    REVERSA
};

enum Personaje
{
    KAKASHI = 0,
    KRATOS = 1,
    NARUTO = 2
};

extern std::map<Personaje, std::string> modelosJugador;

class Player
{
public:
    glm::vec3 posicion;
    glm::mat4 modelMatrix;
    AbstractModel::OBB collider;
    float anguloOrientacion;

    Player(Shader *shader, CollidersController *cc);
    ~Player();
    void update(float dt);
    void render();
    void setTerrain(Terrain *terrain);
    void setJugador(Personaje jugador);
    void setAccion(AccionJugador accion);
    void moverJugador(AccionJugador accion, float dt);

    // Obtener vida actual
    int getVida()
    {
        return this->vida;
    }

    // **Nuevo método: recoger un cubo y aumentar vida**
    void recogerCubo();

private:
    Terrain *terrain;
    Shader *shader;
    Model modelo;
    Personaje jugadorSeleccionado;
    AccionJugador accion;
    float scaleFactor;
    float contadorTiempoRecuperacion;
    int tiempoRecuperacion;
    bool recibioDanio;

    // Para colisiones
    glm::mat4 modelMatrixCollider;
    CollidersController *cc;

    int vida;
    float velocidadCaminando;
    float velocidadCorriendo;

    void addOrUpdateCollider();
    void updateBlinkEffect(float deltaTime);
};

#endif // PLAYER_H
