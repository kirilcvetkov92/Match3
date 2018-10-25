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
    , mGameStateMachine()
    , mMouseButtonWasDown(true)
    , mStartClick(INT_MIN,INT_MIN)
    , mCurrentClick(INT_MIN,INT_MIN)
    {
        InitRoot();
        
    }
    
    ~Game()
    {
        mRoot->RemoveAllChildren();
    }
    
    void InitRoot()
    {
        mRoot = std::make_unique<View>();
        
        // add background
        mBackground = std::make_unique<ViewSprite>();
        mBackground->SetTexture(King::Engine::Texture::TEXTURE_BACKGROUND);
        mBackground->SetPosition(Position(400.0f,300.0f));
        mRoot->AddChild(mBackground.get());
        
        //add label
        mInfoLabel = std::make_unique<ViewText>();
        mInfoLabel->SetPosition(Position(500,250));
        mRoot->AddChild(mInfoLabel.get());
        
        //set counter
        mCounter = std::make_unique<ViewText>();
        mRoot->AddChild(mCounter.get());
        mCounter->SetPosition(Position(150,350));
    }
    
    void Start() {
        InitModelViewComponents();
        mEngine.Start(*this);
    }
    
    void InitModelViewComponents()
    {
        mGameStateMachine.SendEvent(StateMachine::Event::RESUME);
        
        //create grid view
        mViewGrid.SetPosition(350.0f, 100.0f);
        mRoot->AddChild(&mViewGrid);
        
        //clear all callbacks
        mCallBacks.clear();
        
        
        //clear all informations
        ClearInfo();
        
        InitCounter(Settings::START_COUNTER, GAME_CALLBACK(Game::OnGameReady, this),  getGameString());
        
        mModelGrid = std::make_shared<ModelGrid>(
                                                 Settings::MODEL_GRID_WIDTH,
                                                 Settings::MODEL_GRID_HEIGHT,
                                                 Settings::MODEL_GRID_MATCH_LENGTH);
        mViewGrid.SetModel(mModelGrid);
        
        auto callBackTime = std::make_shared<CallBack>(GAME_CALLBACK(Game::OnSecondElapsed, this), 1);
        mCallBacks.insert(callBackTime);
    }
    
    void OnRestartGame()
    {
        mViewGrid = ViewGrid(Settings::VIEW_GRID_SPACING, Settings::VIEW_GEM_DEBUG_LABEL_OFFSET);
        mGameStateMachine.SendEvent(StateMachine::Event::NONE);
    }
    
    void OnGameReady()
    {
        mGameStateMachine.SendEvent(StateMachine::Event::INITIALIZED);
        InitCounter(Settings::GAME_COUNTER, GAME_CALLBACK(Game::OnGameEnd, this) , getGameString());
    }
    
    void WriteInfo(const std::string &message)
    {
        mInfoLabel->setVisible(true);
        mInfoLabel->SetTextString(message);
        mInfoLabel->SetTextItem(King::Engine::TEXT_ITEM_03);
    }
    
    void ClearInfo()
    {
        mInfoLabel->setVisible(false);
    }
    
    void InitCounter(int seconds, const std::function<void()> &func, const std::string &prefix)
    {
        mTime = seconds;
        auto callBackEnd = std::make_shared<CallBack>(func, seconds);
        mCallBacks.insert(callBackEnd);
        mCounter->setVisible(true);
        
        SetCounter(seconds, prefix);
    }
    
    void SetCounter(int seconds, const std::string &prefix)
    {
        std::ostringstream os;
        os<<prefix<<" "<<seconds ;
        mCounter->SetTextString(os.str());
        mCounter->SetTextItem(King::Engine::TEXT_ITEM_03);
    }
    
    void UpdateCallBacks()
    {
        // Iterate over the vector till end
        std::vector<std::shared_ptr<CallBack>> callbacksForRemoval;
        
        for(auto callback : mCallBacks)
        {
            if (callback->mState == Action::State::FINISHED)
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
            case StateMachine::State::RESTART:
                InitModelViewComponents();
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
        mRoot->RemoveChild(&mViewGrid);
        mGameStateMachine.SendEvent(StateMachine::Event::EXIT);
        
        WriteInfo("GAME OVER");
        mCounter->setVisible(false);
        
        auto callBackTime = std::make_shared<CallBack>(GAME_CALLBACK(Game::OnRestartGame, this), 3);
        mCallBacks.insert(callBackTime);
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
        SetCounter(mTime,getGameString());
        
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
    
    std::string getGameString()
    {
        StateMachine::State state = mGameStateMachine.GetCurrentState();
        if (state == StateMachine::State::NEW)
        {
            return "READY ";
        }
        else{
            return "PLAY ";
        }
    }
    
private:
    King::Engine mEngine;
    
    std::shared_ptr<ModelGrid> mModelGrid;
    
    ViewGrid mViewGrid;
    std::unique_ptr<ViewSprite> mBackground;
    std::unique_ptr<ViewText> mCounter;
    std::unique_ptr<ViewText> mInfoLabel;
    std::unique_ptr<View> mRoot;
    
    std::set<std::shared_ptr<CallBack>> mCallBacks;
    
    StateMachine mGameStateMachine;
    
    bool mMouseButtonWasDown;
    int mTime=0;
    
    Position mStartClick;
    Position mCurrentClick;
    
};

int main(int argc, char *argv[]) {
    Game application;
    application.Start();
    return 0;
}
