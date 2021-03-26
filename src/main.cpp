#include <graphics.h>
#define WINX 320
#define WINY 240

int main() {
  /// @todo Extract version number from build system.
  Graphics gfx("Ecosystem v0.1.0", WINX, WINY);
  while (gfx.loop()) {
    gfx.clear();
    gfx.swap();
  }
}
