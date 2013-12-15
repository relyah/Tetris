/*
 * Well.cpp
 *
 *  Created on: 10 Dec 2013
 *      Author: bert
 */

#include "Well.h"

Well::Well(int maxCol, int maxRow, float x, float y, float z) : AbstractPiece(x,y,z)
{

	this->maxRow = maxRow;
	this->maxCol = maxCol;

	CreateContainer();
}

void Well::CreateContainer()
{
	container = PieceArray(maxCol);

		for (int c = 0; c < maxCol; c++) {
			container[c] = PieceRowArray(maxRow);
			for (int r = 0; r < maxRow; r++) {
				container[c][r] = false;
			}
		}
}

bool Well::CanAdd(Piece& other) {
	for (int c = 0; c < other.GetSize(); c++) {
		for (int r = 0; r < other.GetSize(); r++) {
			if (other.getContainer()[c][r] && this->container[other.getLeft() + c][other.getTop() + r])
				return false;
		}
	}
	return true;
}

void Well::Add(Piece& other) {
	for (int c = 0; c < other.GetSize(); c++) {

		for (int r = 0; r < other.GetSize(); r++) {
			if (!other.getContainer()[c][r])
				continue;

			this->container[other.getLeft() + c][other.getTop() + r] = true;
		}
	}

	other.RemoveFullRows();
}

bool Well::CanMove(Piece& other, int incCol, int incRow) {

	int otherMaxRowInWell = other.GetBottomRow() + other.getTop();
	if (otherMaxRowInWell >= this->maxRow - 1)
		return false;

	for (int row = other.GetSize() - 1; row >= 0; row--) {
		for (int col = other.GetSize() - 1; col >= 0; col--) {
			if (other.getContainer()[col][row]) {
				int testCol = col + other.getLeft() + incCol;
				if (testCol < 0 || testCol >= this->maxCol)
					return false;
				int testRow = row + other.getTop() + incRow;
				if (testRow < 0 || testRow >= this->maxRow)
					return false;
				if (this->container[testCol][testRow])
					return false;
			}
		}
	}
	return true;
}

void Well::Drop(Piece& other) {

	int dropDistance = maxRow;

	for (int row = other.GetSize() - 1; row >= 0; row--) {
		for (int col = other.GetSize() - 1; col >= 0; col--) {
			if (other.getContainer()[col][row]) {

				int pieceRowInWell = row + other.getTop();

				//get drop distance for empty well
				dropDistance = GetSmallestDistance(this->maxRow - 1, pieceRowInWell, dropDistance);

				//get drop distance when well contains pieces
				int wellCol = col + other.getLeft();
				for (int wellRow = this->maxRow - 1; wellRow >= 0; wellRow--) {
					//check for piece
					if (this->container[wellCol][wellRow]) {
						dropDistance = GetSmallestDistance(wellRow - 1, pieceRowInWell, dropDistance);
					}
				}
			}

		}
	}

	other.Move(0, dropDistance, true, true);
}

bool Well::CanRotateLeft(Piece& other) {
	for (int row = 0; row < other.GetSize(); row++) {
		for (int col = 0; col < other.GetSize(); col++) {
			if (other.getContainer()[col][row]) {
				int newRow = other.GetSize() - 1 - col;
				int newCol = row;

				if (!IsThereSpaceHere(newCol + other.getLeft(), newRow + other.getTop()))
					return false;
			}
		}

	}
	return true;

}

bool Well::CanRotateRight(Piece& other) {
	for (int row = 0; row < other.GetSize(); row++) {
		for (int col = 0; col < other.GetSize(); col++) {
			if (other.getContainer()[col][row]) {
				int rotatedRow = col;
				int rotatedCol = other.GetSize() - 1 - row;

				if (!IsThereSpaceHere(rotatedCol + other.getLeft(), rotatedRow + other.getTop()))
					return false;
			}
		}

	}
	return true;

}


bool Well::IsThereSpaceHere(int col, int row) {
	if (row < 0 || row >= maxRow)
		return false;
	if (col < 0 || col >= maxCol)
		return false;

	return !container[col][row];
}



