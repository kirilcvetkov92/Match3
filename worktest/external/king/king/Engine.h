// (C) king.com Ltd 2014

#pragma once

#include <memory>

namespace King {
	class Updater;
	class Engine {
	public:
		enum Texture {
			TEXTURE_BACKGROUND,
			TEXTURE_BLUE,
			TEXTURE_GREEN,
			TEXTURE_PURPLE,
			TEXTURE_RED,
			TEXTURE_YELLOW,
			TEXTURE_MAX
		};

		enum Texts {
			TEXT_ITEM_00 = 0,
			TEXT_ITEM_01,
			TEXT_ITEM_02,
			TEXT_ITEM_03,
			TEXT_ITEM_04,
			TEXT_ITEM_05,
			TEXT_ITEMS_MAX
		};

		Engine(const char* assetsDirectory);
		~Engine();

		float GetLastFrameSeconds() const;
		float GetMouseX() const;
		float GetMouseY() const;
		bool GetMouseButtonDown() const;
		
		void Start(Updater& updater);
		void Quit();

		int GetTextureHeight(Texture texture) const;
		int GetTextureWidth(Texture texture) const;
		
		void Render(Texture texture, float x, float y, float rotation = 0.0f);
		void Render(Texts textItem, const char* text, float x, float y, float roatation = 0.0f);

		int GetWidth() const;
		int GetHeight() const;

	private:
		struct EngineImplementation;
		std::unique_ptr<EngineImplementation> mPimpl;
	};
}