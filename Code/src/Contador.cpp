#include "Contador.h"

Contador::Contador(Shader *shaderTexture, FontTypeRendering::FontTypeRendering *modelText, GLuint texId)
{
  this->shaderTexture = shaderTexture;
  this->modelText = modelText;
  this->textureId = texId;
  this->contador = 0;
  boxImagenCubo.init();
  boxImagenCubo.setShader(shaderTexture);
  boxImagenCubo.setScale(glm::vec3(0.08f, 0.12f, 0.00001f));
}

Contador::~Contador()
{
  boxImagenCubo.destroy();
}

void Contador::render()
{
  shaderTexture->setFloat("scaleUV", 1.0f);
  shaderTexture->setFloat("offsetY", 0.0f);
  shaderTexture->setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
  shaderTexture->setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->textureId);
  shaderTexture->setInt("outTexture", 0);
  glm::mat4 boxMatrixModel = glm::mat4(1.0f);
  boxMatrixModel = glm::translate(boxMatrixModel, glm::vec3(0.7f, 0.9f, 0.0f));
  boxImagenCubo.render(boxMatrixModel);

  std::string text = std::to_string(contador) + " - 10";
  // Renderiza el texto
  modelText->render(text, 0.8f, 0.9f, 1.1f);
}

void Contador::incrementarContador()
{
  this->contador++;
}