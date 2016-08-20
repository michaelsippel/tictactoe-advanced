#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <field.h>
#include <viewer.h>
#include <renderer.h>

int main(int argc, char* argv[])
{
    printf("Hello World!\n");

    int width = 1280;
    int height = 720;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_Window* sdl_window = SDL_CreateWindow("TicTacToe Advanced", 0, 0, width, height, SDL_WINDOW_OPENGL);
    SDL_GLContext* sdl_glcontext = (SDL_GLContext*) SDL_GL_CreateContext(sdl_window);

    if(glewInit() != GLEW_OK)
    {
        printf("GLEW init failed!\n");
    }

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    double fovy = 60.0;
    double aspect = (double)width / (double)height;
    double znear = 0.5;
    double zfar = 100.0;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective((GLdouble) fovy, (GLdouble) aspect, (GLdouble) znear, (GLdouble) zfar);

    Field3D* field = new Field3D(Vector3i(3,3,3),
                                 [] ()
    {
        return new Field<3, CellStatus>(Vector3i(3,3,3),
                                        []()
        {
            return new CellStatus();
        });
    });

    Viewer* viewer = new Viewer();
    auto renderer = new Renderer<Field3D>(*field);

    viewer->zoom(3.0);

    float rx = 0.0f;
    float ry = 0.0f;

    bool ex = false;
    while(! ex)
    {
        static float time_prev = 0;
        float time_cur = (float) SDL_GetTicks();

        float frametime = time_cur - time_prev;
        time_prev = time_cur;

        viewer->clear();
        viewer->loadMatrix();
        renderer->draw();

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                            rx = 1.0f;
                            break;
                        case SDLK_s:
                            rx = -1.0f;
                            break;
                        case SDLK_a:
                            ry = 1.0f;
                            break;
                        case SDLK_d:
                            ry = -1.0f;
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                        case SDLK_s:
                            rx = 0.0f;
                            break;
                        case SDLK_a:
                        case SDLK_d:
                            ry = 0.0f;
                    }
                    break;

                case SDL_MOUSEWHEEL:
                    viewer->zoom((float)event.wheel.y * -0.6);
                    break;

                case SDL_QUIT:
                    ex = true;
                    break;
            }
        }

        viewer->rotateX(0.17 * rx * frametime);
        viewer->rotateY(0.17 * ry * frametime);

        SDL_GL_SwapWindow(sdl_window);
    }

    SDL_DestroyWindow(sdl_window);
    SDL_GL_DeleteContext(sdl_glcontext);

    delete viewer;
    delete field;

    return 0;
}
