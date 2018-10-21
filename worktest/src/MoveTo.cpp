//
//  MoveTo.cpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/20/18.
//

#include "MoveTo.hpp"


MoveTo::MoveTo(Position destination, float seconds):Action(seconds),
mCurrentPosition(Position(0,0)),
mDestination(destination),
mSource(Position(0,0)),
mStep(Step(0,0))
{
  
}
void MoveTo::setSource(Position &position)
{
    mSource = Position(position.mX, position.mY);
    Initialize();
}
void MoveTo::Initialize()
{
    mStep = Step((mDestination.mX-mSource.mX)/mSeconds, (mDestination.mY-mSource.mY)/mSeconds);
}

void MoveTo::PerformAction(float period)
{
    mCurrentPosition = Position(mSource.mX+period*mStep.mX,mSource.mY+period*mStep.mY);
}


void MoveTo::OnActionFinished()
{
    //todo
}
