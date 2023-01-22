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

  int w = 800, h = 800;
  Canvas* canvas = new Canvas(w, h);
  CanvasContext* ctx = canvas->createContext();

  for (int i = 0; i < model->nfaces(); ++i) {
    std::vector<int> face = model->face(i);
    Vec3f v0 = model->vert(face[0]);
    Vec3f v1 = model->vert(face[1]);
    Vec3f v2 = model->vert(face[2]);
    float light = ((v2 - v0) ^ (v1 - v0)).normalize() * (Vec3f){0, 0, -1.};

    if (light < 0)
      continue;

    ctx->setColor(new TGAColor(255 * light, 150 * light * 0.3,
                               150 * light * 0.3, 255 * light));
    v0 = v0 + (Vec3f){1., 1., 1.};
    v1 = v1 + (Vec3f){1., 1., 1.};
    v2 = v2 + (Vec3f){1., 1., 1.};
    ctx->triangle(
        {static_cast<int>(v0.x * w / 2.), static_cast<int>(v0.y * h / 2.)},
        {static_cast<int>(v1.x * w / 2.), static_cast<int>(v1.y * h / 2.)},
        {static_cast<int>(v2.x * w / 2.), static_cast<int>(v2.y * h / 2.)});
  }

  ctx->flipV();
  canvas->save("./output.tga");
  return 0;
}