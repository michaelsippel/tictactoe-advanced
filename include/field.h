#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <math/vector.h>

using namespace huge;

template <int N, typename elemType>
class Field
{
    public:
        Field(Vector<N, int> size_)
            : size(size_)
        {
            int num_e = 1;
            for(int i = 0; i < N; i++)
                num_e *=  this->size.data[i];

            this->cells = (elemType*) malloc(num_e * sizeof(elemType));
        }

        ~Field()
        {
            free(this->cells);
        }

        void setCell(Vector<N, int> pos, elemType value)
        {
            this->cells[this->index(pos)] = value;
        }

        elemType getCell(Vector<N, int> pos)
        {
            return this->cells[this->index(pos)];
        }

        Vector<N, int> getSize(void)
        {
            return this->size;
        }

    private:
        int index(Vector<N, int> pos)
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

        Vector<N, int> size;
        elemType* cells;
};

typedef Field<3, Field<3, int>*> Field3D;
