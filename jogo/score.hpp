#include "abcg.hpp"
#include "gamedata.hpp"


class Score {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();
  void update(const GameData &gameData, float deltaTime);

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{1};
  float m_scale{0.200f};
  float m_rotation{};
  glm::vec2 m_translation{glm::vec2(0)};

};

#endif