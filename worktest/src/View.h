#pragma once

#include "Position.h"

#include <king/Engine.h>
#include <vector>
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
    void RunAction(std::shared_ptr<MoveTo> &action);
    void UpdateAction();
    
    Position ConvertToViewSpace(Position globalPosition);
    Position ConvertToWorldSpace(Position viewPosition);

	virtual void Render(King::Engine& engine);
    
    std::shared_ptr<MoveTo> mAction;
protected:
	Position mPosition;
	float mRotation;
    
private:
	std::vector<View*> mChildren;
};
