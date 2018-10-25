#include "ModelGrid.h"

#include <king/Engine.h>
#include <king/Updater.h>
#include <string>
#include <cassert>
#include <algorithm>
#include "Settings.h"

ModelGrid::ModelGrid(size_t width, size_t height, size_t matchLength)
: mTransitions()
, mWidth(width)
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

void ModelGrid::TryMatch(Coordinate gemFrom, Coordinate gemTo)
{
    size_t maxDistance = Settings::NEIGHBOURS_DISTANCE;
    std::pair<int, int> neighbours[] = {{maxDistance,0}, {0,maxDistance}, {-maxDistance,0}, {0,-maxDistance}};
    size_t minDistance = fabs((int)gemFrom.mX-(int)gemTo.mX)+fabs((int)gemFrom.mY-(int)gemTo.mY);
    Coordinate nearestNeighbour = gemTo;
    
    for (auto neighbour : neighbours)
    {
        int mx = (int)gemFrom.mX+neighbour.first;
        int my = (int)gemFrom.mY+neighbour.second;
        Coordinate gemNeighbourPosition = Coordinate(mx, my);
        if(mGems.count(gemNeighbourPosition))
        {
            int distanceX = gemNeighbourPosition.mX-gemTo.mX;
            int distanceY = gemNeighbourPosition.mY-gemTo.mY;
            size_t distance = fabs(distanceX)+fabs(distanceY);
            if(distance<minDistance)
            {
                minDistance=distance;
                nearestNeighbour = gemNeighbourPosition;
            }
        }
    }
    
    gemTo = nearestNeighbour;
    
    if(!mGems.count(gemFrom) || !mGems.count(gemTo))
        return;
    
    std::shared_ptr<ModelGem> modelGemFrom = mGems[gemFrom];
    std::shared_ptr<ModelGem> modelGemTo = mGems[gemTo];
    
    
    if(modelGemFrom->mState!=ModelGem::State::RESTING || modelGemTo->mState!=ModelGem::State::RESTING)
        return;
    
    mGems[gemTo]=modelGemFrom;
    mGems[gemFrom]=modelGemTo;
    
    Position gemFromP = Position(gemFrom.mX, gemFrom.mY);
    Position gemToP = Position(gemTo.mX, gemTo.mY);

    std::vector<std::weak_ptr<ModelGem>> matchedGems = FindMatchedGems();
    
    //Todo (Naive search, not efficient)
    bool found = false;
    for(auto &gem : matchedGems)
    {
        if(auto gem_ptr = gem.lock())
        {
            if(modelGemFrom==gem_ptr || gem_ptr == modelGemTo)
            {
                mTransitions.insert({modelGemFrom, {gemFromP, gemToP}});
                mTransitions.insert({modelGemTo, {gemToP, gemFromP}});
                modelGemFrom->mState=ModelGem::State::SWAPPING;
                modelGemTo->mState=ModelGem::State::SWAPPING;
                found=true;
            }
        }
    }
    if(!found)
    {
        // swap back
        mGems[gemTo]=modelGemTo;
        mGems[gemFrom]=modelGemFrom;
        modelGemFrom->mState=ModelGem::State::SWAPPING;
        modelGemTo->mState=ModelGem::State::SWAPPING;
        
        mTransitions.insert({modelGemFrom, {gemFromP, gemToP}});
        mTransitions.insert({modelGemTo, {gemToP, gemFromP}});
        mTransitions.insert({modelGemFrom, {gemToP, gemFromP}});
        mTransitions.insert({modelGemTo, {gemFromP, gemToP}});
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

std::unordered_map<Position, Position> & ModelGrid::GetRoof() 
{
    return  mRoof;
}

const std::multimap<std::shared_ptr<ModelGem>, std::pair<Position,Position>>& ModelGrid::GetTransitions() const {
    return mTransitions;
}

void ModelGrid::ClearTransitions() {
    return mTransitions.clear();
}
void ModelGrid::Initialise() {
	mGems.clear();
    GenerateGemsOnTop();
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
        
        if(gem->mState == ModelGem::State::MATCHED || !gem)
        {
            for (int i=coordinate.mY; i>0; i--)
            {
                Coordinate currentCoordinate(coordinate.mX, i-1);
                if(mGems.count(currentCoordinate))
                {
                    auto iterator = mGems.find(currentCoordinate);
                    auto &candidateDropGem = iterator->second;;
                    if(candidateDropGem->mState == ModelGem::State::RESTING ||
                       candidateDropGem->mState == ModelGem::State::FALLING)
                    {

                        result.push_back(candidateDropGem);
                    }
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
                gem->mState = ModelGem::State::FALLING;
                for(size_t currentRow=row; currentRow<mHeight; currentRow++)
                {
                    Coordinate dropCoordinate(column, currentRow);
                    Coordinate nextCoordinate(column, currentRow+1);

                    auto dropIterator = mGems.find(nextCoordinate);
                    if(dropIterator != mGems.end() || currentRow+1==mHeight)
                    {
                        
                        mGems.erase(dropCoordinate);
                        mGems.erase(coordinate);
                        mGems.insert({dropCoordinate, gem});
                    
                        Position dropCoordinateP(column, currentRow);
                        Position coordinateP(column,row-1);
                        
                        //Insert to transition multimap
                        mTransitions.insert({gem, {coordinateP, dropCoordinateP}});
                        break;
                    }
                }
            }
        }
    }
}

void ModelGrid::GenerateGemsOnTop()
{
    for(size_t column = 0; column<mWidth; column++) {
        for(size_t row = mHeight; row>0; row--) {
            const auto &gem = getGem(column, row-1);
            std::vector<ModelGem::Color> restrictedColors;
            if (!gem)
            {
                if (column>=mMatchLength-1)
                {
                    int currentCol=column-1;
                    const auto &horizontalNeighbour = getGem(currentCol, row-1);
                    
                    ModelGem::Color color = horizontalNeighbour->mColor;
                    restrictedColors.push_back(color);
                    int limit = column-mMatchLength+1;
                    for(; currentCol>=limit; currentCol--)
                    {
                        const auto &horizontalNeighbour = getGem(currentCol, row-1);
                        if(horizontalNeighbour->mColor!=color)
                        {
                            restrictedColors.pop_back();
                            break;
                        }
                    }
                }
                if (row<=mHeight-mMatchLength+1)
                {
                    int currentRow=row;
                    const auto &horizontalNeighbour = getGem(column, currentRow);
                    
                    ModelGem::Color color = horizontalNeighbour->mColor;
                    restrictedColors.push_back(color);
                    for(; currentRow<row+mMatchLength-1; currentRow++)
                    {
                        const auto &horizontalNeighbour = getGem(column, currentRow);
                        if(horizontalNeighbour->mColor!=color)
                        {
                            restrictedColors.pop_back();
                            break;
                        }
                    }
                }

            auto modelGem = std::make_shared<ModelGem>(ModelGem::GetRandomColor(restrictedColors));
              modelGem->mState = ModelGem::State::FALLING;

            int roofRow=-4;
            int t=-4;
            
            for(int i=0; i<100;i++)
            {
                if(mRoof.count(Position(column,roofRow)))
                {
                    t=fmin(t,roofRow);
                }
                roofRow-=2;
            }
                roofRow=t-2;
            
            mGems.insert({
                Coordinate(column, row-1),
                modelGem
            });

            float sourceX = (float)column;
            float sourceY = (float)roofRow;
            float destinationX = (float)column;
            float destinationY = (float)row-1;
            
            mTransitions.insert({modelGem, {Position(sourceX, sourceY), Position(destinationX, destinationY)}});
            
                

            if(mRoof.count(Position(destinationX, destinationY)))
            {
                Position k = mRoof.find(Position(destinationX, destinationY))->second;
                mRoof.erase(k);
                mRoof.erase(Position(destinationX, destinationY));
           
            }
            
            mRoof.insert({Position(sourceX, sourceY), Position(destinationX, destinationY)});
            mRoof.insert({Position(destinationX, destinationY), Position(sourceX, sourceY)});
        }
        }
    }
}


std::shared_ptr<ModelGem> ModelGrid::getGem(size_t column, size_t row) const
{
    Coordinate coordinate(column, row);
    auto iterator = mGems.find(coordinate);
    if(iterator != mGems.end())
        return iterator->second;
    else return nullptr;
}


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
                (neighbor->second->mColor == color && neighbor->second->mState==ModelGem::State::RESTING)) {
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

