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
#include "StateMachine.hpp"


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
    , mStartClick(INT_MIN,INT_MIN)
    , mCurrentClick(INT_MIN,INT_MIN)
    , mGameStateMachine()
    , mGameState(Game::GameState::NEW){
	}
	
	void Start() {
		mViewGrid.SetPosition(350.0f, 100.0f);
        InitRoot();
        SetBackground();
        InitCounter(Settings::COUNTER);
        
        mModelGrid = std::make_shared<ModelGrid>(
                                                 Settings::MODEL_GRID_WIDTH,
                                                 Settings::MODEL_GRID_HEIGHT,
                                                 Settings::MODEL_GRID_MATCH_LENGTH);
        mViewGrid.SetModel(mModelGrid);
        mGameStateMachine.SendEvent(StateMachine::Event::INITIALIZED);
		mEngine.Start(*this);
	}
    
    
    void onGameStart()
    {
//        std::unique_ptr<ViewText> mTextInfo = std::make_unique<ViewText>();
//        mCounter->SetPosition(Position(150,350));
//        mRoot->AddChild(mCounter.get());
//        SetCounter(5);
//        mTime = seconds;
//        auto callBackEnd = std::make_shared<CallBack>(GAME_CALLBACK(Game::OnGameEnd, this), seconds);
//        auto callBackTime = std::make_shared<CallBack>(GAME_CALLBACK(Game::OnSecondElapsed, this), 1);
//        mCallBacks.insert(callBackEnd);
//        mCallBacks.insert(callBackTime);
    }
    
//    void SetCounter(int seconds)
//    {
//        std::ostringstream os;
//        os<<seconds ;
//        mCounter->SetTextString(os.str());
//        mCounter->SetTextItem(King::Engine::TEXT_ITEM_03);
//    }
//    
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
       
        if(mGameStateMachine.GetCurrentState()==StateMachine::State::END)
        {
            mCallBacks.clear();
            return;
        }
        
        for(auto callback : callbacksForRemoval)
        {
            mCallBacks.erase(callback);
        }
    }
    
	void Update() override {
        
		if (mEngine.GetMouseButtonDown()) {
			mMouseButtonWasDown = true;
            Position newClick = Position(mEngine.GetMouseX(), mEngine.GetMouseY());
            
            if(!(mCurrentClick==newClick))
            {
                mCurrentClick = newClick;
                mGameStateMachine.SendEvent(StateMachine::Event::TOUCH_BEGIN);
            }
		}
        else if (mMouseButtonWasDown) {
            mMouseButtonWasDown = false;
            mGameStateMachine.SendEvent(StateMachine::Event::TOUCH_END);
		}
        
        makeStateActions();
        mViewGrid.UpdateViews();
        UpdateCallBacks();

		mRoot->Render(mEngine);
	}
	
    void makeStateActions()
    {
        StateMachine::State currentState = mGameStateMachine.GetCurrentState();
        Position currentClick = Position(mEngine.GetMouseX(), mEngine.GetMouseY());
        
        Coordinate currentCoordinate = mViewGrid.MapPositionCoordinateToGrid(currentClick);
        Coordinate startCoordinate = mViewGrid.MapPositionCoordinateToGrid(mStartClick);
        
        switch (currentState)
        {
            case StateMachine::State::TOUCH_BEGIN1:
                mStartClick = Position(mEngine.GetMouseX(), mEngine.GetMouseY());
                break;
            case StateMachine::State::SWIPE:
                onSwipe();
                break;
            case StateMachine::State::CORRECT_START_POSITION:
                mStartClick = mCurrentClick;
                mGameStateMachine.SendEvent(StateMachine::Event::NONE);
                break;
            case StateMachine::State::SWIPE_CLICK:
                OnSwipeWithClick(currentCoordinate, startCoordinate);
                break;
            case StateMachine::State::TOUCH_END1:
                break;
            case StateMachine::State::IDLE:
                break;
            case StateMachine::State::TOUCH_BEGIN2:
                break;
            default:
                mModelGrid->Match();
                mModelGrid->Drop();
                mModelGrid->RemoveMatchedGems();
                mModelGrid->MoveDroppedGems();
                mModelGrid->GenerateGemsOnTop();
                break;
        }
    }
    
    void OnGameEnd()
    {
        mViewGrid.RemoveAllChildren();
        
        mViewGrid = ViewGrid(Settings::VIEW_GRID_SPACING, Settings::VIEW_GEM_DEBUG_LABEL_OFFSET);
        mGameStateMachine.SendEvent(StateMachine::Event::EXIT);
    }
    
    void onSwipe()
    {
        Position currentPosition = Position(mEngine.GetMouseX(), mEngine.GetMouseY());
        if(mViewGrid.ApplyInteraction(mStartClick, currentPosition))
        {
            PauseClick();
            auto callBackTime = std::make_shared<CallBack>(GAME_CALLBACK(Game::ResumeClick, this), 0.5);
            mCallBacks.insert(callBackTime);
        }
        else{
            mGameStateMachine.SendEvent(StateMachine::Event::TOUCH_BEGIN);
            mStartClick = Position(mEngine.GetMouseX(), mEngine.GetMouseY());
        }

    }
    
    void OnSwipeWithClick(Coordinate &start, Coordinate &current)
    {
        int distance = abs((int)start.mX-(int)current.mX)+abs((int)start.mY-(int)current.mY);
        if(distance==1)
        {
            onSwipe();
        }
        else
        {
            mGameStateMachine.SendEvent(StateMachine::Event::TOUCH_BEGIN);
            mStartClick = Position(mEngine.GetMouseX(), mEngine.GetMouseY());
        }
    }
    
    void OnSecondElapsed()
    {
        mTime = mTime-1;
        SetCounter(mTime);
        
        auto callBackTime = std::make_shared<CallBack>(GAME_CALLBACK(Game::OnSecondElapsed, this), 1);
        mCallBacks.insert(callBackTime);
    }
    
    bool isClickPaused(){
        return mGameStateMachine.GetCurrentState()==StateMachine::State::IDLE;
    }
    void PauseClick(){
        mGameStateMachine.SendEvent(StateMachine::Event::PAUSE);
    }
    void ResumeClick()
    {
        mGameStateMachine.SendEvent(StateMachine::Event::RESUME);
    }
    
private:
	King::Engine mEngine;

	std::shared_ptr<ModelGrid> mModelGrid;
	
	ViewGrid mViewGrid;
    std::unique_ptr<ViewSprite> mBackground;
    std::unique_ptr<ViewText> mCounter;
    std::unique_ptr<View> mRoot;
    std::set<std::shared_ptr<CallBack>> mCallBacks;
    StateMachine mGameStateMachine;
    
	bool mMouseButtonWasDown;
    int mTime=0;
    
    Position mStartClick;
    Position mCurrentClick;
    
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
