#include "View.h"

#include <king/Updater.h>
#include <algorithm>
#include <string>
#include <iostream>

View::View()
: mPosition(0.0, 0.0)
, mRotation(0.0f),
mVisibility(true)
{}

View::~View()
{}

void View::SetPosition(Position position) {
    if(!mCurrentMoveAction)
    {
        mPosition = position;
    }
}

void View::SetPosition(float x, float y) {
	mPosition.mX = x;
	mPosition.mY = y;
}

const Position& View::GetPosition() const {
	return mPosition;
}

void View::SetRotation(const float rotation) {
	mRotation = rotation;
}

void View::AddChild(View* view) {
	mChildren.push_back(view);
}

void View::RemoveChild(View* view) {
	auto removal = std::remove(mChildren.begin(), mChildren.end(), view);
	mChildren.erase(removal, mChildren.end());
}

void View::RemoveAllChildren() {
	mChildren.clear();
}

void View::RunMoveAction(std::shared_ptr<MoveTo> &action){
    mMoveActions.push(action);
    if(!mMoveActions.empty() && !mCurrentMoveAction)
    {
        mCurrentMoveAction = mMoveActions.front();
        mMoveActions.pop();
        mCurrentMoveAction->setSource(mPosition);
    }
}

void View::UpdateMoveActions()
{
    
    if(!mMoveActions.empty() && !mCurrentMoveAction)
    {
        mCurrentMoveAction = mMoveActions.front();
        mMoveActions.pop();
        mCurrentMoveAction->setSource(mPosition);
    }
    
    if (mCurrentMoveAction)
    {
        mCurrentMoveAction->Update();
        mPosition = mCurrentMoveAction->mCurrentPosition;
        if(mCurrentMoveAction->mState == Action::State::FINISHED)
        {
            mPosition = mCurrentMoveAction->mDestination;
            mCurrentMoveAction = nullptr;
        }
    }
}

void View::setVisible(bool visible)
{
    mVisibility = visible;
}

void View::Render(King::Engine& engine) {
    for(auto* child : mChildren) {
        if(child->mVisibility)
        {
            child->Render(engine);
        }
    }
}

Position View::ConvertToViewSpace(Position globalPosition)
{
    return Position(globalPosition.mX-mPosition.mX, globalPosition.mY-mPosition.mY);
}

Position View::ConvertToWorldSpace(Position viewPosition)
{
    return Position(viewPosition.mX+mPosition.mX, viewPosition.mY+mPosition.mY);
}



