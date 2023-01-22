#pragma once
#include "tgaimage.h"

struct Point {
 public:
  int x;
  int y;
  Point(int x, int y) {
    this->x = x;
    this->y = y;
  }
  Point(double x, double y) {
    this->x = (int)x;
    this->y = (int)y;
  }
};

class CanvasContext;
class Canvas {
 private:
  TGAImage* image;
  friend class CanvasContext;

 public:
  const static TGAColor* COLOR_RED;
  const static TGAColor* COLOR_WHITE;
  const static TGAColor* COLOR_BLACK;
  const static TGAColor* COLOR_GREEN;
  const static TGAColor* COLOR_BLUE;
  const static TGAColor* COLOR_YELLOW;
  const static TGAColor* COLOR_PURPLE;
  const static TGAColor* COLOR_ORANGE;
  const static TGAColor* COLOR_CYAN;
  const static TGAColor* COLOR_GRAY;

  Canvas(int width, int height);
  Canvas& save(const char* path);
  CanvasContext* createContext();
};

class CanvasContext {
 private:
  const TGAColor* color;
  Canvas* canvas;
  // 画线算法
  // 我的实现
  void myline(const Vec2i& start, const Vec2i& end);
  // 基于误差，但使用了浮点数
  void stepToBresenham(const Vec2i& start, const Vec2i& end);
  // 这里有俩表达式
  // 1. y = y0 + t * (y1 - y0) 这个比较容易理解，但是 wiki
  // 上 step2 之后使用的表达式都是下面这个。

  // 2. y = y0 * (1. - t) + y1 * t 这个存在更大的舍入误差，
  // 在斜率无穷(垂直)的时候，渲染结果会在两个像素之间抖动
  void notwork(const Vec2i& start, const Vec2i& end);
  // bresenham 实现
  void bresenham(const Vec2i& start, const Vec2i& end);

  // 三角形填充
  void mytriangle(const Vec2i& a, const Vec2i& b, const Vec2i& c);

 public:
  CanvasContext(Canvas* canvas);
  CanvasContext& setColor(const TGAColor* color);

  CanvasContext& point(const Vec2i& point);
  CanvasContext& line(const Vec2i& start, const Vec2i& end);
  CanvasContext& triangle(const Vec2i& a, const Vec2i& b, const Vec2i& c);
  CanvasContext& flipH();
  CanvasContext& flipV();
};

const TGAColor* Canvas::COLOR_RED = new TGAColor(255, 0, 0, 255);
const TGAColor* Canvas::COLOR_WHITE = new TGAColor(255, 255, 255, 255);
const TGAColor* Canvas::COLOR_BLACK = new TGAColor(0, 0, 0, 255);
const TGAColor* Canvas::COLOR_GREEN = new TGAColor(0, 255, 0, 255);
const TGAColor* Canvas::COLOR_BLUE = new TGAColor(0, 0, 255, 255);
const TGAColor* Canvas::COLOR_YELLOW = new TGAColor(255, 255, 0, 255);
const TGAColor* Canvas::COLOR_PURPLE = new TGAColor(255, 0, 255, 255);
const TGAColor* Canvas::COLOR_ORANGE = new TGAColor(255, 165, 0, 255);
const TGAColor* Canvas::COLOR_CYAN = new TGAColor(0, 255, 255, 255);
const TGAColor* Canvas::COLOR_GRAY = new TGAColor(128, 128, 128, 255);

Canvas::Canvas(int width, int height) {
  this->image = new TGAImage(width, height, TGAImage::RGB);
}

CanvasContext* Canvas::createContext() {
  return new CanvasContext(this);
}
Canvas& Canvas::save(const char* path) {
  this->image->write_tga_file(path);
  return *this;
}

void CanvasContext::myline(const Vec2i& start, const Vec2i& end) {
  if (start.x > end.x) {
    this->myline(end, start);
    return;
  }
  int x0 = start.x, x1 = end.x, y0 = start.y, y1 = end.y;
  for (int x = x0; x <= x1; x++) {
    int t = (x - x0) / (x1 - x0);
    int y = y0 + (y1 - y0) * t;
    int nexty = (end.x - start.x) == 0
                    ? end.y
                    : start.y + (end.y - start.y) * (x + 1 - start.x) /
                                    (end.x - start.x);
    this->point(Vec2i(x, y));
    if (y < end.y) {
      while (y < nexty) {
        this->point(Vec2i(x, y));
        y++;
      }
    } else if (y > end.y) {
      while (y > nexty) {
        this->point(Vec2i(x, y));
        y--;
      }
    }
  }
}

void CanvasContext::stepToBresenham(const Vec2i& start, const Vec2i& end) {
  int x0 = start.x, x1 = end.x, y0 = start.y, y1 = end.y;
  bool steep = false;
  if (std::abs(x1 - x0) < std::abs(y1 - y0)) {
    steep = true;
    std::swap(x0, y0);
    std::swap(x1, y1);
  }

  if (x1 < x0) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }
  float derror = std::abs(y1 - y0) / (float)std::abs(x1 - x0);
  float error = 0;
  int y = y0;

  for (int x = x0; x <= x1; x++) {
    if (!steep)
      this->point(Vec2i(x, y));
    else
      this->point(Vec2i(y, x));
    error += derror;
    if (error > .5) {
      y += (y1 > y0) ? 1 : -1;
      error -= 1;
    }
  }
}

void CanvasContext::notwork(const Vec2i& start, const Vec2i& end) {
  int x0 = start.x, x1 = end.x, y0 = start.y, y1 = end.y;
  bool steep = false;
  if (std::abs(x1 - x0) < std::abs(y1 - y0)) {
    steep = true;
    std::swap(x0, y0);
    std::swap(x1, y1);
  }
  if (x1 < x0) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  for (int x = x0; x <= x1; ++x) {
    float t = (x - x0) / (float)(x1 - x0);
    int y = y0 * (1. - t) + y1 * t;  // what is wrong here and how it works?
    // int y = y0 + t * (y1 - y0);  // this works fine

    if (steep) {
      this->point(Vec2i(y, x));
    } else {
      this->point(Vec2i(x, y));
    }
  }
}

void CanvasContext::bresenham(const Vec2i& start, const Vec2i& end) {
  int x0 = start.x, x1 = end.x, y0 = start.y, y1 = end.y;
  bool steep = false;
  if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
    steep = true;
    std::swap(x0, y0);
    std::swap(x1, y1);
  }

  if (x1 < x0) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }
  int dx = x1 - x0;
  int derror = std::abs(y1 - y0) * 2;
  int error = 0;
  int y = y0;

  for (int x = x0; x <= x1; x++) {
    if (steep) {
      this->point(Vec2i(y, x));
    } else {
      this->point(Vec2i(x, y));
    }
    error += derror;
    if (error > dx) {
      y += (y1 > y0) ? 1 : -1;
      error -= dx * 2;
    }
  }
}

void CanvasContext::mytriangle(const Vec2i& a, const Vec2i& b, const Vec2i& c) {
  Vec2i* vertex[3] = {const_cast<Vec2i*>(&a), const_cast<Vec2i*>(&b),
                      const_cast<Vec2i*>(&c)};
  std::sort(vertex, vertex + 3, [](Vec2i* a, Vec2i* b) { return a->y < b->y; });
  int h1 = vertex[1]->y - vertex[0]->y;
  int h2 = vertex[2]->y - vertex[0]->y;
  int h3 = vertex[2]->y - vertex[1]->y;
  for (int y = vertex[0]->y;
       y < /* == vertex[1] 时会在一端得到 x 坐标 0, 下同 */ vertex[1]->y; ++y) {
    int h = y - vertex[0]->y;
    int startx = vertex[0]->x + (vertex[2]->x - vertex[0]->x) * (h / (float)h2);
    int endx = vertex[0]->x + (vertex[1]->x - vertex[0]->x) * (h / (float)h1);
    if (startx > endx) {
      std::swap(startx, endx);
    }
    for (int x = startx; x <= endx; ++x) {
      this->point({x, y});
    }
  }

  for (int y = vertex[1]->y; y < vertex[2]->y; ++y) {
    int startx = vertex[0]->x + (vertex[2]->x - vertex[0]->x) *
                                    ((float)(y - vertex[0]->y) / h2);
    int endx = vertex[1]->x +
               (vertex[2]->x - vertex[1]->x) * ((float)(y - vertex[1]->y) / h3);
    if (startx > endx) {
      std::swap(startx, endx);
    }
    for (int x = startx; x <= endx; ++x) {
      this->point({x, y});
    }
  }
}

CanvasContext::CanvasContext(Canvas* canvas) {
  this->color = new TGAColor(255, 255, 255, 255);
  this->canvas = canvas;
}

CanvasContext& CanvasContext::setColor(const TGAColor* color) {
  this->color = color;
  return *this;
}

CanvasContext& CanvasContext::point(const Vec2i& point) {
  this->canvas->image->set(point.x, point.y, *this->color);
  return *this;
}

CanvasContext& CanvasContext::line(const Vec2i& start, const Vec2i& end) {
  bresenham(start, end);
  return *this;
}

CanvasContext& CanvasContext::triangle(const Vec2i& a,
                                       const Vec2i& b,
                                       const Vec2i& c) {
  this->mytriangle(a, b, c);
  // this->line(a, b).line(b, c).line(c, a);
  return *this;
}

CanvasContext& CanvasContext::flipH() {
  this->canvas->image->flip_horizontally();
  return *this;
}

CanvasContext& CanvasContext::flipV() {
  this->canvas->image->flip_vertically();
  return *this;
}
