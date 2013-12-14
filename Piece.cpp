/*
 * CurrentPiece.cpp
 *
 *  Created on: 10 Dec 2013
 *      Author: bert
 */

#include "Piece.h"

Piece::Piece(int maxCol, int maxRow, float x, float y, float z) {
	incX = 0.0;
	incY = -0.2;
	incZ = 0.0;

	this->x = x;
	this->y = y;
	this->z = z;

	this->maxRow = maxRow;
	this->maxCol = maxCol;
	top = 0;
	left = 0;
	sideLength = 2;

	piece = PieceArray(maxCol);

	for (int c = 0; c < maxCol; c++) {
		piece[c] = PieceRowArray(maxRow);
		for (int r = 0; r < maxRow; r++) {
			piece[c][r] = false;
		}
	}

	//reset all pieces
//	for (PieceArray::iterator c = piece.begin(); c != piece.end(); ++c) {
//		for (PieceRowArray::iterator r = (*c).begin(); r != (*c).end(); ++r) {
//			*r = false;
//		}
//	}
}

Piece::~Piece() {

}

void Piece::Set(int col, int row, bool flag) {
	if (col < 0 || col >= this->maxCol)
		return;
	if (row < 0 || row >= this->maxRow)
		return;

	piece[col][row] = flag;
}

void Piece::ConvertToCubes(std::vector<float> &cs, std::vector<unsigned short> &el) {

	int numElements = 24;
	int cubeNum = 0;

	int col = 0;
	for (PieceArray::iterator c = piece.begin(); c != piece.end(); ++c) {

		int row = -1;
		for (PieceRowArray::iterator r = (*c).begin(); r != (*c).end(); ++r) {
			row++;
			if (!(*r))
				continue;

			PC f_bl = { (float) (left * sideLength + col * sideLength), (float) (-top * sideLength - (row + 1) * sideLength),
					1.0 };
			PC f_br = { f_bl.x + sideLength, f_bl.y, 1.0 };
			PC f_tl = { f_bl.x, f_bl.y + sideLength, 1.0 };
			PC f_tr = { f_br.x, f_tl.y, 1.0 };
			PC b_tl = { f_tl.x, f_tl.y, -1.0 };
			PC b_tr = { f_tr.x, f_tr.y, -1.0 };
			PC b_bl = { f_bl.x, f_bl.y, -1.0 };
			PC b_br = { f_tr.x, f_bl.y, -1.0 };

			//front
			PushIntoVector(cs, f_bl);
			PushIntoVector(cs, f_br);
			PushIntoVector(cs, f_tr);
			PushIntoVector(cs, f_tl);

			//top
			PushIntoVector(cs, f_tl);
			PushIntoVector(cs, f_tr);
			PushIntoVector(cs, b_tr);
			PushIntoVector(cs, b_tl);

			//back
			PushIntoVector(cs, b_br);
			PushIntoVector(cs, b_bl);
			PushIntoVector(cs, b_tl);
			PushIntoVector(cs, b_tr);

			//bottom
			PushIntoVector(cs, b_bl);
			PushIntoVector(cs, b_br);
			PushIntoVector(cs, f_br);
			PushIntoVector(cs, f_bl);

			//left
			PushIntoVector(cs, b_bl);
			PushIntoVector(cs, f_bl);
			PushIntoVector(cs, f_tl);
			PushIntoVector(cs, b_tl);

			//right
			PushIntoVector(cs, f_br);
			PushIntoVector(cs, b_br);
			PushIntoVector(cs, b_tr);
			PushIntoVector(cs, f_tr);

			MakeElements(el, numElements, cubeNum);

			cubeNum++; //increment the cube counter;

		}
		col++;

	}

}

bool Piece::CanAdd(Piece& other) {
	for (int c = 0; c < other.maxCol; c++) {
		for (int r = 0; r < other.maxRow; r++) {
			if (this->piece[other.left + c][other.top + r])
				return false;
		}
	}
	return true;
}
void Piece::Add(Piece& other) {
	for (int c = 0; c < other.maxCol; c++) {

		for (int r = 0; r < other.maxRow; r++) {
			if (!other.piece[c][r])
				continue;

			this->piece[other.left + c][other.top + r] = true;
		}
	}
}

void Piece::Increment(bool isIncX, bool isIncY, bool isIncZ) {
	x += (isIncX ? incX : 0.0);
	y += (isIncY ? incY : 0.0);
	z += (isIncZ ? incZ : 0.0);
}

bool Piece::CanMove(Piece& other, int incCol, int incRow) {

	int otherMaxRowInWell = other.GetBottomRow() + other.top;
	if (otherMaxRowInWell >= this->maxRow - 1)
		return false;

	for (int row = other.maxRow - 1; row >= 0; row--) {
		for (int col = other.maxCol - 1; col >= 0; col--) {
			if (other.piece[col][row]) {
				int testCol = col + other.left + incCol;
				if (testCol < 0 || testCol >= this->maxCol)
					return false;
				int testRow = row + other.top + incRow;
				if (testRow < 0 || testRow >= this->maxRow)
					return false;
				if (this->piece[testCol][testRow])
					return false;
			}
		}
	}
	return true;
}

bool Piece::MustMove(Piece& other) {
	float upperBound = -(float) (other.top * other.sideLength) - other.incY / 2.0;

	return other.y <= upperBound;
}

void Piece::Move(int incCol, int incRow, bool isAdjustXandY, bool isAdjustAbsolute) {
	top += incRow;
	left += incCol;

	if (isAdjustXandY) {
		if (isAdjustAbsolute) {
			x = (float)(left * sideLength);
			y = (float)(-top * sideLength);
		} else {
			x += (float) (incCol * sideLength);
			y += (float) (-incRow * sideLength);
		}
	}
}

void Piece::Drop(Piece& other) {

	int dropDistance = maxRow;

	for (int row = other.maxRow - 1; row >= 0; row--) {
		for (int col = other.maxCol - 1; col >= 0; col--) {
			if (other.piece[col][row]) {

				int pieceRowInWell = row + other.top;

				//get drop distance for empty well
				dropDistance = GetSmallestDistance(this->maxRow - 1, pieceRowInWell, dropDistance);

				//get drop distance when well contains pieces
				for (int wellRow = this->maxRow - 1; wellRow >= 0; wellRow--) {
					int wellCol = col + other.left;
					//check for piece
					if (this->piece[wellCol][wellRow]) {
						dropDistance = GetSmallestDistance(wellRow, pieceRowInWell, dropDistance);
					}
				}
			}

		}
	}

	other.Move(0, dropDistance, true,true);
}

int Piece::GetSmallestDistance(int wellRow, int pieceRowInWell, int currentDistance) {
	int newDD = wellRow - pieceRowInWell; //calculate distance
	if (newDD < currentDistance) {
		return newDD;
	} else {
		return currentDistance;
	}
}

int Piece::GetBottomRow() {
	for (int row = maxRow - 1; row >= 0; row--) {
		for (int col = maxCol - 1; col >= 0; col--) {
			if (piece[col][row])
				return row;
		}
	}
	return 0;
}

void Piece::PushIntoVector(std::vector<float> &cs, PC &pc) {
	cs.push_back(pc.x);
	cs.push_back(pc.y);
	cs.push_back(pc.z);
}

void Piece::MakeElements(std::vector<unsigned short> &el, int numElements, int cubeNum) {

	int offset = numElements * cubeNum;
//front
	el.push_back(0 + offset);
	el.push_back(1 + offset);
	el.push_back(2 + offset);
	el.push_back(2 + offset);
	el.push_back(3 + offset);
	el.push_back(0 + offset);
//top
	el.push_back(4 + offset);
	el.push_back(5 + offset);
	el.push_back(6 + offset);
	el.push_back(6 + offset);
	el.push_back(7 + offset);
	el.push_back(4 + offset);
//back
	el.push_back(8 + offset);
	el.push_back(9 + offset);
	el.push_back(10 + offset);
	el.push_back(10 + offset);
	el.push_back(11 + offset);
	el.push_back(8 + offset);
//bottom
	el.push_back(12 + offset);
	el.push_back(13 + offset);
	el.push_back(14 + offset);
	el.push_back(14 + offset);
	el.push_back(15 + offset);
	el.push_back(12 + offset);
//left
	el.push_back(16 + offset);
	el.push_back(17 + offset);
	el.push_back(18 + offset);
	el.push_back(18 + offset);
	el.push_back(19 + offset);
	el.push_back(16 + offset);
//rightunsigned
	el.push_back(20 + offset);
	el.push_back(21 + offset);
	el.push_back(22 + offset);
	el.push_back(22 + offset);
	el.push_back(23 + offset);
	el.push_back(20 + offset);

}

