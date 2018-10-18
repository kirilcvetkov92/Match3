#pragma once
#include "SDL_ttf.h"

namespace King {
	class Font {
	public:
		Font(const char* filename, int size);
		~Font();

		TTF_Font* GetTTF() const;

	private:
		TTF_Font* mFont;
	};
}