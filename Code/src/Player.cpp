#include "Player.h"

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
}

Player::~Player()
{
  this->modelo.destroy();
}

void Player::update(float dt)
{
  this->posicion.y = terrain->getHeightTerrain(this->posicion.x, this->posicion.z);
  this->modelMatrix = glm::translate(glm::mat4(1.0f), this->posicion);
  this->modelMatrix = glm::rotate(this->modelMatrix, this->anguloOrientacion, glm::vec3(0.0f, 1.0f, 0.0f));
  this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(this->scaleFactor));

  this->addOrUpdateCollider();

  // vemos si hay colision
  bool hayColision = this->cc->verificarColision("jugador");
  if (hayColision)
  {
    //std::cout << "Jugador colisiono" << std::endl;
    // actualizamos la posicion del jugador con la del collider
    //this->modelMatrix[3].x = this->modelMatrixCollider[3].x;
    //this->modelMatrix[3].z = this->modelMatrixCollider[3].z;
  }

  bool hayColisionConEnemigo = this->cc->verificarColision2("jugador", "enemigo");
  if (hayColisionConEnemigo && this->recibioDanio == false)
  {
    if (this->vida > 0)
    {
      this->vida -= 1;
      this->recibioDanio = true;
      this->contadorTiempoRecuperacion = 0.0f;
    }
  }

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

void printMat4(const glm::mat4 &matrix)
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
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

  // std::cout << "Model matrix" << std::endl;
  // printMat4(modelMatrix);

  // std::cout << "Model matrix collider" << std::endl;
  // printMat4(modelMatrixCollider);

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