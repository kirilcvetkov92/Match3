#include "Engine.h"

#include <memory>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <string>
#include <chrono>
#include <memory>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Font.h"
#include "Window.h"
#include "Updater.h"
#include "Sprite.h"
#include "TextItem.h"


namespace King {
	static const size_t WindowWidth = 800;
	static const size_t WindowHeight = 600;

	struct Engine::EngineImplementation {
		const int mInitCode;
		std::unique_ptr<Window> mWindow;
		std::unique_ptr<Sprite> mSprites[Engine::TEXTURE_MAX];
		std::unique_ptr<TextItem> mTextItems[Engine::TEXT_ITEMS_MAX];
		std::unique_ptr<Font> mFont;
		Updater* mUpdater;
		float mFrameTime;

		struct MouseData {
			float mPositionX;
			float mPositionY;
			bool mButtonDown;
		};

		MouseData mMouse = { 0, 0, false };
		bool mExitRequested;
		
		EngineImplementation()
			: mInitCode(SDL_Init(SDL_INIT_EVERYTHING) != 0)
			, mWindow(std::make_unique<Window>(WindowWidth, WindowHeight))
			, mUpdater(nullptr)
			, mFrameTime(0.0f)
			, mExitRequested(false) {
			if (TTF_Init() != 0) {
				throw std::runtime_error("Failed to initialize SDL ttf");
			}
		}

		~EngineImplementation() {
			mWindow.reset();
			mFont.reset();
			for (auto& sprite : mSprites) {
				sprite.reset();
			}

			TTF_Quit();
			SDL_Quit();
		}

		void InitializeTexts() {
			for (auto& item : mTextItems) {
				item.reset(new TextItem("", mFont->GetTTF(), mWindow->GetRenderer()));
			}
		}

		void LoadSprite(Engine::Texture texture, const std::string& filename) {
			mSprites[texture].reset(new Sprite(filename.c_str(), mWindow->GetRenderer()));
		}

		Sprite& GetSprite(Engine::Texture texture) const {
			return *mSprites[texture];
		}

		void LoadFont(const std::string& filename, int size) {
			mFont.reset(new Font(filename.c_str(), size));
		}

		Font& GetFont() const {
			return *mFont;
		}

		TextItem& GetTextItem(Engine::Texts item) {
			return *mTextItems[item];
		}

		void Start();
		void PollEvents();
	};

	Engine::Engine(const char* assetsDirectory)
		: mPimpl(new EngineImplementation) {
		const std::string assets(assetsDirectory);
		mPimpl->LoadSprite(Engine::TEXTURE_BACKGROUND, assets + "/BackGround.jpg");
		mPimpl->LoadSprite(Engine::TEXTURE_BLUE, assets + "/Blue.png");
		mPimpl->LoadSprite(Engine::TEXTURE_GREEN, assets + "/Green.png");
		mPimpl->LoadSprite(Engine::TEXTURE_PURPLE, assets + "/Purple.png");
		mPimpl->LoadSprite(Engine::TEXTURE_RED, assets + "/Red.png");
		mPimpl->LoadSprite(Engine::TEXTURE_YELLOW, assets + "/Yellow.png");
		mPimpl->LoadFont(assets + "/fonts/Roboto-Bold.ttf", 24);
		mPimpl->InitializeTexts();
	}

	Engine::~Engine() { }

	float Engine::GetLastFrameSeconds() const {
		return mPimpl->mFrameTime;
	}

	float Engine::GetMouseX() const {
		return mPimpl->mMouse.mPositionX;
	}

	float Engine::GetMouseY() const {
		return mPimpl->mMouse.mPositionY;
	}

	bool Engine::GetMouseButtonDown() const {
		return mPimpl->mMouse.mButtonDown;
	}

    
	void Engine::Quit() {
		mPimpl->mExitRequested = true;
	}

	void Engine::Start(Updater& updater) {
		mPimpl->mUpdater = &updater;
		mPimpl->Start();
	}

	int Engine::GetTextureHeight(Texture texture) const {
		return mPimpl->GetSprite(texture).GetHeight();
	}

	int Engine::GetTextureWidth(Texture texture) const {
		return mPimpl->GetSprite(texture).GetWidth();
	}

	void Engine::Render(Texture texture, float x, float y, float rotation) {
		const auto& sprite = mPimpl->GetSprite(texture);
		const SDL_Rect destination = { static_cast<int>(x-sprite.GetWidth()/2), static_cast<int>(y-sprite.GetHeight()/2), sprite.GetWidth(), sprite.GetHeight() };
		SDL_RenderCopyEx(mPimpl->mWindow->GetRenderer(), sprite.GetTexture(), nullptr, &destination, rotation, nullptr, SDL_FLIP_NONE);
	}

	void Engine::Render(Texts item, const char* text, float x, float y, float roatation) {
		auto& textItem = mPimpl->GetTextItem(item);
		textItem.SetText(text);
		const SDL_Rect destination = { static_cast<int>(x-textItem.GetWidth()/2), static_cast<int>(y-textItem.GetHeight()/2), textItem.GetWidth(), textItem.GetHeight() };
		SDL_RenderCopyEx(mPimpl->mWindow->GetRenderer(), textItem.GetTexture(), nullptr, &destination, roatation, nullptr, SDL_FLIP_NONE);
	}

	int Engine::GetWidth() const {
		return WindowWidth;
	}

	int Engine::GetHeight() const {
		return WindowHeight;
	}

	void Engine::EngineImplementation::Start() {
		while (!mExitRequested) {
			auto begin = std::chrono::steady_clock::now();
			SDL_RenderClear(mWindow->GetRenderer());
			if (mUpdater) {
				mUpdater->Update();
			}
			SDL_RenderPresent(mWindow->GetRenderer());
			PollEvents();
			auto end = std::chrono::steady_clock::now();
			mFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() / 1000.0f;
		}
	}

	void Engine::EngineImplementation::PollEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
			case SDL_KEYDOWN:
				mExitRequested = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mMouse.mButtonDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mMouse.mButtonDown = false;
				break;
			case SDL_MOUSEMOTION:
				mMouse.mPositionX = static_cast<float>(event.motion.x);
				mMouse.mPositionY = static_cast<float>(event.motion.y);
				break;
			default:
				break;
			}
		}
	}
}
