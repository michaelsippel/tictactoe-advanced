#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

#include <color.h>

#include <field.h>
#include <viewer.h>
#include <renderer.h>

#include <connection.h>

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
    SDLNet_Init();

    char servername[64];
    char username[64];

    std::cout << "Please the Server-IP: ";
    std::cin >> servername;
    std::cout << "Please enter your username:  ";
    std::cin >> username;

    Connection c = Connection(servername, 12345, username);

    return 0;

    int width = 1280;
    int height = 720;
    bool supersampling = false;
    double fovy = 60.0;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_Window* sdl_window = SDL_CreateWindow("TicTacToe Advanced", 0, 0, width, height, SDL_WINDOW_OPENGL);
    SDL_GLContext* sdl_glcontext = (SDL_GLContext*) SDL_GL_CreateContext(sdl_window);

    if(glewInit() != GLEW_OK)
    {
        printf("GLEW init failed!\n");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


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

    Renderer<Field3D>* selector1 = NULL;
    Renderer<Field<3, CellStatus>>* selector2 = NULL;

    Vector3i selection = Vector3i(0, 0, 0);
    Vector3i lvl1_select;

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

        static float t = 0.0f;
        static int nframes = 0;
        t += frametime;
        nframes++;
        if(t >= 1000.0f)
        {
            printf("%d FPS, %f ms\n", nframes, frametime);
            nframes = 0;
            t = 0.0f;
        }

        // rendering
        viewer->clear();
        viewer->loadMatrix();

        if(supersampling)
        {
            glMatrixMode(GL_MODELVIEW);
            for(int i = 0; i < 4; i++)
            {
                glPushMatrix();
                glTranslatef((i % 2) * 0.5f / width, (i / 2) * 0.5f / height, 0.0f);
                renderer->draw();
                glAccum(i ? GL_ACCUM : GL_LOAD, 0.25);
                glPopMatrix();
            }

            glAccum(GL_RETURN, 1.0f);
        }
        else
            renderer->draw();

        // event handling
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

                        case SDLK_ESCAPE:
                            renderer->deselect();
                            selector1 = NULL;
                            selector2 = NULL;
                            break;

                        case SDLK_RETURN:
                            if(selector1 == NULL)
                            {
                                selector1 = renderer;
                                selector1->select(Color4f(0.8f, 0.0f, 0.0f, 0.8f));
                                selector1->select(selection, Color4f(0.0f, 0.8f, 0.0f, 0.8f));
                            }
                            else if(selector2 == NULL)
                            {
                                lvl1_select = selection;
                                selector2 = selector1->select(selection, Color4f(0.8f, 0.0f, 0.0f, 0.8f));
                                selector2->select(selection, Color4f(0.0f, 0.8f, 0.0f, 0.8f));
                            }
                            else
                            {
                                *(field->getCell(lvl1_select)->getCell(selection)) = 1;
                                renderer->deselect();
                                selector1 = NULL;
                                selector2 = NULL;
                            }
                            break;

                        case SDLK_i:
                            selection.x()++;
                            selection.x() %= 3;
                            goto update_selection;
                        case SDLK_o:
                            selection.y()++;
                            selection.y() %= 3;
                            goto update_selection;
                        case SDLK_p:
                            selection.z()++;
                            selection.z() %= 3;
                            goto update_selection;

update_selection:
                            if(selector1 != NULL && selector2 == NULL)
                                selector1->select(selection, Color4f(0.0f, 0.8f, 0.0f, 0.8f));
                            else if(selector2 != NULL)
                                selector2->select(selection, Color4f(0.0f, 0.8f, 0.0f, 0.8f));
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

        int mx, my;
        if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            // pressed left button
        }

        // animation
        viewer->rotateX(0.17 * rx * frametime);
        viewer->rotateY(0.17 * ry * frametime);

        // swap buffers
        SDL_GL_SwapWindow(sdl_window);
    }

    SDL_DestroyWindow(sdl_window);
    SDL_GL_DeleteContext(sdl_glcontext);

    delete viewer;
    delete field;

    return 0;
}
