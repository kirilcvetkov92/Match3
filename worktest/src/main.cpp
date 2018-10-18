#define GLM_FORCE_RADIANS

#include "Settings.h"
#include "Point2.h"
#include "ModelGrid.h"
#include "ViewGrid.h"

#include <king/Engine.h>
#include <king/Updater.h>

#ifdef __APPLE__
#define ASSETS_PATH "./../bin/assets"
#else
#define ASSETS_PATH "./assets"
#endif

class Game : public King::Updater {
public:
	Game()
	: mEngine(ASSETS_PATH)
	, mViewGrid(
		Settings::VIEW_GRID_SPACING,
		Settings::VIEW_GEM_DEBUG_LABEL_OFFSET)
	, mMouseButtonWasDown(true) {
	}
	
	void Start() {
		mViewGrid.SetPosition(100.0f, 100.0f);
		mEngine.Start(*this);
	}
	
	void Update() override {
		if (mEngine.GetMouseButtonDown()) {
			mMouseButtonWasDown = true;
		} else if (mMouseButtonWasDown) {
            mMouseButtonWasDown = false;
            mModelGrid = std::make_shared<ModelGrid>(
                Settings::MODEL_GRID_WIDTH,
                Settings::MODEL_GRID_HEIGHT,
                Settings::MODEL_GRID_MATCH_LENGTH);
            mViewGrid.SetModel(mModelGrid);
            mModelGrid->Match();
		}
        mViewGrid.UpdateViews();

		mViewGrid.Render(mEngine);
	}
	
private:
	King::Engine mEngine;

	std::shared_ptr<ModelGrid> mModelGrid;
	
	ViewGrid mViewGrid;
	
	bool mMouseButtonWasDown;
};

int main(int argc, char *argv[]) {
	Game application;
	application.Start();
	return 0;
}
