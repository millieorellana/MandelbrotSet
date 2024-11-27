#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include "ComplexPlane.h"

using namespace std;
using namespace sf;

int main(){
    int screenWidth = VideoMode::getDesktopMode().width / 2;
    int screenHeight = VideoMode::getDesktopMode().height / 2;

    RenderWindow window(VideoMode(screenWidth, screenHeight), "Mandelbrot Game!", Style::Default);
    ComplexPlane complexPlane(screenWidth, screenHeight);

    Font font;
    if (!font.loadFromFile("Arial.ttf"))  // Replace with the correct path if necessary
    {
        cout << "Error loading font" << endl;
        return -1;
    }

    // Set up text to display instructions
    Text instructions("Hello", font, 30);
    instructions.setPosition(10, 10);
    instructions.setFillColor(Color::White);

    Text prompt("what up", font, 30);
    prompt.setPosition(10, 50);
    prompt.setFillColor(Color::White);

    while (window.isOpen()) {
        // Handle Input segment
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                // Quit the game when the window is closed
                window.close();
            }
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Right)
                {
                    complexPlane.zoomOut();
                }
                else if (event.mouseButton.button == Mouse::Left) {
                    complexPlane.zoomIn();

                }
            }
        }
    }

}