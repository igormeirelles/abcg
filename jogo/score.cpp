#include "ship.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Ship::initializeGL(GLuint program) {
  
  using std::chrono::system_clock;

  std::chrono::duration<int,std::ratio<60*60*24> > one_day (1);

  system_clock::time_point today = system_clock::now();
  system_clock::time_point tomorrow = today + one_day;

  std::time_t tt;

  tt = system_clock::to_time_t ( today );
  

  tt = system_clock::to_time_t ( tomorrow );
  

  return 0;
}

void Ship::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);
  glBindVertexArray(m_vao);
  
  glUniform1f(m_scaleLoc, m_scale);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  glUseProgram(0);
}

void Ship::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void Ship::update(const GameData &gameData, float deltaTime) {
  
  }
}
