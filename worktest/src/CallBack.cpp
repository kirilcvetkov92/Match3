//
//  CallBack.cpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/21/18.
//

#include "CallBack.hpp"

CallBack::CallBack(const std::function<void()> &func, float seconds):Action(seconds)
{
    mFunction = func;
}

CallBack::~CallBack()
{

}
void CallBack::Initialize()
{
    //todo
}

void CallBack::OnActionFinished()
{
    mFunction();
}

void CallBack::PerformAction(float period)
{
    GetTimeRemaining();
}

float CallBack::GetTimeRemaining()
{

    return mSeconds;
}

