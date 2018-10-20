#pragma once

#include "Coordinate.h"
#include "ModelGem.h"

#include <memory>
#include <vector>
#include <unordered_map>

class ModelGrid {
public:
	ModelGrid(size_t width, size_t height, size_t matchLength);
	void Match();
    void Drop();//kiril
    void RemoveMatchedGems(); //kiril
    void MoveDroppedGems(); // kiril
    void GenerateGemsOnTop(); //kiril
	const std::unordered_map<Coordinate, std::shared_ptr<ModelGem>>& GetGems() const;

private:
	void Initialise();
	std::vector<std::weak_ptr<ModelGem>> FindMatchedGems() const;
    std::vector<std::weak_ptr<ModelGem>> FindDroppingGems() const; //kiril
    std::vector<std::weak_ptr<ModelGem>> GenerateGemsWithDifficulty(size_t difficulty, size_t row, size_t column) const; //kiril
    std::shared_ptr<ModelGem> getGem(size_t column, size_t row) const; 

	static bool IsCoordinatePartOfMatch(
		const std::unordered_map<Coordinate, std::shared_ptr<ModelGem>>& gems,
		Coordinate coordinate,
		size_t width,
		size_t height,
		size_t matchLength);
	
	std::unordered_map<Coordinate, std::shared_ptr<ModelGem>> mGems;
	
	size_t mWidth;
	size_t mHeight;
	size_t mMatchLength;
};