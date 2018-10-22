#define GLM_FORCE_RADIANS

#include "Settings.h"
#include "Point2.h"
#include "ModelGrid.h"
#include "ViewGrid.h"

#include <king/Engine.h>
#include <king/Updater.h>
#include "ViewText.h"
#include "CallBack.hpp"
#include <set>
#include <sstream>

#ifdef __APPLE__
#define ASSETS_PATH "./../bin/assets"
#else
#define ASSETS_PATH "./assets"
#endif

class Game : public King::Updater {
    typedef Position ClickPosition;
    
public:
	Game()
	: mEngine(ASSETS_PATH)
	, mViewGrid(
		Settings::VIEW_GRID_SPACING,
		Settings::VIEW_GEM_DEBUG_LABEL_OFFSET)
    , mCallBacks()
	, mMouseButtonWasDown(true)
    , mStartClick(-1,-1)
    , mGameState(Game::GameState::NEW){
	}
	
	void Start() {
		mViewGrid.SetPosition(350.0f, 100.0f);
        InitRoot();
        SetBackground();
        InitCounter(Settings::COUNTER);
		mEngine.Start(*this);
	}
    
    void InitRoot()
    {
        mRoot = std::make_unique<View>();
    }
    
    void SetBackground()
    {
        mBackground = std::make_unique<ViewSprite>();
        mBackground->SetTexture(King::Engine::Texture::TEXTURE_BACKGROUND);
        mBackground->SetPosition(Position(400.0f,300.0f));
        mRoot->AddChild(mBackground.get());
        mRoot->AddChild(&mViewGrid);
    }
	
    void InitCounter(int seconds)
    {
        mCounter = std::make_unique<ViewText>();
        mCounter->SetPosition(Position(150,350));
        mRoot->AddChild(mCounter.get());
        SetCounter(seconds);
        mTime = seconds;
        auto callBackEnd = std::make_shared<CallBack>(GAME_CALLBACK(Game::OnGameEnd, this), seconds);
        auto callBackTime = std::make_shared<CallBack>(GAME_CALLBACK(Game::OnSecondElapsed, this), 1);
        mCallBacks.insert(callBackEnd);
        mCallBacks.insert(callBackTime);
    }
    
    void SetCounter(int seconds)
    {
        std::ostringstream os;
        os<<seconds ;
        mCounter->SetTextString(os.str());
        mCounter->SetTextItem(King::Engine::TEXT_ITEM_03);
    }
    
    void UpdateCallBacks()
    {
        // Iterate over the vector till end
        std::vector<std::shared_ptr<CallBack>> callbacksForRemoval;
        
        for(auto callback : mCallBacks)
        {
            if (callback->mState == King::Action::State::FINISHED)
            {
                callbacksForRemoval.push_back(callback);
            }
            else
                callback->Update();
     
        }
       
        for(auto callback : callbacksForRemoval)
        {
            mCallBacks.erase(callback);
        }

    }
    
	void Update() override {
        
		if (mEngine.GetMouseButtonDown() && !isClickPaused()) {
			mMouseButtonWasDown = true;
            
            if(mGameState==GameState::READY)
                mStartClick = Position(mEngine.GetMouseX(), mEngine.GetMouseY());
            
            mGameState=GameState::CLICK;
            Position currentPosition(mEngine.GetMouseX(), mEngine.GetMouseY());
            
            if(mViewGrid.ApplyInteraction(mStartClick, currentPosition))
            {
                PauseClick();
                auto callBackTime = std::make_shared<CallBack>(GAME_CALLBACK(Game::ResumeClick, this), 0.5);
                mCallBacks.insert(callBackTime);
            }
            
		}
        else if (mMouseButtonWasDown) {
            mMouseButtonWasDown = false;
            if (mGameState==Game::GameState::NEW)
            {
                mGameState = Game::GameState::READY;
                mModelGrid = std::make_shared<ModelGrid>(
                    Settings::MODEL_GRID_WIDTH,
                    Settings::MODEL_GRID_HEIGHT,
                    Settings::MODEL_GRID_MATCH_LENGTH);
                mViewGrid.SetModel(mModelGrid);
                mModelGrid->Match();
                mModelGrid->Drop();
                mModelGrid->RemoveMatchedGems();
                mModelGrid->MoveDroppedGems();
                //mModelGrid->GenerateGemsOnTop();
            }
		}
        
        if (!mEngine.GetMouseButtonDown())
        {
            if (mGameState==GameState::CLICK)
            {
                mGameState = GameState::READY;
            }
        }
        
        mViewGrid.UpdateViews();
        UpdateCallBacks();

		mRoot->Render(mEngine);
	}
	
    void OnGameEnd()
    {
        //todo
    }
    
    void OnSecondElapsed()
    {
        mTime = mTime-1;
        SetCounter(mTime);
        auto callBackTime = std::make_shared<CallBack>(GAME_CALLBACK(Game::OnSecondElapsed, this), 1);
        mCallBacks.insert(callBackTime);

    }
    
    bool isClickPaused(){
        return mGameState == GameState::IDLE;
    }
    void PauseClick(){
        mGameState = GameState::IDLE;
    }
    void ResumeClick()
    {
        mGameState = GameState::READY;
    }
    
private:
	King::Engine mEngine;

	std::shared_ptr<ModelGrid> mModelGrid;
	
	ViewGrid mViewGrid;
    std::unique_ptr<ViewSprite> mBackground;
    std::unique_ptr<ViewText> mCounter;
    std::unique_ptr<View> mRoot;
    std::set<std::shared_ptr<CallBack>> mCallBacks;
    
	bool mMouseButtonWasDown;
    int mTime=60;
    Position mStartClick;
    
    enum class GameState {
        NEW,
        READY,
        CLICK,
        IDLE,
        FINISH
    };
    
    GameState mGameState;
};

int main(int argc, char *argv[]) {
	Game application;
	application.Start();
	return 0;
}
