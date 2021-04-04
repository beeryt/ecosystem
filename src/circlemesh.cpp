#include "circlemesh.h"
#include <array>
  template <unsigned count>
constexpr auto generate_circle_data()
{
  using std::array;
  using std::pair;
  array<Mesh::Vertex, count + 2> vertices = {};
  array<unsigned, count + 2> indices = {};

  // center of circle
  vertices[0] = Mesh::Vertex({0, 0, 0});
  indices[0] = 0;

  // triangle fan vertices
  for (unsigned i = 0; i <= count; ++i)
  {
    constexpr float radius = 0.5;
    float x = (radius * cos(i * 2 * M_PI / count));
    float y = (radius * sin(i * 2 * M_PI / count));
    vertices[i] = Mesh::Vertex({x, y, 0});
    indices[i] = i + 1;
  }

  return std::make_pair(vertices, indices);
}

template <unsigned count>
constexpr auto generate_circle_vertices() { return generate_circle_data<count>().first; }

template <unsigned count>
constexpr auto generate_circle_indices() { return generate_circle_data<count>().second; }

constexpr unsigned TRIANGLE_COUNT = 64;
constexpr static auto circle_vertices = generate_circle_vertices<TRIANGLE_COUNT>();
constexpr static auto circle_indices = generate_circle_indices<TRIANGLE_COUNT>();
constexpr static auto circle_textures = std::array<std::shared_ptr<Texture>, 0>{};

CircleMesh::CircleMesh() : Mesh(circle_vertices, circle_indices, circle_textures) { type = TRIANGLE_FAN; }
