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

void ModelGrid::Drop() {
    std::vector<std::weak_ptr<ModelGem>> droppingGems = FindDroppingGems();
    for (auto& gem : droppingGems) {
        if (auto gemPtr = gem.lock()) {
            gemPtr->mState = ModelGem::State::DROPPING;
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

std::vector<std::weak_ptr<ModelGem>> ModelGrid::FindDroppingGems() const {
    std::vector<std::weak_ptr<ModelGem>> result;
    for (auto iterator : mGems) {
        auto& coordinate = iterator.first;
        auto& gem = iterator.second;
        
        if(gem->mState == ModelGem::State::MATCHED)
        {
            for (int i=coordinate.mY; i>0; i--)
            {
                Coordinate currentCoordinate(coordinate.mX, i-1);
                auto iterator = mGems.find(currentCoordinate);
                auto &candidateDropGem = iterator->second;
                if(candidateDropGem->mState == ModelGem::State::RESTING)
                {
                    result.push_back(candidateDropGem);
                }
            }
        }
    }
    return result;
}


void ModelGrid::MoveDroppedGems()
{
    for(size_t row=mHeight; row>0; row--)
    {
        for(size_t column=0; column<mWidth; column++)
        {
            Coordinate coordinate(column,row-1);
            auto iterator = mGems.find(coordinate);

            if(iterator == mGems.end())
                continue;

            std::shared_ptr<ModelGem> gem = iterator->second;
            if (gem->mState == ModelGem::State::DROPPING){
                for(size_t currentRow=row; currentRow<mHeight; currentRow++)
                {
                    Coordinate dropCoordinate(column, currentRow);
                    Coordinate nextCoordinate(column, currentRow+1);

                    auto dropIterator = mGems.find(nextCoordinate);
                    if(dropIterator != mGems.end() || currentRow+1==mHeight)
                    {
                        mGems.erase(coordinate);
                        mGems.insert({dropCoordinate, gem});
                        break;
                    }
                }
            }
        }
    }
}


void ModelGrid::GenerateGemsOnTop()
{
    for(size_t column = 0; column < mWidth; column++) {
        for(size_t row = 0; row < mHeight; row++) {
            Coordinate coordinate(column,row);
            auto iterator = mGems.find(coordinate);
            if(iterator == mGems.end())
            {
                std::shared_ptr<ModelGem> newGem = std::make_shared<ModelGem>(ModelGem::GetRandomColor());
                newGem->mState = ModelGem::State::MATCHED;
                mGems.insert({
                    coordinate,
                    newGem
                });
            }
        }
    }
}


//std::vector<Coordinate> ModelGrid::GenerateGemsOnTop()
//{
//    std::vector<Coordinate> newGemCoordinates;
//    size_t maximumHeight = 0;
//    //Find the  (width+y)-coordinate for new games on the floor
//    for(size_t column = 0; column < mWidth; column++) {
//        for(size_t row = 0; row < mHeight; row++) {
//            Coordinate coordinate(column,row);
//            auto iterator = mGems.find(coordinate);
//            if(iterator == mGems.end())
//            {
//                size_t currentHeight = coordinate.mY;
//                if(maximumHeight<currentHeight)
//                {
//                    maximumHeight = currentHeight;
//                }
//                newGemCoordinates.push_back(coordinate);
//            }
//        }
//    }
//   // place the gems
//    std::vector<Coordinate> newGemCoordinates;
//
//    for (auto coordinate : newGemCoordinates){
//        std::shared_ptr<ModelGem> newGem = std::make_shared<ModelGem>(ModelGem::GetRandomColor());
//        newGem->mState = ModelGem::State::MATCHED;
//        Coordinate topCoordinate(coordinate.mX, coordinate.mY-(maximumHeight+1));
//        mGems.insert({
//            topCoordinate,
//            newGem
//        });
//    }
//
//}

void ModelGrid::RemoveMatchedGems()
{
  
    for (auto iterator = mGems.cbegin(); iterator != mGems.cend();) {
        auto& gem = iterator->second;
        if (gem->mState == ModelGem::State::MATCHED){
            mGems.erase(iterator++);
        }
        else{
            ++iterator;
        }
    }
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

