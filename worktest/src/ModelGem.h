#pragma once

#include "Coordinate.h"

class ModelGem {
public:
	enum class Color {
		BLUE,
		GREEN,
		PURPLE,
		RED,
		YELLOW
	};
	
	enum class State {
		DROPPING,
		RESTING,
		SWAPPING,
		MATCHED
	};
	
	ModelGem(Color color);
	
	State mState;
	Color mColor;

	static Color GetRandomColor();
};
