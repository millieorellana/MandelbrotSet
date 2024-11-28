#ifndef COMPLEX_PLANE_H
#define COMPLEX_PLANE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;

class ComplexPlane : public Drawable {
public:
    //setting up plane
    ComplexPlane(int pixelWidth, int pixelHeight);

    void updateRender();
    void zoomIn();
    void zoomOut();
    void setCenter(Vector2i mousePixel);
    void setMouseLocation(Vector2i mousePixel);
    void loadText(Text& text);

    enum class State { CALCULATING, DISPLAYING };
    State getState() const { return m_state; }

private:
    virtual void draw(RenderTarget& target, RenderStates states) const override;
    size_t countIterations(Vector2f coord);
    void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
    Vector2f mapPixelToCoords(Vector2i mousePixel);

    // plane and pixel variables
    Vector2i m_pixel_size;
    Vector2f m_plane_center;
    Vector2f m_plane_size;
    float m_aspectRatio;

    //mouse position
    Vector2f m_mouseLocation;

    //zoom settings
    int m_zoomCount;
    const float BASE_ZOOM = 0.5f;
    const float BASE_WIDTH = 4.0f;
    const float BASE_HEIGHT = 4.0f;

    //rendering Mandelbrot
    VertexArray m_vArray;
    State m_state;

    //max iterations
    const size_t MAX_ITER = 1000;
};

#endif
