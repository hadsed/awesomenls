/*
 * main.c
 *
 * This is the main file. All input parameters are read in
 * and defined via the input file. Grid object is instantiated
 * and computation routines are called.
 *
 * Author: Hadayat Seddiqi
 *
 */

#include <sys/stat.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "typedefs.h"
#include "macros.h"
//#include "potentials.h"
#include "Grid.h"
#include "Grid1D.h"
#include "Grid2D.h"
#include "Grid3D.h"

int main(int argc, char *argv[]) {
  using namespace std;

  char line[MAX_LINE_LENGTH];
  int ret;
  string pfile;
  FILE *fptr, *outfptr;

  // Define all parameter input variables
  string inputdata;
  int debug,
    dim,
    reality,
    nstatic,
    nramp,
    nrelease,
    outnum,
    gridnx,
    gridny,
    gridnz,
    potential,
    method;
  double stepx,
    stepy,
    stepz,
    stept,
    nonlin,
    vdepth,
    rhot;

  // Create data output directory
  int dirchk = mkdir("data", 0755);

  if (debug) {
    if (dirchk == 0) {
      printf("Successfully created data directory.\n");
    } else {
      printf("Failed creating data directory, probably already exists.\n");
    }
  }

  // Check to see if user told us about parameter file
  if (argc == 1) {
      printf("Parameter file must be specified at run time.\n");
      exit(1);
  } else if (argc == 2) {
      // Open the file, and see if there are any problems
      if ((fptr = fopen(argv[1], "r+")) == NULL) {
	  printf("Cannot open parameter file.\n");
	  exit(1);
      }
  }

  // Get input values from parameter file
  while (fgets(line, MAX_LINE_LENGTH, fptr) != NULL) {
      ret = 0;
      ret += sscanf(line, "Debug = %d", &debug);
      ret += sscanf(line, "Dimensions = %d", &dim);
      ret += sscanf(line, "TimeReality = %d", &reality);
      //      ret += sscanf(line, "InputFile = %s", &inputdata);
      ret += sscanf(line, "StaticIter = %d", &nstatic);
      ret += sscanf(line, "RampIter = %d", &nramp);
      ret += sscanf(line, "ReleaseIter = %d", &nrelease);
      //      ret += sscanf(line, "OutputsName = %s", &outname);
      ret += sscanf(line, "OutputsNum = %d", &outnum);
      ret += sscanf(line, "PointsX = %d", &gridnx);
      ret += sscanf(line, "PointsY = %d", &gridny);
      ret += sscanf(line, "PointsZ = %d", &gridnz);
      ret += sscanf(line, "StepX = %lf", &stepx);
      ret += sscanf(line, "StepY = %lf", &stepy);
      ret += sscanf(line, "StepZ = %lf", &stepz);     
      ret += sscanf(line, "StepT = %lf", &stept);
      ret += sscanf(line, "Nonlinearity = %lf", &nonlin);
      ret += sscanf(line, "Potential = %d", &potential);
      ret += sscanf(line, "Method = %d", &method);
      ret += sscanf(line, "Depth = %lf", &vdepth);
      ret += sscanf(line, "RadiusRing = %lf", &rhot);
  }

  /* Do some error checking here, in case the user puts
     in some nonsensical parameters. */

  // Check that we don't have outputs at fractional iterations
  if ( (nstatic + nramp + nrelease) < outnum ) {
    printf("Total iterations exceeds number of outputs.\nBe sure to set all (three) iteration parameters.\n");
    exit(1);
  }


  /* Honestly, we should be casting all of these variables
     to floats or doubles even, so that we make sure no
     bullshit with missing decimal points happens with the 
     parameters file. */

  // Initialize the grid
  if (dim == 3) {
    Grid3D Psi = Grid3D(gridnx, gridny, gridnz,
			stepx, stepy, stepz,
			dim, potential,
			vdepth, rhot);
  } else if (dim == 2) {
    Grid2D Psi = Grid2D(gridnx, gridny,
			stepx, stepy,
			dim, potential,
			vdepth, rhot);
  } else if (dim == 1) {
    Grid1D Psi = Grid1D(gridnx,stepx,stept,
			dim, potential, reality,
			vdepth, rhot);

    Psi.Initialize();
    Psi.Compute(method, nonlin, nrelease, outnum);

  } else {
    printf("Dimensions out of range.");
    exit(1);
  }

  // Write out values to output file
  if ((outfptr = fopen("data/params.out", "w")) == NULL) {
      printf("Cannot open output parameter file.\n");
      exit(1);
  }

  fprintf(outfptr, "Dimensions = %d\n", dim);
  fprintf(outfptr, "Reality = %d\n", reality);
  fprintf(outfptr, "StaticIter = %d\n", nstatic);
  fprintf(outfptr, "RampIter = %d\n", nramp);
  fprintf(outfptr, "ReleaseIter = %d\n", nrelease);
  fprintf(outfptr, "PointsX = %d\n", gridnx);
  fprintf(outfptr, "PointsY = %d\n", gridny);
  fprintf(outfptr, "PointsZ = %d\n", gridnz);
  fprintf(outfptr, "StepX = %lf\n", stepx);
  fprintf(outfptr, "StepY = %lf\n", stepy);
  fprintf(outfptr, "StepZ = %lf\n", stepz);
  fprintf(outfptr, "StepT = %lf\n", stept);
  fprintf(outfptr, "Nonlinearity = %lf\n", nonlin);
  fprintf(outfptr, "Potential = %d\n", potential);

  return 0;

}
