#include "graphics.h"
#include "mesh.h"
#include "shader.h"
#include "version.h"
#include "camera.h"
#include <spdlog/spdlog.h>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <iostream>
#define WINX 320
#define WINY 320
#include "grid.h"
#include "random.h"
#include "circlemesh.h"

glm::vec3 white{1.0, 1.0, 1.0};
glm::vec3 red{1.0, 0.0, 0.0};

auto printMat4 = [](glm::mat4 v, const std::string &message = "") {
  spdlog::info(message);
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      printf("%+3.1f ", v[i][j]);
    }
    printf("\n");
  }
};

#include <unistd.h>
int main()
{
  //spdlog::set_level(spdlog::level::debug);
  spdlog::trace("{}", __PRETTY_FUNCTION__);

  auto title = "Ecosystem " VERSION_STRING;
  auto win = Graphics::CreateWindow(WINX, WINY, title);
  spdlog::info("Launching {}", title);

  Shader s{ "shaders/triangle.vs", "shaders/shader.fs" };
  s.use();
  while (!win.ShouldClose())
  {
    Graphics::HandleEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    static CircleMesh cm;
    cm.Draw(s);
    win.SwapBuffers();
  }
}
