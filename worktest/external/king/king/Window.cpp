#include "Window.h"
#include <stdexcept>
#include <SDL.h>

namespace King {
	Window::Window(size_t width, size_t height)
		: mWindow(SDL_CreateWindow("KingTest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, static_cast<int>(width), static_cast<int>(height), 0)) 
		, mRenderer(SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) { 
		if (!mWindow || !mRenderer) {
			throw std::runtime_error(SDL_GetError());
		}
	}

	Window::~Window() { 
		if (mWindow) {
			SDL_DestroyWindow(mWindow);
		}
	}

	SDL_Surface* Window::GetSurface() {
		return SDL_GetWindowSurface(mWindow);
	}

	SDL_Renderer* Window::GetRenderer() {
		return mRenderer;
	}

}