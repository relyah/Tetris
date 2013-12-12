/*
 * CurrentPiece.h
 *
 *  Created on: 10 Dec 2013
 *      Author: bert
 */

#ifndef CURRENTPIECE_H_
#define CURRENTPIECE_H_

#include <stdlib.h>
#include <vector>


typedef std::vector<bool> PieceRowArray;
typedef std::vector<PieceRowArray> PieceArray;


struct PC {
	float x;
	float y;
	float z;
};

class CurrentPiece {
	int row, col;
	int left;
	int top;
	int sideLength;
	PieceArray  piece;
public:
	CurrentPiece(int col, int row);
	virtual ~CurrentPiece();

	void Set(int column, int row, bool flag);

	void ConvertToCubes(std::vector<float> &cs, std::vector<unsigned short> &el);

	void Add(CurrentPiece other);

private:
	void PushIntoVector(std::vector<float> &vector, PC &pc);
	void MakeElements(std::vector<unsigned short> &el, int numElements, int cubeNum);
};

#endif /* CURRENTPIECE_H_ */
