#include <GL/glew.h>
#include <GL/gl.h>
#include <viewer.h>

Viewer::Viewer()
{
    this->rotation = Vector3f(0.0f, 0.0f, 0.0f);
}

Viewer::~Viewer()
{
}

void Viewer::rotateX(float v)
{
    this->rotation.data[0] += v;
}

void Viewer::rotateY(float v)
{
    this->rotation.data[1] += v;
}

void Viewer::zoom(float v)
{
    this->rotation.data[2] -= v;
}

void Viewer::clear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Viewer::loadMatrix(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, this->rotation.z());

    glRotatef(this->rotation.x(), 1.0f, 0.0f, 0.0f);
    glRotatef(this->rotation.y(), 0.0f, 1.0f, 0.0f);
}
