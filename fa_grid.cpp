#include "fa_grid.h"

/* FLOW ACCUMULATION GRID METHODS */

FA_Grid::FA_Grid(Grid* elevGrid) {

	this->numCols = elevGrid->getNumCols();
	this->numRows = elevGrid->getNumRows();

	this->xllCorner = elevGrid->getXllCorner();
	this->yllCorner = elevGrid->getYllCorner();

	this->cellSize = elevGrid->getCellSize();

	this->minVal = elevGrid->getMinVal();
	this->maxVal = elevGrid->getMaxVal();

	this->nodata_value = elevGrid->getNodataValue();

	// allocate array
	this->gridVals = (float**) malloc(sizeof(float*) * this->numRows);
	assert(this->gridVals);
	for (int i = 0; i < this->numRows; i++) {
		this->gridVals[i] = (float*) malloc(sizeof(float) * this->numCols);
		assert(this->gridVals[i]);
	}

	this->initializeGrid(elevGrid);

}

void FA_Grid::initializeGrid(Grid* elevGrid) {

	for (int i = 0; i < this->numRows; i++) {
		for (int j = 0; j < this->numCols; j++) {

			if (this->getGridValueAt(i,j) != this->nodata_value) {
				this->setGridValueAt(i, j, INITIAL_VALUE);
				continue;
			}
			
			this->setGridValueAt(i, j, this->nodata_value);
		}
	}
}

float FA_Grid::computeFlowAt(Grid* fdGrid, int i, int j) {

	// Base case


	for (int row = -1; row < 2; row++) {
		if (row + i < 0 || row + i > this->numRows - 1) {
			cout << "row out of range" << endl;
			continue;
		}
		for (int col = -1; col < 2; col++) {
			if (col + j < 0 || row + j > this->numCols - 1 || (row + i == 0 && col + j == 0)) {
				cout << "col out of range " << endl;
				continue;
			}
			if (this->flowsInto(fdGrid, i + row, j + col, row, col)) {
				// recurse on this cell to compute total flow
				float value = this->getGridValueAt(i,j) + this->computeFlowAt(fdGrid, i + row, j + col);
				this->setGridValueAt(i, j, value);
				cout << "set grid value to " << value << endl;
				// this->setGridValueAt(i, j, this->getGridValueAt(i, j) + this->computeFlowAt(fdGrid, i + row, j + col));
			}
		}
	}

	// return accumulated flow value; returns 1 if nothing flows in
	return this->getGridValueAt(i, j);
}

void FA_Grid::computeFlow(Grid* fdGrid) {
	for (int row = 0; row < this->numRows - 1; row++) {
		for (int col = 0; col < this->numCols; col++) {
			this->setGridValueAt(0, 0, this->computeFlowAt(fdGrid, row, col));
		}
	}
}

bool FA_Grid::flowsInto(Grid* fdGrid, int i, int j, int offsetX, int offsetY) {
	if (fdGrid->getGridValueAt(i, j) == WEST && offsetX == 0 && offsetY == 1) {
		return true;
	}
	if (fdGrid->getGridValueAt(i, j) == NORTHWEST && offsetX == 1 && offsetY == 1) {
		return true;
	}
	if (fdGrid->getGridValueAt(i, j) == NORTH && offsetX == 1 && offsetY == 0) {
		return true;
	}
	if (fdGrid->getGridValueAt(i, j) == NORTHWEST && offsetX == 1 && offsetY == -1) {
		return true;
	}
	if (fdGrid->getGridValueAt(i, j) == EAST && offsetX == 0 && offsetY == -1) {
		return true;
	}
	if (fdGrid->getGridValueAt(i, j) == SOUTHEAST && offsetX == -1 && offsetY  == -1) {
		return true;
	}
	if (fdGrid->getGridValueAt(i, j) == SOUTH && offsetX == -1 && offsetY  == 0) {
		return true;
	}
	if (fdGrid->getGridValueAt(i, j) == SOUTHEAST && offsetX == -1 && offsetY  == 1) {
		return true;
	}
	
	cout << "returning no flow" << endl;
	return NO_FLOW;
}