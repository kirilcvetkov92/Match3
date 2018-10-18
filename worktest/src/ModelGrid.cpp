#include "ModelGrid.h"

#include <king/Engine.h>
#include <king/Updater.h>
#include <string>
#include <cassert>
#include <algorithm>


ModelGrid::ModelGrid(size_t width, size_t height, size_t matchLength)
: mWidth(width)
, mHeight(height)
, mMatchLength(matchLength)
{
	Initialise();
}

void ModelGrid::Match() {
	std::vector<std::weak_ptr<ModelGem>> matchedGems = FindMatchedGems();
	for (auto& gem : matchedGems) {
		if (auto gemPtr = gem.lock()) {
			gemPtr->mState = ModelGem::State::MATCHED;
		}
	}
}

const std::unordered_map<Coordinate, std::shared_ptr<ModelGem>>& ModelGrid::GetGems() const {
	return mGems;
}

void ModelGrid::Initialise() {
	mGems.clear();
	
	for(size_t column = 0; column < mWidth; column++) {
		for(size_t row = 0; row < mHeight; row++) {
			mGems.insert({
				Coordinate{ column, row },
				std::make_unique<ModelGem>(ModelGem::GetRandomColor())
			});
		}
	}
}

std::vector<std::weak_ptr<ModelGem>> ModelGrid::FindMatchedGems() const {
	std::vector<std::weak_ptr<ModelGem>> result;
	for (auto iterator : mGems) {
		auto& coordinate = iterator.first;
		auto& gem = iterator.second;
		if ((gem->mState == ModelGem::State::RESTING) &&
			IsCoordinatePartOfMatch(mGems, coordinate, mWidth, mHeight, mMatchLength)) {
			result.push_back(gem);
		}
	}
	return result;
}

bool ModelGrid::IsCoordinatePartOfMatch(
	const std::unordered_map<Coordinate, std::shared_ptr<ModelGem>>& gems,
	Coordinate coordinate,
	size_t width,
	size_t height,
	size_t matchLength) {
	auto iterator = gems.find(coordinate);
	if (iterator == gems.end()) {
		return false;
	}
	
	enum class Axis {
		COLUMN,
		ROW
	};
	
	static auto checkRun = [](
		const std::unordered_map<Coordinate, std::shared_ptr<ModelGem>>& gems,
		Coordinate coordinate,
		size_t matchLength,
		ModelGem::Color color,
		Axis axis,
		size_t axisLimit) {
		int padding = std::max(0, static_cast<int>(matchLength) - 1);
		size_t axisPosition = (axis == Axis::COLUMN) ? coordinate.mX : coordinate.mY;
		
		size_t start = std::max(static_cast<int>(axisPosition) - padding, 0);
		size_t end = std::min(static_cast<int>(axisPosition) + padding, static_cast<int>(axisLimit) - 1);
		
		size_t run = 0;
		
		for(size_t position = start; position <= end; position++) {
			auto neighbor = gems.find(
				(axis == Axis::COLUMN) ?
				Coordinate(position, coordinate.mY) :
				Coordinate(coordinate.mX, position));
			if ((neighbor != gems.end()) &&
				(neighbor->second->mColor == color)) {
				run++;
				if (run >= matchLength) {
					return true;
				}
			} else {
				run = 0;
			}
		}
		return false;
	};
	
	return (
		checkRun(gems, coordinate, matchLength, iterator->second->mColor, Axis::COLUMN, width) ||
		checkRun(gems, coordinate, matchLength, iterator->second->mColor, Axis::ROW, height));
}
