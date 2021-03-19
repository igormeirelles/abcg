#include "asteroids.hpp"

#include <cstdlib>
#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Asteroids::initializeGL(GLuint program, int quantity) {
  terminateGL();

  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  // Create asteroids
  m_asteroids.clear();
  m_asteroids.resize(quantity);

  for (auto &asteroid : m_asteroids) {
    asteroid = createAsteroid();
    float lanes[3] = {-0.66f,0,0.66f};
    srand(time(0));
    int sort = rand() % 3;
    asteroid.m_translation = {lanes[sort],1};
  }
}

void Asteroids::paintGL() {
  glUseProgram(m_program);

  for (auto &asteroid : m_asteroids) {
    glBindVertexArray(asteroid.m_vao);

    glUniform4fv(m_colorLoc, 1, &asteroid.m_color.r);
    glUniform1f(m_scaleLoc, asteroid.m_scale);
    glUniform1f(m_rotationLoc, asteroid.m_rotation);

    glUniform2f(m_translationLoc, asteroid.m_translation.x,
                    asteroid.m_translation.y);

     glDrawArrays(GL_TRIANGLE_FAN, 0, asteroid.m_polygonSides + 2);

    glBindVertexArray(0);
  }

  glUseProgram(0);
}

void Asteroids::terminateGL() {
  for (auto asteroid : m_asteroids) {
    glDeleteBuffers(1, &asteroid.m_vbo);
    glDeleteVertexArrays(1, &asteroid.m_vao);
  }
}

void Asteroids::update(const Ship &ship, float deltaTime) {
  for (auto &asteroid : m_asteroids) {   
    asteroid.m_translation -= ship.m_velocity * deltaTime;
  
    if (asteroid.m_translation.y < -2.5f) {
      float lanes[3] = {-0.66f,0,0.66f};
      srand(time(0));
      int sort = rand() % 3;
      asteroid.m_translation = {lanes[sort],2};
  }
  }
}

Asteroids::Asteroid Asteroids::createAsteroid(glm::vec2 translation,
                                              float scale) {
  Asteroid asteroid;

  auto &re{m_randomEngine};  // Shortcut

  // Randomly choose the number of sides
  std::uniform_int_distribution<int> randomSides(6, 20);
  asteroid.m_polygonSides = randomSides(re);

  // Choose a random color
  std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
  asteroid.m_color = glm::vec4(0,1,0,0) * randomIntensity(re);

  asteroid.m_color.a = 1.0f;
  asteroid.m_rotation = 0.0f;
  asteroid.m_scale = scale;

  asteroid.m_translation = translation;
  asteroid.m_translation.x = 0;

  // Create geometry
  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  auto step{M_PI * 2 / asteroid.m_polygonSides};
  std::uniform_real_distribution<float> randomRadius(0.8f, 1.0f);
  for (auto angle : iter::range(0.0, M_PI * 2, step)) {
    auto radius{randomRadius(re)};
    positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO
  glGenBuffers(1, &asteroid.m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, asteroid.m_vbo);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
               positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  glGenVertexArrays(1, &asteroid.m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(asteroid.m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, asteroid.m_vbo);
  glEnableVertexAttribArray(positionAttribute);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);

  return asteroid;
}
