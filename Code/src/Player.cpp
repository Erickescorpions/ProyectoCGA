#include "Player.h"

std::map<Personaje, std::map<std::string, std::string>> modelosJugador = {
    {Personaje::KRATOS, {{"caminando", "../models/kratos/KratosCaminando.fbx"}, {"corriendo", "../models/kratos/KratosRun.fbx"}, {"quieto", "../models/kratos/KratosQuieto.fbx"}, {"reversa", "../models/kratos/KratosReversa.fbx"}}},
    {Personaje::NARUTO, {{"caminando", "../models/naruto/NarutoCaminando.fbx"}, {"corriendo", "../models/naruto/NarutoRun.fbx"}, {"quieto", "../models/naruto/NarutoQuieto.fbx"}, {"reversa", "../models/naruto/NarutoReversa.fbx"}}},
    {Personaje::KAKASHI, {{"caminando", "../models/kakashi/KakashiCaminando.fbx"}, {"corriendo", "../models/kakashi/KakashiRun.fbx"}, {"quieto", "../models/kakashi/KakashiQuieto.fbx"}, {"reversa", "../models/kakashi/KakashiReversa.fbx"}}}};

Player::Player(Shader *shader, CollidersController *cc)
    : scaleFactor(0.01f)
{
  this->shader = shader;
  this->cc = cc;
  this->anguloOrientacion = 0.0f;

  this->modelMatrix = glm::mat4(1.0f);
  this->modelMatrixCollider = glm::mat4(1.0f);

  this->posicion = glm::vec3(30.0f, 3.0f, 0.0f);
  this->modelMatrix = glm::translate(this->modelMatrix, this->posicion);
  this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(this->scaleFactor));
  this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
}

Player::~Player()
{
  this->modeloCaminando.destroy();
  this->modeloCorriendo.destroy();
  this->modeloQuieto.destroy();
  this->modeloReversa.destroy();
}

void Player::update(float dt)
{
  this->posicion.y = terrain->getHeightTerrain(this->posicion.x, this->posicion.z);
  this->modelMatrix = glm::translate(glm::mat4(1.0f), this->posicion);
  this->modelMatrix = glm::rotate(this->modelMatrix, this->anguloOrientacion, glm::vec3(0.0f, 1.0f, 0.0f));
  this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(this->scaleFactor));

  this->addOrUpdateCollider();
}

void Player::render()
{
  this->modelo->render(this->modelMatrix);
}

void Player::setTerrain(Terrain *terrain)
{
  this->terrain = terrain;
}

void Player::setJugador(Personaje jugador)
{
  this->jugadorSeleccionado = jugador;
  this->modeloQuieto.loadModel(modelosJugador[jugador]["quieto"]);
  this->modeloQuieto.setShader(this->shader);
  this->modeloCaminando.loadModel(modelosJugador[jugador]["caminando"]);
  this->modeloCaminando.setShader(this->shader);
  this->modeloCorriendo.loadModel(modelosJugador[jugador]["corriendo"]);
  this->modeloCorriendo.setShader(this->shader);
  this->modeloReversa.loadModel(modelosJugador[jugador]["reversa"]);
  this->modeloReversa.setShader(this->shader);
  this->modelo = &this->modeloQuieto;
}

void Player::setAccion(AccionJugador accion)
{
  if (this->accion == accion)
    return;

  this->accion = accion;
  switch (accion)
  {
  case AccionJugador::QUIETO:
    this->modelo = &this->modeloQuieto;
    break;
  case AccionJugador::CAMINANDO:
    this->modelo = &this->modeloCaminando;
    break;
  case AccionJugador::CORRIENDO:
    this->modelo = &this->modeloCorriendo;
    break;
  case AccionJugador::REVERSA:
    this->modelo = &this->modeloReversa;
    break;
  }
}

void Player::addOrUpdateCollider()
{
  this->modelMatrixCollider = glm::mat4(1.0f);
  this->modelMatrixCollider = glm::rotate(this->modelMatrixCollider, this->anguloOrientacion, glm::vec3(0, 1, 0));
  this->collider.u = glm::quat_cast(this->modelMatrixCollider);
  this->modelMatrixCollider = glm::scale(this->modelMatrixCollider, glm::vec3(this->scaleFactor));
  this->modelMatrixCollider = glm::translate(this->modelMatrixCollider,
                                             glm::vec3(this->modelo->getObb().c.x, this->modelo->getObb().c.y, this->modelo->getObb().c.z));

  this->collider.e = this->modelo->getObb().e * glm::vec3(1.15f) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
  this->collider.c = glm::vec3(this->modelMatrix[3]);
  this->collider.c.y += 1.8f;

  // agregamos la colision al collider controller
  this->cc->addOrUpdateCollidersOBB("jugador", this->collider, this->modelMatrixCollider);
}

void Player::moverJugador(AccionJugador accion, int direccion)
{
  if (accion == AccionJugador::CAMINANDO)
  {
    if (direccion == 1)
    {
      posicion.x += sin(this->anguloOrientacion) * 0.1f;
      posicion.z += cos(this->anguloOrientacion) * 0.1f;
    }
    else
    {
      posicion.x -= sin(this->anguloOrientacion) * 0.1f;
      posicion.z -= cos(this->anguloOrientacion) * 0.1f;
    }
  }
  else if (accion == AccionJugador::CORRIENDO)
  {
    posicion.x += sin(this->anguloOrientacion) * 0.3f;
    posicion.z += cos(this->anguloOrientacion) * 0.3f;
  }
}