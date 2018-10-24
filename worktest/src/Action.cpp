//
//  Action.cpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/20/18.
//

#include "Action.hpp"

Action::Action(float seconds):mState(Action::State::PENDING) {
    mSeconds = seconds;
}

Action::~Action()
{
}

void Action::Update()
{
    if(mState == Action::State::PENDING)
    {
        mState = Action::State::RUNNING;
        mBegin = steady_clock::now();
    }
    
    if(mState == Action::State::RUNNING)
    {
        
        steady_clock::time_point updateTime = steady_clock::now();
        steady_clock::duration time_span = updateTime - mBegin;
        
        double nseconds = double(time_span.count()) * steady_clock::period::num / steady_clock::period::den;
        
        if(mSeconds - nseconds >= 1e-8)
        {
            PerformAction(nseconds);
        }
        else{
            mState = Action::State::FINISHED;
            OnActionFinished();
        }
    }

}


