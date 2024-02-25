#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Graphics.hpp>

struct Shape {
  std::string name;
  int posX, posY;
  int velX, velY;
  float color[3];

  Shape(const std::string &name, int posX, int posY, int velX, int velY,
        float color[3])
      : name(name), posX(posX), posY(posY), velX(velX), velY(velY) {
    this->color[0] = color[0];
    this->color[1] = color[1];
    this->color[2] = color[2];
  }
};

struct Circle : public Shape {
  int radius;
  sf::CircleShape circle;

  Circle(const std::string &name, int posX, int posY, int velX, int velY,
         float color[3], int radius)
      : Shape(name, posX, posY, velX, velY, color), radius(radius) {
    circle = sf::CircleShape(radius, 20);
  }
};

struct Rectangle : public Shape {
  int width, height;
  sf::RectangleShape rect;

  Rectangle(const std::string &name, int posX, int posY, int velX, int velY,
            float color[3], int width, int height)
      : Shape(name, posX, posY, velX, velY, color), width(width),
        height(height) {
    rect = sf::RectangleShape(sf::Vector2f(width, height));
  }
};

std::vector<Circle> circle_shapes;
std::vector<Rectangle> rect_shapes;

void loadFromFile(const std::string &filename) {
  std::ifstream fin(filename);

  if (!fin.is_open()) {
    std::cout << "Error opening file: " << filename << std::endl;
    exit(-1);
  }

  std::string lineID;
  int wWidth, wHeight;
  fin >> lineID >> wWidth >> wHeight;
  std::cout << "Window width: " << wWidth << " Window height: " << wHeight
            << std::endl;

  std::string fontPath;
  int fontSize;
  int fontColorR, fontColorG, fontColorB;
  fin >> lineID >> fontPath >> fontSize >> fontColorR >> fontColorG >>
      fontColorB;
  std::cout << "Font path: " << fontPath << " Font size: " << fontSize
            << " Font color: " << fontColorR << " " << fontColorG << " "
            << fontColorB << std::endl;

  std::string name;
  int posX, posY;
  int velX, velY;
  int colorR, colorG, colorB;
  int radius;
  int width, height;

  while (fin >> lineID) {
    fin >> name >> posX >> posY >> velX >> velY >> colorR >> colorG >> colorB;
    std::cout << lineID << " " << name << " " << posX << " " << posY << " "
              << velX << " " << velY << " " << colorR << " " << colorG << " "
              << colorB << " ";

    float color[3] = {colorR / 256.0f, colorG / 256.0f, colorB / 256.0f};

    if (lineID == "Circle") {
      fin >> radius;
      std::cout << radius << std::endl;
      circle_shapes.push_back(
          Circle({name, posX, posY, velX, velY, color, radius}));
    }

    else if (lineID == "Rectangle") {
      fin >> width >> height;
      std::cout << width << " " << height << std::endl;
      rect_shapes.push_back(
          Rectangle({name, posX, posY, velX, velY, color, width, height}));
    }

    else {
      std::cout << "Invalid shape" << std::endl;
      exit(-1);
    }
  }
}

int main(int argc, char *argv[]) {
  loadFromFile("input.txt");

  const int wWidth = 800;
  const int wHeight = 600;
  sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
  window.setFramerateLimit(60);

  ImGui::SFML::Init(window);
  sf::Clock deltaClock;

  ImGui::GetStyle().ScaleAllSizes(1.0f);

  sf::Font myFont;
  if (!myFont.loadFromFile("bin/fonts/font.ttf")) {
    std::cout << "Error loading font\n";
    exit(-1);
  }

  sf::Text text("Hello World", myFont, 30);
  text.setPosition(0, wHeight - (float)text.getCharacterSize());

  char displayString[255] = "Sample Text";

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (event.type == sf::Event::Closed)
        window.close();

      // if (event.type == sf::Event::KeyPressed) {
      // std::cout << "Key pressed: " << event.key.code << std::endl;
      // if (event.key.code == sf::Keyboard::X) {
      // }
      // }
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::Begin("Settings");
    ImGui::InputText("Display String", displayString, 255);
    if (ImGui::Button("Set Text"))
      text.setString(displayString);

    for (auto &shape : circle_shapes) {
      if (ImGui::TreeNode(shape.name.c_str())) {
        ImGui::SliderInt("Circle Radius", &shape.radius, 0, 100);
        ImGui::ColorEdit3("Color Circle", shape.color);
        ImGui::TreePop();
      }
    }

    for (auto &shape : rect_shapes) {
      if (ImGui::TreeNode(shape.name.c_str())) {
        ImGui::SliderInt("Width", &shape.width, 0, 100);
        ImGui::SliderInt("Height", &shape.height, 0, 100);
        ImGui::ColorEdit3("Color Rect", shape.color);
        ImGui::TreePop();
      }
    }

    ImGui::End();

    window.clear();

    for (auto &shape : circle_shapes) {
      if (shape.posX < 0 || shape.posX > wWidth - 2 * shape.radius)
        shape.velX *= -1;

      if (shape.posY < 0 || shape.posY > wHeight - 2 * shape.radius)
        shape.velY *= -1;

      shape.posX += shape.velX;
      shape.posY += shape.velY;
      shape.circle.setPosition(shape.posX, shape.posY);
      shape.circle.setRadius(shape.radius);
      shape.circle.setFillColor(sf::Color(
          shape.color[0] * 255, shape.color[1] * 255, shape.color[2] * 255));
      window.draw(shape.circle);
    }

    for (auto &shape : rect_shapes) {
      if (shape.posX < 0 || shape.posX > wWidth - shape.width)
        shape.velX *= -1;

      if (shape.posY < 0 || shape.posY > wHeight - shape.height)
        shape.velY *= -1;

      shape.posX += shape.velX;
      shape.posY += shape.velY;
      shape.rect.setPosition(shape.posX, shape.posY);
      shape.rect.setSize(sf::Vector2f(shape.width, shape.height));
      shape.rect.setFillColor(sf::Color(
          shape.color[0] * 255, shape.color[1] * 255, shape.color[2] * 255));
      window.draw(shape.rect);
    }

    window.draw(text);
    ImGui::SFML::Render(window);
    window.display();
  }
  return 0;
}