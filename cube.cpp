#include <cmath>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

struct Vec3
{
    float x;
    float y;
    float z;
};

Vec3 rotateY(Vec3 point, float angle)
{
    float radians = angle * 3.14159265f / 180.0f;

    float c = std::cos(radians);
    float s = std::sin(radians);

    Vec3 result;

    result.x = point.x * c + point.z * s;
    result.y = point.y;
    result.z = -point.x * s + point.z * c;

    return result;
}

void project(
    Vec3 point,
    float& screenX,
    float& screenY
)
{
    float z = point.z + 5.0f;

    if (z < 0.1f)
        z = 0.1f;

    float scale = 500.0f / z;

    screenX = 500.0f + point.x * scale;
    screenY = 400.0f - point.y * scale;
}


// Store the projected points here.
static float projected[16];


extern "C"
{

    EMSCRIPTEN_KEEPALIVE
    void project_cube(float angle)
    {
        Vec3 points[8] =
        {
            { -1.f, -1.f, -1.f },
            {  1.f, -1.f, -1.f },
            {  1.f,  1.f, -1.f },
            { -1.f,  1.f, -1.f },

            { -1.f, -1.f,  1.f },
            {  1.f, -1.f,  1.f },
            {  1.f,  1.f,  1.f },
            { -1.f,  1.f,  1.f }
        };

        for (int i = 0; i < 8; i++)
        {
            Vec3 rotated = rotateY(points[i], angle);

            project(
                rotated,
                projected[i * 2],
                projected[i * 2 + 1]
            );
        }
    }


    EMSCRIPTEN_KEEPALIVE
    float get_point_x(int index)
    {
        return projected[index * 2];
    }


    EMSCRIPTEN_KEEPALIVE
    float get_point_y(int index)
    {
        return projected[index * 2 + 1];
    }

}