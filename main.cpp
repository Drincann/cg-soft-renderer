// #include "./libcanvas.cpp"

#include <cmath>
#include <vector>
#include "./lib/geometry.h"
#include "./lib/libcanvas.cpp"
#include "./lib/model.cpp"
#include "./lib/tgaimage.cpp"

int main(int argc, char** argv) {
  Model* model = nullptr;
  if (2 == argc) {
    model = new Model(argv[1]);
  } else {
    model = new Model("obj/african_head.obj");
  }
  int w = 4000, h = 4000;
  Canvas* canvas = new Canvas(w, h);
  CanvasContext* ctx = canvas->createContext();
  std::vector<const TGAColor*> colors = {
      Canvas::COLOR_RED,    Canvas::COLOR_WHITE,  Canvas::COLOR_GREEN,
      Canvas::COLOR_BLUE,   Canvas::COLOR_YELLOW, Canvas::COLOR_PURPLE,
      Canvas::COLOR_ORANGE, Canvas::COLOR_CYAN,   Canvas::COLOR_GRAY};

  for (int i = 0; i < model->nfaces(); i++) {
    std::vector<int> face = model->face(i);
    Vec3f v0 = ((model->vert(face[0]) + (Vec3f){1., 1., 1.}));
    Vec3f v1 = ((model->vert(face[1]) + (Vec3f){1., 1., 1.}));
    Vec3f v2 = ((model->vert(face[2]) + (Vec3f){1., 1., 1.}));

    ctx->setColor(colors[i % colors.size()]);
    for (int j = 0; j < 3; j++) {
      ctx->triangle({static_cast<int>(v0.x * w / 2.),
                     static_cast<int>(w - v0.y * h / 2.)},
                    {static_cast<int>(v1.x * w / 2.),
                     static_cast<int>(w - v1.y * h / 2.)},
                    {static_cast<int>(v2.x * w / 2.),
                     static_cast<int>(w - v2.y * h / 2.)});
    }
  }

  canvas->save("./output.tga");
  return 0;
}