#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <math/vector.h>

#include <field.h>

class Viewer
{
    public:
        Viewer(Field3D* field_);
        ~Viewer();

        void rotateX(float v);
        void rotateY(float v);
        void zoom(float v);

        void clear(void);
        void draw(void);
        void drawLinedCube(void);
        void drawSolidCube(void);

        template <int N, typename elemType>
        void drawField(Field<N, elemType>* field)
        {
            glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
            this->drawLinedCube();

            glPushMatrix();
            glScalef(0.8f, 0.8f, 0.8f);
            glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
            this->drawSolidCube();
            glPopMatrix();
        }

    private:
        Vector3f rotation;
        Field3D* field;

        GLuint vbuf_id;
        GLuint libuf_id;
        GLuint fibuf_id;
};
