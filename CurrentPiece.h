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

#define MAXROW 5
#define MAXCOL 5

typedef std::array<bool, MAXROW> PieceRowArray;
typedef std::array<PieceRowArray, MAXCOL> PieceArray;


struct PC {
	float x;
	float y;
	float z;
};

class CurrentPiece {
	int left;
	int top;
	int sideLength;
	PieceArray  piece;
public:
	CurrentPiece();
	virtual ~CurrentPiece();

	void Set(int column, int row, bool flag);

	void ConvertToCubes(std::vector<float> &cs, std::vector<unsigned short> &el);

private:
	void PushIntoVector(std::vector<float> &vector, PC &pc);
	void MakeElements(std::vector<unsigned short> &el, int numElements, int cubeNum);
};

#endif /* CURRENTPIECE_H_ */
