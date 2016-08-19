#pragma once

#include <math/vector.h>

#include <field.h>

class Viewer
{
    public:
        Viewer();
        ~Viewer();

        void rotateX(float v);
        void rotateY(float v);
        void zoom(float v);

        void clear(void);
        void draw(void);

    private:
        Vector3f rotation;
};
