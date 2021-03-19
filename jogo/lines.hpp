#include "abcg.hpp"
#include "gamedata.hpp"

class Lines {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{255,255,0,0};
  float m_scale{1};
  glm::vec2 m_translation{glm::vec2(0)};
};