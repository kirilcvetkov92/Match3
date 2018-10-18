#include "TextItem.h"

namespace {
	SDL_Texture* GenerateTexture(const char* text, TTF_Font* font, SDL_Renderer* renderer) {
		const SDL_Color color = { 225, 225, 225, 255 };
		const auto surface = TTF_RenderText_Blended(font, text, color);
		const auto texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}
}

namespace King {
	TextItem::TextItem(const char* text, TTF_Font* font, SDL_Renderer* renderer)
		: mTexture(nullptr)
		, mRenderer(renderer)
		, mFont(font)
		, mWidth(-1), mHeight(-1) {
		SetText(text);
	}

	TextItem::~TextItem() {
		if (mTexture) {
			SDL_DestroyTexture(mTexture);
		}
	}

	void TextItem::SetText(const char* text) {
		if (mText != text) {
			mText = text;

			if (mTexture) {
				SDL_DestroyTexture(mTexture);
			}

			mTexture = GenerateTexture(text, mFont, mRenderer);
			SDL_QueryTexture(mTexture, nullptr, nullptr, &mWidth, &mHeight);
		}
	}

	SDL_Texture* TextItem::GetTexture() const {
		return mTexture;
	}

	int TextItem::GetWidth() const {
		return mWidth;
	}

	int TextItem::GetHeight() const {
		return mHeight;
	}
}
