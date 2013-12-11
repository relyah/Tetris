/*
 * CurrentPiece.cpp
 *
 *  Created on: 10 Dec 2013
 *      Author: bert
 */

#include "CurrentPiece.h"

CurrentPiece::CurrentPiece() {
	top = 0;
	left = 0;
	sideLength = 10;

	//reset all pieces
	for (PieceArray::iterator c = piece.begin(); c != piece.end(); ++c) {
		for (PieceRowArray::iterator r = (*c).begin(); r != (*c).end(); ++r) {
			*r = false;
		}
	}
}

CurrentPiece::~CurrentPiece() {

}

void CurrentPiece::Set(int col, int row, bool flag) {
	if (col < 0 || col >= MAXCOL)
		return;
	if (row < 0 || row >= MAXROW)
		return;

	piece[col][row] = flag;
}

void CurrentPiece::ConvertToCubes(std::vector<float> &cs, std::vector<unsigned short> &el) {

	int numElements = 24;
	int cubeNum = 0;

	int x = 0;
	for (PieceArray::iterator c = piece.begin(); c != piece.end(); ++c) {

		int y = -1;
		for (PieceRowArray::iterator r = (*c).begin(); r != (*c).end(); ++r) {
			y++;
			if (!(*r))
				continue;

			PC f_tl = { (double) (left * sideLength + x * sideLength), (double) (top * sideLength + y * sideLength), 1.0 };
			PC f_tr = { f_tl.x + sideLength, f_tl.y, 1.0 };
			PC f_bl = { f_tl.x, f_tl.y - sideLength, 1.0 };
			PC f_br = { f_tr.x, f_bl.y, 1.0 };
			PC b_tl = { f_tl.x, f_tl.y, -1.0 };
			PC b_tr = { f_tr.x, f_tr.y, -1.0 };
			PC b_bl = { f_bl.x, f_bl.y, -1.0 };
			PC b_br = { f_tr.x, f_bl.y, -1.0 };

			//front
			PushIntoVector(cs, f_bl); // cs.push_back(f_bl.x); 	cs.push_back(f_bl.y); 	cs.push_back(f_bl.z);
			PushIntoVector(cs, f_br); // cs.push_back(f_br.x); 	cs.push_back(f_br.y); 	cs.push_back(f_br.z);
			PushIntoVector(cs, f_tr); //cs.push_back(f_tr.x); 	cs.push_back(f_tr.y); 	cs.push_back(f_tr.z);
			PushIntoVector(cs, f_tl); //cs.push_back(f_tl.x); 	cs.push_back(f_tl.y); 	cs.push_back(f_tl.z);

			//top
			PushIntoVector(cs, f_tl); //cs.push_back(f_tl.x); 	cs.push_back(f_tl.y); 	cs.push_back(f_tl.z);
			PushIntoVector(cs, f_tr); //cs.push_back(f_tr.x); 	cs.push_back(f_tr.y); 	cs.push_back(f_tr.z);
			PushIntoVector(cs, b_tr); //cs.push_back(b_tr.x); 	cs.push_back(b_tr.y); 	cs.push_back(b_tr.z);
			PushIntoVector(cs, b_tl); //cs.push_back(b_tl.x); 	cs.push_back(b_tl.y); 	cs.push_back(b_tl.z);

			//back
			PushIntoVector(cs, b_br); //cs.push_back(b_br.x); 	cs.push_back(b_br.y); 	cs.push_back(b_br.z);
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
		x++;

	}

}

void CurrentPiece::PushIntoVector(std::vector<float> &cs, PC &pc) {
	cs.push_back(pc.x);
	cs.push_back(pc.y);
	cs.push_back(pc.z);

}

void CurrentPiece::MakeElements(std::vector<unsigned short> &el, int numElements, int cubeNum) {

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
	//right
	el.push_back(20 + offset);
	el.push_back(21 + offset);
	el.push_back(22 + offset);
	el.push_back(22 + offset);
	el.push_back(23 + offset);
	el.push_back(20 + offset);

}

