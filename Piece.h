/*
 * CurrentPiece.h
 *
 *  Created on: 10 Dec 2013
 *      Author: bert
 */

#ifndef PIECE_H_
#define PIECE_H_

#include <stdlib.h>
#include <math.h>

#include "AbstractPiece.h"

class Piece : public AbstractPiece {

public:
	Piece(int size, float x, float y, float z);
	virtual ~Piece();

	void Set(int col, int row, bool flag);

	bool MustMove();

	void RotateLeft();
	void RotateRight();

	void Increment(bool incX, bool incY, bool incZ);
	void Move(int incCol, int incRow, bool isAdjustXandY = false, bool isAdjustAbsolute = false);

	int GetBottomRow();
	void RemoveFullRows();

	int GetSize() {return size;}

protected:
	virtual void CreateContainer();

private:
	int size;

	void RemoveGaps();
};

#endif /* PIECE_H_ */
