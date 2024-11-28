#include "ComplexPlane.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

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
            for (int j = 0; j < m_pixel_size.x; j++)
            {
                m_vArray[j + i * m_pixel_size.x].position = {static_cast<float>(j), static_cast<float>(i)};
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
    m_zoomCount++;
    m_plane_size = {BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount), BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount)};
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    m_plane_size = {BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount), BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount)};
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
    std::stringstream ss;
    ss << "Mandelbrot Set\n"
       << "Center: (" << std::fixed << std::setprecision(5) << m_plane_center.x << ", " << m_plane_center.y << ")\n"
       << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n"
       << "Left-click to Zoom in\n"
       << "Right-click to Zoom out";
    text.setString(ss.str());
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
    size_t iterations = 0;
    float x = 0.0f, y = 0.0f, cx = coord.x, cy = coord.y;

    while (iterations < MAX_ITER)
    {
        float xTemp = x * x - y * y + cx;
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
        r = g = b = 0; //black
        return;
    }

    float normalized = static_cast<float>(count) / MAX_ITER;

    if (normalized < 0.2f)
    {
        r = 128 + 127 * (normalized / 0.2f);
        g = 0;
        b = 255 - 128 * (normalized / 0.2f);
    }
    else if (normalized < 0.5f)
    {
        normalized = (normalized - 0.2f) / 0.3f;
        r = 0;
        g = 255 * normalized;
        b = 255 - 255 * normalized;
    }
    else if (normalized < 0.8f)
    {
        normalized = (normalized - 0.5f) / 0.3f;
        r = 255 * normalized;
        g = 255;
        b = 0;
    }
    else
    {
        normalized = (normalized - 0.8f) / 0.2f;
        r = 255;
        g = 255 - 255 * normalized; 
        b = 0;
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    return {
        (static_cast<float>(mousePixel.x) / m_pixel_size.x) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0f),
        (static_cast<float>(mousePixel.y) / m_pixel_size.y) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0f)
    };
}
