#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[])
{
	printf("Hello World!\n");

	int width = 800;
	int height = 600;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	SDL_Window* sdl_window = SDL_CreateWindow("TicTacToe Advanced", 0, 0, width, height, SDL_WINDOW_OPENGL);
	SDL_GLContext* sdl_glcontext = (SDL_GLContext*) SDL_GL_CreateContext(sdl_window);

	bool ex = false;
	while(! ex)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch(event.type)
		{
			case SDL_QUIT:
				ex = true;
				break;
		}
	}

	SDL_DestroyWindow(sdl_window);
	SDL_GL_DeleteContext(sdl_glcontext);

	return 0;
}