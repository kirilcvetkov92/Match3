#include "View.h"

#include <king/Updater.h>
#include <algorithm>
#include <string>

View::View()
: mPosition(0.0, 0.0)
, mRotation(0.0f)
{}

View::~View()
{}

void View::SetPosition(Position position) {
	mPosition = position;
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

void View::RunAction(std::shared_ptr<MoveTo> &action){
    action->setSource(mPosition);
    mAction = action;
}

void View::UpdateAction()
{
    if (mAction)
    {
        mAction->Update();
        mPosition = mAction->mCurrentPosition;
        if(mAction->mState==King::Action::State::FINISHED)
        {
            mAction = nullptr;
        }
    }
    
}

void View::Render(King::Engine& engine) {
	for(auto* child : mChildren) {
		child->Render(engine);
	}
}
