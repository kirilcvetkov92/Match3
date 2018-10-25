#include "ModelGem.h"

#include <vector>
#include <random>

ModelGem::ModelGem(Color color)
: mState(State::RESTING)
, mColor(color)
{}

ModelGem::Color ModelGem::GetRandomColor() {
	static std::vector<Color> COLORS = {
		Color::BLUE,
		Color::GREEN,
		Color::PURPLE,
		Color::RED,
		Color::YELLOW
	};
	static std::random_device randomDevice;
	static std::uniform_int_distribution<size_t> distribution(0, COLORS.size() - 1);

	size_t index = distribution(randomDevice);

	return COLORS[index];
    
}


ModelGem::Color ModelGem::GetRandomColor(std::vector<ModelGem::Color> restrictedColors) {
    static std::vector<Color> COLORS = {
        Color::BLUE,
        Color::GREEN,
        Color::PURPLE,
        Color::RED,
        Color::YELLOW
    };
    static std::random_device rd;
    static std::mt19937 g(rd());
    
    std::shuffle(COLORS.begin(), COLORS.end(), g);

    for(int i=0; i<COLORS.size(); i++)
    {
        bool found = (std::find(restrictedColors.begin(), restrictedColors.end(), COLORS[i]) != restrictedColors.end());
        if (!found)
            return COLORS[i];
    }
    return Color::RED;

}


