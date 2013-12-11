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
}

CurrentPiece::~CurrentPiece() {

}

void CurrentPiece::ConvertToCubes() {

	int x = 0;
	for (PieceArray::iterator c = piece.begin(); c != piece.end(); ++c) {

		int y = -1;
		for (PieceRowArray::iterator r = (*c).begin(); r != (*c).end(); ++r) {
			y++;
			if (!(*r))
				continue;

			double f_tl_x = left*sideLength+x*sideLength;
			double f_tl_y = top*sideLength+y*sideLength;
			double f_tl_z = 1.0;

			double f_tr_x = f_tl_x+sideLength;
			double f_tr_y = f_tl_y;
			double f_tr_z = 1.0;

			double f_bl_x = f_tl_x;
			double f_bl_y = f_tl_y-sideLength;
			double f_bl_z = 1.0;

			double f_br_x = f_tr_x;
			double f_br_y = f_bl_y;
			double f_br_z = 1.0;

			double b_tl_x = f_tl_x;
			double b_tl_y = f_tl_y;
			double b_tl_z = -1.0;

			double b_tr_x = f_tr_x;
			double b_tr_y = f_tr_y;
			double b_tr_z = -1.0;

			double b_bl_x = f_bl_x;
			double b_bl_y = f_bl_y;
			double b_bl_z = -1.0;

			double b_br_x = f_tr_x;
			double b_br_y = f_bl_y;
			double b_br_z = -1.0;

		}
		x++;

	}

}

