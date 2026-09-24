#include<raylib.h>

#include<iostream>
#include<vector>
#include<format>
#include<random>

/*Fläche Kreis / Fläche Quadrat
PI * r² / 2 x 2    = prozentualer Anteil im Quadrat
pi/4 | * 4 ist wieder pi*/

struct RandomPointGenerator {
public:
  [[nodiscard]] float getRandom01() {
    static std::mt19937 generator { std::random_device()()};
    return std::uniform_real_distribution<float>(-1.0f, 1.0f)(generator); 
}

  [[nodiscard]]Vector2 getRandomPoint() {
    Vector2 point{};
    point.x = getRandom01();
    point.y = getRandom01();
    return point;
  }
};

struct Point {
  Vector2 coordinates_;
  bool isInside_;
};

using Points = std::vector<Point>;
class PiEstimation {
public:
  explicit PiEstimation(RandomPointGenerator&& random) : random_(std::move(random)) {}
  void update() {
    auto point{ (random_.getRandomPoint())};
    bool isInside{point.x * point.x + point.y * point.y < 1}; // r = 1;  x² + y² < r² Pythagoras; ist Punkt noch innerhalb Kreis r = 1?
    points_.emplace_back(point, isInside);
    inside_ += isInside;
  }

  void draw() {
    float radius { GetScreenWidth() / 4.0f};
    Vector2 centerPos{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    for(const auto& point : points_) {
      DrawCircle(centerPos.x + point.coordinates_.x * radius, centerPos.y + point.coordinates_.y * radius, //Scaling
      2,
      point.isInside_ ? RED : BLUE);
    }
    size_t total{ points_.size()};
    float piEstimate{ (static_cast<float>(inside_) / total) * 4}; //aktuelle Anzahl an Punkte umgerechnet in PI
    std::string displayText{ std::format("pi estimate {}", piEstimate)};
    DrawText(displayText.data(), 10, 10, 60, LIME);
  }
private:
  Points points_{};
  size_t inside_{};
  RandomPointGenerator random_;
};

int main() {
  PiEstimation estimation(RandomPointGenerator{});
  InitWindow(1000, 1000, "Monte Carlo PI Estimation");
  SetTargetFPS(500);

  while(!WindowShouldClose()) { 
    PollInputEvents();
    for(auto i{ 0uz}; i < 500; i++) {
      estimation.update();
    }

    BeginDrawing();
    ClearBackground(BLACK);
    estimation.draw();
    EndDrawing();
}
  return 0;
}