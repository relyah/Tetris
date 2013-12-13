/*
 * CurrentPiece.h
 *
 *  Created on: 10 Dec 2013
 *      Author: bert
 */

#ifndef PIECE_H_
#define PIECE_H_

#include <stdlib.h>
#include <vector>

typedef std::vector<bool> PieceRowArray;
typedef std::vector<PieceRowArray> PieceArray;

struct PC {
	float x;
	float y;
	float z;
};

class Piece {

	float x, y, z;
	float incX, incY, incZ;
	int maxRow, maxCol;
	int left;
	int top;
	int sideLength;
	PieceArray piece;

public:
	Piece(int maxCol, int maxRow, float x, float y, float z);
	virtual ~Piece();

	void Set(int column, int row, bool flag);

	void ConvertToCubes(std::vector<float> &cs, std::vector<unsigned short> &el);

	void Add(Piece other);
	bool CanMove(Piece other);
	bool MustMove(Piece other);
	void Increment(bool incX, bool incY, bool incZ);
	void Move(int incCol, int incRow);

	float X() {
		return x;
	}
	float Y() {
		return y;
	}
	float Z() {
		return z;
	}

private:
	int GetBottomRow();
	void PushIntoVector(std::vector<float> &vector, PC &pc);
	void MakeElements(std::vector<unsigned short> &el, int numElements, int cubeNum);
};

#endif /* PIECE_H_ */