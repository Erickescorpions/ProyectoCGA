#ifndef CONTADOR_H
#define CONTADOR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Shader.h"
#include "Headers/Box.h"
#include "Headers/FontTypeRendering.h"

class Contador
{
public:
  int contador;
  Shader *shaderTexture;
  Box boxImagenCubo;
  FontTypeRendering::FontTypeRendering *modelText;
  GLuint textureId;

  Contador(Shader *shaderTexture, FontTypeRendering::FontTypeRendering *modelText, GLuint texId);
  ~Contador();
  void update(float dt);
  void render();
  void incrementarContador();
};

#endif