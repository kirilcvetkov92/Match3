#pragma once
#include <stddef.h>

#include <stdlib.h>

struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;

namespace King {
	class Window {
	public:
		Window(size_t width, size_t height);
		~Window();

		SDL_Surface* GetSurface();
		SDL_Renderer* GetRenderer();

	private:
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
	};
}
