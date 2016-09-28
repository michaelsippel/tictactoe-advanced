#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <math/vector.h>

template <typename T>
class Renderer;

using namespace huge;

class CellStatus
{
    public:
        CellStatus() :value(0) {}
        CellStatus(int v) : value(v) {}
        int value;

        Renderer<CellStatus>* getRenderer(void);
};

template <int N, typename elemType>
class Field
{
    public:
        Field(Vector<N, uint32_t> size_, elemType* (*create) (void))
            : size(size_)
        {
            num_e = 1;
            for(int i = 0; i < N; i++)
                num_e *=  this->size.data[i];

            this->cells = (elemType**) malloc(num_e * sizeof(elemType*));

            for(int i=0; i < num_e; i++)
                this->cells[i] = create();
        }

        ~Field()
        {
            for(int i=0; i < num_e; i++)
                delete this->cells[i];

            free(this->cells);
        }

        elemType* getCell(Vector<N, uint32_t> pos)
        {
            return this->cells[this->index(pos)];
        }

        Vector<N, uint32_t> getSize(void)
        {
            return this->size;
        }

        Renderer<Field<N, elemType>>* getRenderer(void);

    private:
        int index(Vector<N, uint32_t> pos)
        {
            int index = 0;
            size_t off = 1;
            for(int i = 0; i < N; i++)
            {
                index += off * pos.data[i];
                off *= this->size.data[i];
            }
            return index;
        }

        Vector<N, uint32_t> size;
        int num_e;
        elemType** cells;
};


typedef Field<3, Field<3, CellStatus>> Field3D;
/*
#include <renderer.h>

template <int N, typename elemType>
Renderer<Field<N, elemType>>* Field<N, elemType>::getRenderer(void)
{
    return new Renderer<Field<N, elemType>>(*this);
}

Renderer<CellStatus>* CellStatus::getRenderer(void)
{
    return new Renderer<CellStatus>(*this);
}
*/