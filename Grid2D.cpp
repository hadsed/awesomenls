/*
 * Grid.c
 *
 * Define constructor and member functions.
 *
 * Author: Hadayat Seddiqi
 *
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Grid.h"
#include "Grid2D.h"
#include "macros.h"

Grid2D::Grid2D(int x, int y, float dx, float dy,
	       int d, int p, float v, float r) : Grid(d, p, v, r) {
  gridnx = x;
  gridny = y;
  stepx = dx;
  stepy = dy;

  // Resize the spacial vectors
  gridx.resize(gridnx);
  gridy.resize(gridny);

  // Resize the potential multi-vector
  potential.resize(gridnx*gridny);

}

Grid2D::~Grid2D() {

}

void Grid2D::Initialize() {

  std::vector<float> gridx2(gridnx);
  std::vector<float> gridy2(gridny);

  int i,j;

  switch(potentialtype) {
    case 0:
      for (i = 0; i < gridnx; i++) {
	// Set up points in z direction
	gridx[i] = stepx*(i - gridnx/2);
	gridx2[i] = gridx[i]*gridx[i];

	for (j = 0; j < gridny; j++) {
	  // Set up points in y direction
	  gridy[j] = stepy*(j - gridny/2);
	  gridy2[j] = gridy[j]*gridy[j];
	  
	  // Set up potential
	  potential[i*gridny + j] = gridx2[i] + gridy2[j];
	}
      }
      break;

    case 1:
      for (i = 0; i < gridnx; i++) {
	// Set up points in z direction
	gridx[i] = stepx*(i - gridnx/2);
	gridx2[i] = gridx[i]*gridx[i];

	for (j = 0; j < gridny; j++) {
	  // Set up points in y direction
	  gridy[j] = stepy*(j - gridny/2);
	  gridy2[j] = gridy[j]*gridy[j];

	  // Set up potential
	  potential[i*gridny + j] = vdepth*(gridx2[i] + gridy2[j])/(rhot*rhot)*
	    exp(-(gridx2[i] + gridy2[j])/(rhot*rhot));
	}
      }
      break;

    default:
      printf("Potential type not defined.");
      exit(1);
      break;
  }
}
