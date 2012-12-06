/*
 * Grid3D.cpp
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
#include "Grid3D.h"
#include "macros.h"

Grid3D::Grid3D(int x, int y, int z, float dx, float dy, float dz, 
	       int d, int p, float v, float r) : Grid(d, p, v, r) {
  gridnx = x;
  gridny = y;
  gridnz = z;
  stepx = dx;
  stepy = dy;
  stepz = dz;

  // Resize the spacial vectors
  gridx.resize(gridnx);
  gridy.resize(gridny);
  gridz.resize(gridnz);

  // Resize the potential multi-vector
  potential.resize(gridnx*gridny*gridnz);

}

Grid3D::~Grid3D() {

}

void Grid3D::Initialize() {

  std::vector<float> gridx2(gridnx);
  std::vector<float> gridy2(gridny);
  std::vector<float> gridz2(gridnz);

  int i,j,k;

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

	  for (k = 0; k < gridnz; k++) {
	    // Set up points in z direction
	    gridz[k] = stepz*(k - gridnz/2);
	    gridz2[k] = gridz[k]*gridz[k];
	    
	    // Set up potential
	    potential[(i*gridny + j)*gridnz + k] = gridx2[i] + gridy2[j] + gridz2[k];
	  }
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

	  for (k = 0; k < gridnz; k++) {
	    // Set up points in z direction
	    gridz[k] = stepz*(k - gridnz/2);
	    gridz2[k] = gridz[k]*gridz[k];

	    // Set up potential
	    potential[(i*gridny + j)*gridnz + k] = vdepth*(gridx2[i] + gridy2[j] + gridz2[k])/(rhot*rhot)*
	      exp(-(gridx2[i] + gridy2[j] + gridz2[k])/(rhot*rhot));
	  }
	}
      }
      break;

    default:
      printf("Potential type not defined.");
      exit(1);
      break;
      }
}
