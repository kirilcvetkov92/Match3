//
//  StateMachine.cpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/23/18.
//

#include "StateMachine.hpp"
#include <iostream>

using namespace std;


StateMachine::StateMachine():mCurrentStateFunction(GAME_CALLBACK_1(StateMachine::OnNew, this)), mCurrentState(NEW)
{
    
}

void StateMachine::SendEvent(StateMachine::Event event)
{
    mCurrentStateFunction(event);
}

StateMachine::State& StateMachine::GetCurrentState()
{
    return mCurrentState;
}



void StateMachine::SetCurrentState(StateMachine::State state)
{
    mCurrentState = state;
    
    switch(state)
    {
        case State::NEW:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnNew, this);
            break;
        case State::READY:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnReady, this);
            break;
        case State::TOUCH_BEGIN1:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnTouchBegin1, this);
            break;
        case State::TOUCH_END1:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnTouchEnd1, this);
            break;
        case State::TOUCH_BEGIN2:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnTouchBegin2, this);
            break;
        case State::SWIPE:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnSwipe, this);
            break;
        case State::END:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnEnd, this);
            break;
        case State::IDLE:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnIdle, this);
            break;
        case State::SWIPE_CLICK:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnSwipeClick, this);
            break;
        case State::CORRECT_START_POSITION:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnCorrectStartPosition, this);
            break;
        case State::RESTART:
            mCurrentStateFunction = GAME_CALLBACK_1(StateMachine::OnRestart, this);
            break;
    }
}

void StateMachine::OnNew(StateMachine::Event event)
{
    switch(event)
    {
        case StateMachine::Event::INITIALIZED:
            SetCurrentState(StateMachine::State::READY);
            break;
        default:
            break;
    }
}
void StateMachine::OnReady(Event event)
{
    switch(event)
    {
        case StateMachine::Event::TOUCH_BEGIN:
            SetCurrentState(StateMachine::State::TOUCH_BEGIN1);
            break;
        case StateMachine::Event::EXIT:
            SetCurrentState(StateMachine::State::END);
            break;
        default:
            break;
    }
}

void StateMachine::OnRestart(Event event)
{
    switch(event)
    {
        case StateMachine::Event::RESUME:
            SetCurrentState(StateMachine::State::NEW);
            break;
        default:
            break;
    }
}

void StateMachine::OnTouchBegin1(Event event)
{
    switch(event)
    {
        case StateMachine::Event::TOUCH_END:
            SetCurrentState(StateMachine::State::TOUCH_END1);
            break;
        case StateMachine::Event::TOUCH_BEGIN:
            SetCurrentState(StateMachine::State::SWIPE);
            break;
        case StateMachine::Event::EXIT:
            SetCurrentState(StateMachine::State::END);
            break;
        default:
            break;
    }
}
void StateMachine::OnTouchEnd1(Event event)
{
    switch(event)
    {
        case StateMachine::Event::TOUCH_BEGIN:
            SetCurrentState(StateMachine::State::TOUCH_BEGIN2);
            break;
        case StateMachine::Event::EXIT:
            SetCurrentState(StateMachine::State::END);
            break;
        default:
            break;
    }
}
void StateMachine::OnTouchBegin2(Event event)
{
    switch(event)
    {
        case StateMachine::Event::TOUCH_END:
            SetCurrentState(StateMachine::State::SWIPE_CLICK);
            break;
        case StateMachine::Event::TOUCH_BEGIN:
            SetCurrentState(StateMachine::State::CORRECT_START_POSITION);
            break;

        case StateMachine::Event::EXIT:
            SetCurrentState(StateMachine::State::END);
            break;
            
        default:
            break;
    }
}

void StateMachine::OnSwipe(Event event)
{
    switch(event)
    {
        case StateMachine::Event::TOUCH_END:
            SetCurrentState(StateMachine::State::TOUCH_END1);
            break;
        case StateMachine::Event::PAUSE:
            SetCurrentState(StateMachine::State::IDLE);
            break;
        case StateMachine::Event::EXIT:
            SetCurrentState(StateMachine::State::END);
            break;
        default:
            break;
    }
}

void StateMachine::OnSwipeClick(Event event)
{
    switch(event)
    {
        case StateMachine::Event::TOUCH_BEGIN:
            SetCurrentState(StateMachine::State::TOUCH_END1);
            break;
        case StateMachine::Event::PAUSE:
            SetCurrentState(StateMachine::State::IDLE);
            break;
        case StateMachine::Event::EXIT:
            SetCurrentState(StateMachine::State::END);
            break;
        default:
            break;
    }
}
void StateMachine::OnIdle(Event event)
{
    switch(event)
    {
        case StateMachine::Event::RESUME:
            SetCurrentState(StateMachine::State::READY);
            break;
        case StateMachine::Event::EXIT:
            SetCurrentState(StateMachine::State::END);
            break;
        default:
            break;
    }
}

void StateMachine::OnCorrectStartPosition(Event event)
{
     SetCurrentState(StateMachine::State::READY);
}

void StateMachine::OnEnd(Event event)
{
    switch(event)
    {
        case StateMachine::Event::NONE:
            SetCurrentState(StateMachine::State::RESTART);
            break;
        default:
            break;
    }
    
}

