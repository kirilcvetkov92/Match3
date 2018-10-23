#pragma once

#include "Coordinate.h"
#include <iostream>

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
        FALLING,
		RESTING,
		SWAPPING,
		MATCHED
	};
	
	ModelGem(Color color);
	
	State mState;
	Color mColor;

	static Color GetRandomColor();
    static Color GetRandomColor(std::vector<Color> restrictedColors);

    void toString();
};
