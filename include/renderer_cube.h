#pragma once

// Cubes
enum cube
{
    LINED,
    SOLID
};

template <>
class Renderer<enum cube>
{
        public:
            Renderer()
        {
            if(! init)
            {
                init = true;
                static const GLfloat cubevert[] =
                {
                    -0.5f,  -0.5f, -0.5f,
                    0.5f,  -0.5f, -0.5f,
                    0.5f,   0.5f, -0.5f,
                    -0.5f,   0.5f, -0.5f,

                    -0.5f,  -0.5f,  0.5f,
                    0.5f,  -0.5f,  0.5f,
                    0.5f,   0.5f,  0.5f,
                    -0.5f,   0.5f,  0.5f,
                };

                static const GLushort line_cubeindex[] =
                {
                    0,1, 1,2, 2,3, 3,0,
                    4,5, 5,6, 6,7, 7,4,
                    0,4, 1,5, 2,6, 3,7,
                };
                static const GLushort face_cubeindex[] =
                {
                    0,1,2,3,
                    4,5,6,7,
                    0,1,5,4,
                    1,2,6,5,
                    2,3,7,6,
                    3,0,4,7,
                };

                glGenBuffers(1, &vbuf_id);
                glGenBuffers(1, &libuf_id);
                glGenBuffers(1, &fibuf_id);

                glBindBuffer(GL_ARRAY_BUFFER, vbuf_id);
                glBufferData(GL_ARRAY_BUFFER, sizeof(cubevert), cubevert, GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, libuf_id);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_cubeindex), line_cubeindex, GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fibuf_id);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face_cubeindex), face_cubeindex, GL_STATIC_DRAW);
            }
        }

        static void draw(enum cube type)
        {
            switch(type)
            {
                case LINED:
                    glBindBuffer(GL_ARRAY_BUFFER, vbuf_id);
                    glVertexPointer(3, GL_FLOAT, 0, NULL);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, libuf_id);

                    glEnableClientState(GL_VERTEX_ARRAY);
                    glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, (void*)0);
                    glDisableClientState(GL_VERTEX_ARRAY);
                    break;

                case SOLID:
                    glBindBuffer(GL_ARRAY_BUFFER, vbuf_id);
                    glVertexPointer(3, GL_FLOAT, 0, NULL);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fibuf_id);

                    glEnableClientState(GL_VERTEX_ARRAY);
                    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, (void*)0);
                    glDisableClientState(GL_VERTEX_ARRAY);
                    break;
            }
        }

        static bool init;

    private:
        static GLuint vbuf_id;
        static GLuint libuf_id;
        static GLuint fibuf_id;
};
