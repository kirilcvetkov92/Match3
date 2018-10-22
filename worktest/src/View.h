#pragma once

#include "Position.h"

#include <king/Engine.h>
#include <queue>
#include "MoveTo.hpp"
class View {
public:
	View();
	virtual ~View();

	void SetPosition(Position position);

	void SetPosition(float x, float y);
	const Position& GetPosition() const;
    
	void SetRotation(const float rotation);

	void AddChild(View* view);
	void RemoveChild(View* view);

	void RemoveAllChildren();
    
    Position ConvertToViewSpace(Position globalPosition);
    Position ConvertToWorldSpace(Position viewPosition);

	virtual void Render(King::Engine& engine);
    virtual void RunMoveAction(std::shared_ptr<MoveTo> &action);
    virtual void UpdateMoveActions();
    std::queue<std::shared_ptr<MoveTo>> mMoveActions;
    std::shared_ptr<MoveTo> mCurrentMoveAction;
protected:

	Position mPosition;
	float mRotation;
    
private:
	std::vector<View*> mChildren;
};
