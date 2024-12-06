#include "Player.h"
#include <iostream>

std::map<Personaje, std::string> modelosJugador = {
    {Personaje::KRATOS, "../models/kratos/kratos.fbx"},
    {Personaje::NARUTO, "../models/naruto/naruto.fbx"},
    {Personaje::KAKASHI, "../models/kakashi/kakashi.fbx"}};

Player::Player(Shader *shader, CollidersController *cc)
    : scaleFactor(0.01f), vida(MAXIMA_VIDA), tiempoRecuperacion(TIEMPO_RECUPERACION)
{
    this->shader = shader;
    this->cc = cc;
    this->anguloOrientacion = 0.0f;

    this->modelMatrix = glm::mat4(1.0f);
    this->modelMatrixCollider = glm::mat4(1.0f);

    this->posicion = glm::vec3(30.0f, 3.0f, 0.0f);
    this->recibioDanio = false;
    this->contadorTiempoRecuperacion = 0.0f;

    shader->setVectorFloat3("damageColor", glm::value_ptr(glm::vec3(1.0, 0.2, 0.2)));
    shader->setFloat("blinkIntensity", 0.0f);

    this->velocidadCaminando = 3.0f;
    this->velocidadCorriendo = 10.0f;
}

Player::~Player()
{
    this->modelo.destroy();
}

void Player::update(float dt)
{
    // Verificamos si hay colisión con el terreno
    bool hayColision = this->cc->verificarColision("jugador");
    if (hayColision)
    {
        this->posicion = this->modelMatrixCollider[3];
    }

    // Colisión con un enemigo
    bool hayColisionConEnemigo = this->cc->verificarColision2("jugador", "enemigo");
    if (hayColisionConEnemigo && !this->recibioDanio)
    {
        if (this->vida > 0)
        {
            this->vida -= 1;
            this->recibioDanio = true;
            this->contadorTiempoRecuperacion = 0.0f;
        }
    }

    // Colisión con un cubo
    bool hayColisionConCubo = this->cc->verificarColision2("jugador", "cubo");
    if (hayColisionConCubo)
    {
        recogerCubo();
    }

    this->posicion.y = terrain->getHeightTerrain(this->posicion.x, this->posicion.z);
    this->modelMatrix = glm::translate(glm::mat4(1.0f), this->posicion);
    this->modelMatrix = glm::rotate(this->modelMatrix, this->anguloOrientacion, glm::vec3(0.0f, 1.0f, 0.0f));
    this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(this->scaleFactor));

    this->moverJugador(this->accion, dt);
    this->addOrUpdateCollider();
    this->updateBlinkEffect(dt);
}

void Player::render()
{
    this->modelo.render(this->modelMatrix);
}

void Player::updateBlinkEffect(float deltaTime)
{
    float blinkIntensity = 0.0f;
    if (this->recibioDanio)
    {
        this->contadorTiempoRecuperacion += deltaTime;
        blinkIntensity = abs(sin(this->contadorTiempoRecuperacion * 10.0f)); // Oscila entre 0 y 1
        if (this->contadorTiempoRecuperacion >= tiempoRecuperacion)
        {
            blinkIntensity = 0.0f;
            this->recibioDanio = false;
        }
    }

    shader->setFloat("blinkIntensity", blinkIntensity);
}

void Player::recogerCubo()
{
    if (this->vida < MAXIMA_VIDA)
    {
        this->vida += 1;
        std::cout << "Cubo recogido. Vida actual: " << this->vida << std::endl;
    }
}

void Player::setTerrain(Terrain *terrain)
{
    this->terrain = terrain;
}

void Player::setJugador(Personaje jugador)
{
    this->jugadorSeleccionado = jugador;
    this->modelo.loadModel(modelosJugador[jugador]);
    this->modelo.setShader(this->shader);
    this->modelo.setAnimationIndex(2);

    this->modelMatrix = glm::mat4(1.0f);
}

void Player::setAccion(AccionJugador accion)
{
    if (this->accion == accion)
        return;

    this->accion = accion;
    switch (accion)
    {
    case AccionJugador::QUIETO:
        this->modelo.setAnimationIndex(2);
        break;
    case AccionJugador::CAMINANDO:
        this->modelo.setAnimationIndex(1);
        break;
    case AccionJugador::CORRIENDO:
        this->modelo.setAnimationIndex(0);
        break;
    case AccionJugador::REVERSA:
        this->modelo.setAnimationIndex(3);
        break;
    }
}

void Player::addOrUpdateCollider()
{
    this->modelMatrixCollider = glm::mat4(1.0f);

    this->modelMatrixCollider = glm::translate(this->modelMatrixCollider, glm::vec3(this->modelMatrix[3]));
    this->modelMatrixCollider = glm::rotate(this->modelMatrixCollider, this->anguloOrientacion, glm::vec3(0, 1, 0));
    this->modelMatrixCollider = glm::rotate(this->modelMatrixCollider, glm::radians(90.0f), glm::vec3(1, 0, 0));
    this->collider.u = glm::quat_cast(this->modelMatrixCollider);
    this->modelMatrixCollider = glm::scale(this->modelMatrixCollider, glm::vec3(this->scaleFactor));
    this->modelMatrixCollider = glm::translate(this->modelMatrixCollider,
                                               glm::vec3(this->modelo.getObb().c.x, this->modelo.getObb().c.y, this->modelo.getObb().c.z));

    this->collider.e = this->modelo.getObb().e * glm::vec3(this->scaleFactor) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
    this->collider.c = glm::vec3(this->modelMatrixCollider[3]);

    // Agregamos la colisión al controlador de colisiones
    this->cc->addOrUpdateCollidersOBB("jugador", this->collider, this->modelMatrixCollider);
}

void Player::moverJugador(AccionJugador accion, float dt)
{
    switch (accion)
    {
    case AccionJugador::CAMINANDO:
        posicion.x += sin(this->anguloOrientacion) * dt * this->velocidadCaminando;
        posicion.z += cos(this->anguloOrientacion) * dt * this->velocidadCaminando;
        break;

    case AccionJugador::REVERSA:
        posicion.x -= sin(this->anguloOrientacion) * dt * this->velocidadCaminando;
        posicion.z -= cos(this->anguloOrientacion) * dt * this->velocidadCaminando;
        break;

    case AccionJugador::CORRIENDO:
        posicion.x += sin(this->anguloOrientacion) * dt * this->velocidadCorriendo;
        posicion.z += cos(this->anguloOrientacion) * dt * this->velocidadCorriendo;
        break;

    default:
        break;
    }
}
