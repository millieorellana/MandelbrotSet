#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

int main()
{
    const int width = 800;
    const int height = 600;

    // Create window
    RenderWindow window(VideoMode(width, height), "Mandelbrot Set");
    ComplexPlane plane(width, height);

    // Load font
    Font font;
    if (!font.loadFromFile("Arial.ttf")) // Ensure this font file exists in your working directory
    {
        // Display error message on the window if font loading fails
        RenderWindow errorWindow(VideoMode(400, 100), "Error");
        Text errorText;
        errorText.setFont(font);
        errorText.setCharacterSize(20);
        errorText.setFillColor(Color::Red);
        errorText.setPosition(10, 30);
        errorText.setString("Failed to load font file!");

        while (errorWindow.isOpen())
        {
            Event event;
            while (errorWindow.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    errorWindow.close();
            }

            errorWindow.clear(Color::Black);
            errorWindow.draw(errorText);
            errorWindow.display();
        }

        return -1;
    }

    // Create text object for Mandelbrot information
    Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(16);
    infoText.setFillColor(Color::White);
    infoText.setPosition(10, 10);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                    plane.zoomIn();
                else if (event.mouseButton.button == Mouse::Right)
                    plane.zoomOut();

                plane.setCenter(Mouse::getPosition(window));
            }
        }

        plane.setMouseLocation(Mouse::getPosition(window));

        if (plane.getState() == ComplexPlane::State::CALCULATING)
        {
            plane.updateRender();
        }

        // Update the text with Mandelbrot information
        plane.loadText(infoText);

        window.clear();
        window.draw(plane);
        window.draw(infoText); // Ensure the text is drawn on top of the plane
        window.display();
    }

    return 0;
}
