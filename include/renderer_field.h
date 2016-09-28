#pragma once

class CellStatus;
template <int N, typename elemType>
class Field;

#include <field.h>
#include <color.h>

#define SELECT_COLOR(x) \
                    if(selected) \
                    { \
                        glScalef(1.1f, 1.1f, 1.1f); \
                        glColor4f(selectColor.r(), selectColor.g(), selectColor.b(), selectColor.a()); \
                    } \
                    else \
                    { \
                        glColor4f(0.8f, 0.8f, 0.8f, x); \
                    } \

template <>
class Renderer<CellStatus>
{
    public:
        Renderer(CellStatus& obj)
            :status(obj), selected(false) {}

        void select(Color4f col)
        {
            selectColor = col;
            this->selected = true;
        }

        void deselect(void)
        {
            this->selected = false;
        }

        void draw(void)
        {
            Renderer<enum cube> cube = Renderer<enum cube>();
            switch(status.value)
            {
                case 0:
                    SELECT_COLOR(0.05);
                    cube.draw(LINED);
                    break;

                case 1:
                    glScalef(0.6f, 0.6f, 0.6f);
                    glColor4f(0.8f, 0.0f, 0.0f, 1.0f);
                    cube.draw(SOLID);
                    break;

                case 2:
                    glScalef(0.6f, 0.6f, 0.6f);
                    glColor4f(0.0f, 0.0f, 0.8f, 1.0f);
                    cube.draw(SOLID);
                    break;
            }
        }

    private:
        Color4f selectColor;
        bool selected;
        CellStatus& status;
};

template <typename elemType>
class Renderer<Field<3, elemType>>
{
    public:
        Renderer(Field<3, elemType>& obj)
            :field(obj), selected(false), selectionRenderer(NULL) {}

        void select(Color4f col)
        {
            selectColor = col;
            selected = true;
        }

        Renderer<elemType>* select(Vector3i pos, Color4f col)
        {
            deselect();
            select(selectColor);
            selection = pos;
            selectionRenderer = new Renderer<elemType>(*field.getCell(selection));
            selectionRenderer->select(col);
            return selectionRenderer;
        }

        void deselect(void)
        {
            if(selected)
            {
                selected = false;
                if(selectionRenderer != NULL)
                    delete selectionRenderer;
                selectionRenderer = NULL;
            }
        }

        void draw(void)
        {
            static Renderer<enum cube> cube = Renderer<enum cube>();

            glPushMatrix();

            //glScalef(0.98f, 0.98f, 0.98f);
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

                        if(selectionRenderer != NULL && selection.x() == x && selection.y() == y && selection.z() == z)
                            selectionRenderer->draw();
                        else
                        {
                            auto s = field.getCell(Vector3i(x,y,z));
                            auto r = Renderer<elemType>(*s);
                            r.draw();
                        }

                        glPopMatrix();
                    }
                }
            }
            glPopMatrix();

            glPushMatrix();
            glScalef(1.01f, 1.01f, 1.01f);
            SELECT_COLOR(0.5);
            cube.draw(LINED);
            glPopMatrix();
        }

    private:
        Field<3, elemType>& field;
        bool selected;
        Renderer<elemType>* selectionRenderer;
        Color4f selectColor;
        Vector3i selection;
};
