/*
 * CurrentPiece.h
 *
 *  Created on: 10 Dec 2013
 *      Author: bert
 */

#ifndef CURRENTPIECE_H_
#define CURRENTPIECE_H_

#include <stdlib.h>
#include <array>
#include <vector>

typedef std::array<bool, 5> PieceRowArray;
typedef std::array<PieceRowArray, 5> PieceArray;

class CurrentPiece {
	int left;
	int top;
	int sideLength;
	std::vector<int> x;
	PieceArray  piece;
public:
	CurrentPiece();
	virtual ~CurrentPiece();

	void ConvertToCubes();
};

#endif /* CURRENTPIECE_H_ */
