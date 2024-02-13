#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

int main(int argc, char *argv[])
{
    const int wWidth = 800;
    const int wHeight = 600;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    ImGui::GetStyle().ScaleAllSizes(1.0f);

    float c[3] = {0.f, 1.f, 1.f};

    // example circle shape
    float circleRadius = 50.f;
    int circleSegments = 32;
    float circleSpeedX = 1.0f;
    float circleSpeedY = 0.5f;
    bool drawCircle = true;
    bool drawText = true;

    sf::CircleShape circle(circleRadius, circleSegments);
    circle.setPosition(wWidth / 2, wHeight / 2);

    sf::Font myFont;
    if (!myFont.loadFromFile("bin/fonts/font.ttf"))
    {
        std::cout << "Error loading font\n";
        exit(-1);
    }

    sf::Text text("Hello World", myFont, 30);
    text.setPosition(0, wHeight - (float)text.getCharacterSize());

    char displayString[255] = "Sample Text";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed: " << event.key.code << std::endl;

                if (event.key.code == sf::Keyboard::X)
                {
                    circleSpeedX *= -1.0f;
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Sample window");
        ImGui::Text("Hello World");
        ImGui::Checkbox("Draw Circle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Text", &drawText);
        ImGui::SliderFloat("Circle Radius", &circleRadius, 0.0f, 100.0f);
        ImGui::SliderInt("Circle Segments", &circleSegments, 3, 64);
        ImGui::ColorEdit3("Color Circle", c);
        ImGui::InputText("Display String", displayString, 255);

        if (ImGui::Button("Set Text"))
            text.setString(displayString);

        ImGui::SameLine();

        if (ImGui::Button("Reset Circle"))
            circle.setPosition(0, 0);

        ImGui::End();

        circle.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255));
        circle.setPointCount(circleSegments);
        circle.setRadius(circleRadius);

        circle.setPosition(circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY);

        window.clear();
        if (drawCircle)
            window.draw(circle);

        if (drawText)
            window.draw(text);

        ImGui::SFML::Render(window);

        window.display();
    }
    return 0;
}