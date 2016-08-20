#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

template <typename T>
class Renderer
{
    public:
        Renderer(T& object_) :object(object_) {};
        ~Renderer() {}

        static void draw(T& obj, bool selected) {}
        void draw(void)
        {
            this->draw(this->object);
        }

    private:
        T& object;
};

#include "renderer_cube.h"
#include "renderer_field.h"
