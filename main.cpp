//Mildred and Simone
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

int main()
{
    //width
    const int width = 800;
    //height
    const int height = 600;

    //creating window
    RenderWindow window(VideoMode(width, height), "Mandelbrot Set");
    //creating complex plane
    ComplexPlane plane(width, height);

    //loading font for displaying text
    Font font;
    if (!font.loadFromFile("Arial.ttf")) //checking if font loads
    {
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

    //text for mandelbrot information
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
            if (event.type == Event::Closed) //closing the window
                window.close();

            if (event.type == Event::MouseButtonPressed) //mouse clocks
            {
                if (event.mouseButton.button == Mouse::Left) //zoom in
                    plane.zoomIn();
                else if (event.mouseButton.button == Mouse::Right)//zoom out
                    plane.zoomOut();

                plane.setCenter(Mouse::getPosition(window)); //recenter
            }
        }

        plane.setMouseLocation(Mouse::getPosition(window)); //updating mouse position

        if (plane.getState() == ComplexPlane::State::CALCULATING)
        {
            plane.updateRender();
        }

        //updating mandelbrot info
        plane.loadText(infoText);

        window.clear(); //clear window
        window.draw(plane); //drawing mandelbrot set
        window.draw(infoText); //drawing text display
        window.display();
    }

    return 0;
}
