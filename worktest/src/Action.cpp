//
//  Action.cpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/20/18.
//

#include "Action.hpp"

namespace King {
    Action::Action(float seconds, std::weak_ptr<View> view) {
        mSeconds = seconds;
        mBegin = steady_clock::now();
        mView = view;
        mState = King::Action::State::RUNNING;
    }
    
    void Action::Update()
    {
        if(mState == King::Action::State::RUNNING)
        {
            if (auto viewPtr = mView.lock()) {
                
                steady_clock::time_point updateTime = steady_clock::now();
                steady_clock::duration time_span = updateTime - mBegin;
                
                double nseconds = double(time_span.count()) * steady_clock::period::num / steady_clock::period::den;
                
                
                if(mSeconds - nseconds < 1e-8)
                {
                    PerformAction(nseconds);
                }
                else{
                    mState = King::Action::State::FINISHED;
                }
            }
        }
    }
}
