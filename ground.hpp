#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"
#include <vector>

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};
  glm::vec4 Ka;
  glm::vec4 Kd;
  glm::vec4 Ks;
  float shininess;

  bool operator==(const Vertex& other) const noexcept {
    static const auto epsilon{std::numeric_limits<float>::epsilon()};
    return glm::all(glm::epsilonEqual(position, other.position, epsilon)) &&
           glm::all(glm::epsilonEqual(normal, other.normal, epsilon)) &&
           glm::all(glm::epsilonEqual(texCoord, other.texCoord, epsilon)) &&
           glm::all(glm::epsilonEqual(Ka, other.Ka, epsilon)) &&
           glm::all(glm::epsilonEqual(Kd, other.Kd, epsilon)) &&
           glm::all(glm::epsilonEqual(Ks, other.Ks, epsilon));
  }
};

class Ground {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void generateMountainHeight(float x, float z);
  void loadDiffuseTexture(std::string_view path);
  void loadObj(std::string_view path, bool standardize = true);

private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  GLuint m_diffuseTexture{};
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  bool m_hasNormals{false};
  bool m_hasTexCoords{false};
};

#endif