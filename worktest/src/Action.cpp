//
//  Action.cpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/20/18.
//

#include "Action.hpp"

namespace King {
    Action::Action(float seconds) {
        mSeconds = seconds;
        mBegin = steady_clock::now();
        mState = King::Action::State::RUNNING;
    }
    
    Action::~Action()
    {
    }
    
    void Action::Update()
    {
        if(mState == King::Action::State::RUNNING)
        {
            
            steady_clock::time_point updateTime = steady_clock::now();
            steady_clock::duration time_span = updateTime - mBegin;
            
            double nseconds = double(time_span.count()) * steady_clock::period::num / steady_clock::period::den;
            
            if(mSeconds - nseconds >= 1e-8)
            {
                PerformAction(nseconds);
            }
            else{
                OnActionFinished();
                mState = King::Action::State::FINISHED;
            }
        }
        else{
            mState = King::Action::State::FINISHED;
        }
    }
}

