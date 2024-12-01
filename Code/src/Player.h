#ifndef PLAYER_H
#define PLAYER_H

// GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include loader Model class
#include "Headers/Model.h"
#include "Headers/Shader.h"
#include "Headers/Terrain.h"

#include <iostream>
#include <map>
#include <string>

extern std::map<std::string, std::map<std::string, std::string>> modelosJugador;

class Player
{
public:
  glm::vec3 posicion;
  glm::mat4 modelMatrix;
  Model modelo;
  std::string jugadorSeleccionado;

  Player(Shader* shader);
  void render();
  void setTerrain(Terrain *terrain);
  void setJugador(std::string jugador);

private:
  Terrain* terrain;
  Shader* shader;
};

#endif // PLAYER_H