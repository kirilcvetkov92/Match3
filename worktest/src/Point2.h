#pragma once
#include <cstddef>
#include <utility>

template <typename TDimension>
class Point2 {
public:
	Point2(TDimension x, TDimension y)
	: mX(x)
	, mY(y)
	{}
	
	bool operator==(const Point2<TDimension>& other) const {
		return (mX == other.mX) && (mY == other.mY);
	}

	TDimension mX;
	TDimension mY;
};

namespace {
	template<typename T>
	static void CumulativeHash(std::size_t& hash, T const& key) {
		std::hash<T> hasher;
		hash ^= hasher(key) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	}
}

namespace std {
	template<typename T>
	struct hash<Point2<T>> {
		std::size_t operator()(Point2<T> const& value) const {
			std::size_t result(0);
			CumulativeHash(result, value.mX);
			CumulativeHash(result, value.mY);
			return result;
		}
	};
}
