#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "ComplexPlane.h"

using namespace std;
using namespace sf;


ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixel_size = {pixelWidth, pixelHeight};
    m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
    m_plane_center = {0.0f, 0.0f};
    m_plane_size = {BASE_WIDTH, BASE_HEIGHT * m_aspectRatio};
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    m_vArray = VertexArray(Points, pixelWidth * pixelHeight);


}
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
    if (m_state == State::CALCULATING)
    {
        for (int i = 0; i < m_pixel_size.y; i++)
        {
            for (int j = 0; j < m_pixel_size.x; i++)
            {
                m_vArray[j + i * m_pixel_size.x].position = { (float)j, (float)i };
                Vector2f complexCoord = mapPixelToCoords({j, i});
                int iterations = countIterations(complexCoord);

                Uint8 r, g, b;

                iterationsToRGB(iterations, r, g, b);
                m_vArray[j + i * m_pixel_size.x].color = {r, g, b};
            }
        }
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn()
{
    float x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_zoomCount++;
    m_plane_size = {x_size, y_size};
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
    float x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_zoomCount--;
    m_plane_size = {x_size, y_size};
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel){

    Vector2f newCenter = mapPixelToCoords(mousePixel);
    m_plane_center = newCenter;
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
    stringstream ss;
    ss << "Mandelbrot Set\n";

    ss << "Center: ("
       << fixed << setprecision(5)
       << m_plane_center.x << ", "
       << m_plane_center.y << ")\n";

    ss << "Cursor: ("
       << fixed << setprecision(5)
       << m_mouseLocation.x << ", "
       << m_mouseLocation.y << ")\n";

    ss << "Left-click to Zoom in\n";

    ss << "Right-click to Zoom out";

    text.setString(ss.str());
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
    size_t iterations = 0;
    const size_t maxIterations = MAX_ITER;

    float x = 0.0f, y = 0.0f;
    float cx = coord.x, cy = coord.y;

    while(iterations < maxIterations)
    {
        float xTemp = (x * x) - (y * y) + cx;
        y = 2 * x * y + cy;
        x = xTemp;

        if (x * x + y * y > 4.0f)
        {
            break;
        }

        iterations++;
    }
    return iterations;
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count >= MAX_ITER)
    {
        r = g = b = 0;
        return;
    }

    float normalized = /*static_cast<float> */ count / MAX_ITER;

    if (normalized < 0.2f)
    {
        //purple to blue
        r = (128 + 127 * normalized / 0.2f); // slide red from 128 to 255
        g = 0;
        b = (255 - 128 * normalized / 0.2f); //blue from 255 - 127
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    float real = (static_cast<float>(mousePixel.x) / static_cast<float>(m_pixel_size.x)) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0f);
    float imaginary = (static_cast<float>(mousePixel.y) / static_cast<float>(m_pixel_size.y)) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0f);
    
    return Vector2f(real, imaginary);
}

