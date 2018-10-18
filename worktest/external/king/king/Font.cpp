#include "Font.h"
#include <stdexcept>

namespace King {
	Font::Font(const char* filename, int size) 
		: mFont(TTF_OpenFont(filename, size)) { 
		if (!mFont) {
			throw std::runtime_error(SDL_GetError());
		}
	}

	Font::~Font() { 
		if (mFont) {
			TTF_CloseFont(mFont);
		}
	}

	TTF_Font* Font::GetTTF() const {
		return mFont;
	}
}