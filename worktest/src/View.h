#pragma once

#include "Position.h"

#include <king/Engine.h>
#include <vector>

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

	virtual void Render(King::Engine& engine);

protected:
	Position mPosition;
	float mRotation;
	
private:
	std::vector<View*> mChildren;
};
