#include "graphics.h"
#include "shader.h"
#include "grid.h"
#include "version.h"
#include "random.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#define WINX 320
#define WINY 320

struct Agent {
  glm::vec2 position;
  glm::vec2 velocity;
  glm::vec2 force;
  float mass = 10.0f;
  float radius = 1.0f;
  void draw(Shader& shader);
};

class Game {
  public:
    Game();
    void update(double dt);
    void render();
  private:
    std::vector<std::shared_ptr<Agent>> agents;
    Grid<Agent, 4, 4> grid;
    Shader shader{ "shaders/triangle.vs", "shaders/shader.fs" };
};

int main()
{
  //spdlog::set_level(spdlog::level::debug);
  spdlog::trace("{}", __PRETTY_FUNCTION__);

  auto title = "Ecosystem " VERSION_STRING;
  auto win = Graphics::CreateWindow(WINX, WINY, title);
  spdlog::info("Launching {}", title);

  Game game;

  while (!win.ShouldClose())
  {
    using Clock = std::chrono::high_resolution_clock;
    static auto start_time = Clock::now();
    auto elapsed = Clock::now() - start_time;
    double dt = std::chrono::duration_cast<std::chrono::duration<double>>(elapsed).count();
    start_time += elapsed;

    Graphics::HandleEvents(0.5);
    glClear(GL_COLOR_BUFFER_BIT);
    game.update(dt);
    game.render();
    win.SwapBuffers();
  }
}

Game::Game() {
  auto agent = std::make_shared<Agent>();
  agent->force = Random::vec2(glm::vec2{-.2}, glm::vec2{.2});
  spdlog::info("agent.position: ({},{})", agent->position.x, agent->position.y);
  spdlog::info("agent.force: ({},{})", agent->force.x, agent->force.y);
  spdlog::info("agent.radius: {}", agent->radius);
  agents.push_back(agent);
}

void Game::update(double dt) {
  for (auto agent : agents) {
    agent->position += agent->velocity * static_cast<float>(dt);
    agent->velocity += (agent->force / agent->mass) * static_cast<float>(dt);
  }
}

void Game::render() {
  shader.use();
  for (auto agent : agents) {
    agent->draw(shader);
    spdlog::debug("agent.position: ({},{})", agent->position.x, agent->position.y);
  }
}

#include "circlemesh.h"
void Agent::draw(Shader& shader) {
  auto model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0));
  model = glm::scale(model, glm::vec3(radius * 0.1f));
  shader.setMat4("Model", model);
  static CircleMesh cm;
  cm.Draw(shader);
}
