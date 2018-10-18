#pragma once
#include "Font.h"
#include <string>

namespace King {
	class TextItem {
	public:
		TextItem(const char* text, TTF_Font* font, SDL_Renderer* renderer);
		~TextItem();

		void SetText(const char* text);
		SDL_Texture* GetTexture() const;
	
		int GetWidth() const;
		int GetHeight() const;

	private:
		SDL_Texture* mTexture;
		SDL_Renderer* mRenderer;
		TTF_Font* mFont;
		std::string mText;
		int mWidth;
		int mHeight;
	};
}