#pragma once
#include <string>
#include "SDL_render.h"

namespace King {
	class Sprite {
	public:
		Sprite(const char* filename, SDL_Renderer* renderer);
		~Sprite();

		SDL_Texture* GetTexture() const;
		int GetWidth() const;
		int GetHeight() const;

	private:
		SDL_Texture* mTexture;
		int mWidth;
		int mHeight;
	};
}