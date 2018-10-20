//
//  MoveTo.cpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/20/18.
//

#include "MoveTo.hpp"


namespace King {
    MoveTo::MoveTo(Position destination, float seconds, std::weak_ptr<View> view):Action(seconds, view),
    mDestination(destination),
    mStep(Step(0,0))
    {
        Initialize();
    }
    
    void MoveTo::Initialize()
    {
        if (auto viewPtr = mView.lock()) {
            Position mInitialPosition = viewPtr->GetPosition();
            mStep = Step((mDestination.mX-mInitialPosition.mX)/mSeconds, (mDestination.mY-mInitialPosition.mY)/mSeconds);
        }
    }
    
    void MoveTo::PerformAction(float period)
    {
        if (auto viewPtr = mView.lock()) {
            Position currentPosition = viewPtr->GetPosition();
            currentPosition.mX+=mStep.mX;
            currentPosition.mY+=mStep.mY;
            viewPtr->SetPosition(currentPosition);
        }
    }
}
