#include "Player.h"

std::map<Personaje, std::map<std::string, std::string>> modelosJugador = {
    {Personaje::KRATOS, {{"caminando", "../models/kratos/KratosCaminando.fbx"}, {"corriendo", "../models/kratos/KratosRun.fbx"}, {"quieto", "../models/kratos/KratosQuieto.fbx"}, {"reversa", "../models/kratos/KratosReversa.fbx"}}},
    {Personaje::NARUTO, {{"caminando", "../models/naruto/NarutoCaminando.fbx"}, {"corriendo", "../models/naruto/NarutoRun.fbx"}, {"quieto", "../models/naruto/NarutoQuieto.fbx"}, {"reversa", "../models/naruto/NarutoReversa.fbx"}}},
    {Personaje::KAKASHI, {{"caminando", "../models/kakashi/KakashiCaminando.fbx"}, {"corriendo", "../models/kakashi/KakashiRun.fbx"}, {"quieto", "../models/kakashi/KakashiQuieto.fbx"}, {"reversa", "../models/kakashi/KakashiReversa.fbx"}}}};

Player::Player(Shader *shader)
{
  this->shader = shader;

  this->modelMatrix = glm::mat4(1.0f);
  this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(30.0f, 0.05f, 0.0f));
  this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(0.01f));
  this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
}

void Player::render()
{
  glm::mat4 modelMatrixBody = glm::mat4(this->modelMatrix);
  modelMatrixBody[3][1] = terrain->getHeightTerrain(modelMatrixBody[3][0], modelMatrixBody[3][2]);
  this->modelo->render(modelMatrixBody);
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
    this->modelo = &this->modeloQuieto;
    break;
  }
}