#include "Sprite.h" 
#include "SDL_image.h"

#include <stdexcept>

namespace {
	SDL_Texture* LoadTexture(const char* filename, SDL_Renderer* renderer) {
		if (auto surface = IMG_Load(filename)) {
			auto texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			return texture;
		}

		throw std::runtime_error(SDL_GetError());
		return nullptr;
	}
}

namespace King {
    
    
	Sprite::Sprite(const char* filename, SDL_Renderer* renderer)
		: mTexture(::LoadTexture(filename, renderer)) 
		, mWidth(-1)
		, mHeight(-1) { 
		if (!mTexture) {
			throw std::runtime_error(SDL_GetError());
		} else {
			SDL_QueryTexture(mTexture, nullptr, nullptr, &mWidth, &mHeight);
		}
	}

	Sprite::~Sprite() {
		if (mTexture) {
			SDL_DestroyTexture(mTexture);
		}
	}
	
	SDL_Texture* Sprite::GetTexture() const {
		return mTexture;
	}
	int Sprite::GetWidth() const {
		return mWidth;
	}
	int Sprite::GetHeight() const {
		return mHeight;
	}
}
