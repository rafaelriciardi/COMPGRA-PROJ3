#include "ground.hpp"
#include <filesystem>

void Ground::create(GLuint program) {
  // Unit quad on the xz plane
  std::array<glm::vec3, 8> vertices{{{-0.1f, 0.0f, +0.1f},
                                     {-0.1f, 0.0f, -0.1f},
                                     {+0.1f, 0.0f, +0.1f},
                                     {+0.1f, 0.0f, -0.1f}}};

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  // Save location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Ground::paint() {
  abcg::glBindVertexArray(m_VAO);

  // Draw a grid of 2N+1 x 2N+1 tiles on the xz plane, centered around the origin
  auto const N{50};
  for (auto const z : iter::range(-N, N + 1)) {
    for (auto const x : iter::range(-N, N + 1)) {
      // Calculate the height using a sine wave function
      float amplitude = 1.0f;
      float frequency = 0.1f;
      float phase = 0.0f;
      float height = amplitude * std::sin(frequency * x + phase) * std::sin(frequency * z + phase);
      height = std::round(height / 0.1f) * 0.1f;
      // Set model matrix as a translation matrix with the adjusted height
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(x/5.0f, height, z/5.0f));
      abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      // Set color (checkerboard pattern
      if(height < 0){
        abcg::glUniform4f(m_colorLoc, 0.0f, 0.0f, 0.75f, 1.0f); 
      }else if (height > 0.7) {
        abcg::glUniform4f(m_colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); 
      } else {
        abcg::glUniform4f(m_colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); 
      }
      abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
  }

  abcg::glBindVertexArray(0);
}


void Ground::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Ground::loadDiffuseTexture(std::string_view path) {
  if (!std::filesystem::exists(path))
    return;

  abcg::glDeleteTextures(1, &m_diffuseTexture);
  m_diffuseTexture = abcg::loadOpenGLTexture({.path = path});
}