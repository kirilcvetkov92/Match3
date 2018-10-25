#pragma once

#include <cstddef>

namespace Settings {
	static const size_t MODEL_GRID_WIDTH = 8;
	static const size_t MODEL_GRID_HEIGHT = 8;
	static const size_t MODEL_GRID_MATCH_LENGTH = 3;
	
	static const float VIEW_GRID_SPACING = 50.0f;
	
	static const float VIEW_GEM_DEBUG_LABEL_OFFSET = 20.0f;
    static const float SPEED = 9.81f*35;
    static const int GAME_COUNTER = 60;
    static const int START_COUNTER = 3;
    static const int NEIGHBOURS_DISTANCE = 1;
    static const int ROOF_OFFSET = 4;
    static const int GEM_ROOF_OFFSET = 2;
    
    static const bool DEBUG = false;
}
