#pragma once

#include "Coordinate.h"
#include "ModelGem.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <map>
#include "MoveTo.hpp"


class ModelGrid {
public:
	ModelGrid(size_t width, size_t height, size_t matchLength);
	void Match();
    void Drop();
    void RemoveMatchedGems();
    void MoveDroppedGems();
    void GenerateGemsOnTop();
    void ClearTransitions();

	const std::unordered_map<Coordinate, std::shared_ptr<ModelGem>>& GetGems() const;
    const std::multimap<std::shared_ptr<ModelGem>, std::pair<Position,Position>>& GetTransitions() const;
    std::unordered_map<Position, Position>& GetRoof() ;

    void TryMatch(Coordinate gemFrom, Coordinate gemTo);
    void RemoveFromRoof(Position &position);
    void InsertToRoof(Position &sourcePosition, Position &destinationPosition);

    
private:
	void Initialise();
	std::vector<std::weak_ptr<ModelGem>> FindMatchedGems() const;
    std::vector<std::weak_ptr<ModelGem>> FindDroppingGems() const; //kiril
    std::shared_ptr<ModelGem> getGem(size_t column, size_t row) const; 

	static bool IsCoordinatePartOfMatch(
		const std::unordered_map<Coordinate, std::shared_ptr<ModelGem>>& gems,
		Coordinate coordinate,
		size_t width,
		size_t height,
		size_t matchLength);
	
	std::unordered_map<Coordinate, std::shared_ptr<ModelGem>> mGems;
    std::unordered_map<Position, Position> mRoof;
    std::multimap<std::shared_ptr<ModelGem>, std::pair<Position,Position>> mTransitions;

	size_t mWidth;
	size_t mHeight;
	size_t mMatchLength;
};
