/*
 * AbstractPiece.h
 *
 *  Created on: 15 Dec 2013
 *      Author: bert
 */

#ifndef ABSTRACTPIECE_H_
#define ABSTRACTPIECE_H_

#include "TetrisCommon.h"

class AbstractPiece {
public:
	AbstractPiece(float x, float y, float z);
	virtual ~AbstractPiece();
	virtual void Reset(float x = 0.0, float y = 0.0, float z = 0.0);
	virtual void ConvertToCubes(std::vector<float> &cs, std::vector<unsigned short> &el);

	PieceArray getContainer() {
		return container;
	}

	float getIncX() const {
		return incX;
	}

	float getIncY() const {
		return incY;
	}

	float getIncZ() const {
		return incZ;
	}

	int getLeft() const {
		return left;
	}

	int getSideLength() const {
		return sideLength;
	}

	int getTop() const {
		return top;
	}

	float getX() const {
		return x;
	}

	float getY() const {
		return y;
	}

	float getZ() const {
		return z;
	}

protected:
	float x, y, z;
	float incX, incY, incZ;
	int left;
	int top;
	int sideLength;
	PieceArray container;

	virtual void CreateContainer()=0;
	int GetSmallestDistance(int wellRow, int pieceRowInWell, int currentDistanc);

private:
	void PushIntoVector(std::vector<float> &vector, PC &pc, float *normal, float* colour);
	void MakeElements(std::vector<unsigned short> &el, int numElements, int cubeNum);

};

#endif /* ABSTRACTPIECE_H_ */
