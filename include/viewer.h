#pragma once

#include <GL/glew.h>
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
        void loadMatrix(void);

    private:
        Vector3f rotation;
};
