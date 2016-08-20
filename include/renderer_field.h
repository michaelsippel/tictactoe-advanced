#pragma once

class CellStatus;
template <int N, typename elemType>
class Field;

#include <field.h>

template <>
class Renderer<CellStatus>
{
    public:
        Renderer(CellStatus& obj)
            :object(obj) {}

        void draw(void)
        {
            this->draw(this->object);
        }

        static void draw(CellStatus& status)
        {
            Renderer<enum cube> cube = Renderer<enum cube>();
            switch(status.value)
            {
                case 0:
                    glColor4f(0.8f, 0.8f, 0.8f, 0.1f);
                    cube.draw(LINED);
                    break;

                case 1:
                    glColor4f(0.8f, 0.0f, 0.0f, 1.0f);
                    cube.draw(SOLID);
                    break;

                case 2:
                    glColor4f(0.0f, 0.0f, 0.8f, 1.0f);
                    cube.draw(SOLID);
                    break;
            }
        }

    private:
        CellStatus& object;
};

template <typename elemType>
class Renderer<Field<3, elemType>>
{
    public:
        Renderer(Field<3, elemType>& obj)
            :object(obj) {}

        void draw(void)
        {
            this->draw(this->object);
        }

        static void draw(Field<3, elemType>& field)
        {
            static Renderer<enum cube> cube = Renderer<enum cube>();

            glPushMatrix();

            glScalef(0.98f, 0.98f, 0.98f);
            glScalef(1.0f/(float)field.getSize().x(), 1.0f/(float)field.getSize().y(), 1.0f/(float)field.getSize().z());

            for(int x = 0; x < field.getSize().x(); x++)
            {
                for(int y = 0; y < field.getSize().y(); y++)
                {
                    for(int z = 0; z < field.getSize().z(); z++)
                    {
                        glPushMatrix();
                        glTranslatef((float)x - (float)(field.getSize().x()-1)/2.0f,
                                     (float)y - (float)(field.getSize().y()-1)/2.0f,
                                     (float)z - (float)(field.getSize().z()-1)/2.0f);

                        auto s = field.getCell(Vector3i(x,y,z));
                        Renderer<elemType>::draw(*s);

                        glPopMatrix();
                    }
                }
            }

            glPopMatrix();

            glColor4f(0.8f, 0.8f, 0.8f, 0.5f);
            cube.draw(LINED);
        }

    private:
        Field<3, elemType>& object;
};
