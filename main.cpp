#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

int main()
{
    const int width = 800;
    const int height = 600;

    RenderWindow window(VideoMode(width, height), "Mandelbrot Set");
    ComplexPlane plane(width, height);

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

        window.clear();
        window.draw(plane);
        window.display();
    }

    return 0;
}
