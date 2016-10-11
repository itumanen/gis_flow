#include "fa_grid.h"
#include <cstring>

int main(int argc, char** argv) {

	//check args and argc to make sure the user entered a  file
    //name and a value on the command line

    if (argc < 2) {
    	cout << "error, " << argc << " is an invalid number of inputs" << endl;
    	return(EXIT_FAILURE);
    }

    string elevName, fdName, faName; 
    Grid* elevGrid;
    FD_Grid* fdGrid;
    FA_Grid* faGrid;
    
    string outputFD = "flow_direction.txt";
    string outputFA = "flow_accumulation.txt";
    
    // Original grid
    elevGrid = new Grid(argv[1]);
    elevGrid->printInfo();
    elevGrid->printGrid();

    fdGrid = new FD_Grid(elevGrid);
    fdGrid->computeFlowDir(elevGrid);
    printf("\nFlow direction grid\n");
    fdGrid->printGrid();
    fdGrid->writeGridToFile(outputFD);

    faGrid = new FA_Grid(elevGrid);
    printf("\nFlow accumulation grid\n");
    faGrid->computeFlow(fdGrid);
    faGrid->printGrid();
    faGrid->writeGridToFile(outputFA);

    // delete memory from array
    elevGrid->freeGridData();
    fdGrid->freeGridData();
    faGrid->freeGridData();

	return(EXIT_SUCCESS);
}