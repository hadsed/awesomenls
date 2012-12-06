/*
 * Grid1D.c
 *
 * Define constructor and member functions.
 *
 * Author: Hadayat Seddiqi
 *
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex>

#include "macros.h"
#include "typedefs.h"
#include "Grid.h"
#include "Grid1D.h"

Grid1D::Grid1D(int x, double dx, double dt, int d, int p, int rlty, double v, double r) : Grid(d, p, v, r) {
  gridnx = x;
  stepx = dx;
  stept = dt;
  reality = rlty;

  // Resize spacial and potential vectors
  gridx.resize(gridnx);
  potential.resize(gridnx);
  psi.resize(gridnx);

  if (reality) {
    psir.resize(gridnx);
    psii.resize(gridnx);
  }

}

Grid1D::~Grid1D() {

}

void Grid1D::Initialize() {
  
  std::vector<double> gridx2(gridnx);

  int i;
  FILE *outinit;

  // Open file for output of initial state
  if ((outinit = fopen("data/initial", "w")) == NULL) {
    printf("Cannot open file for initial frame output.\n");
    exit(1);
  }

  switch(potentialtype) {

    case 0:
      for (i = 0; i < gridnx; i++) {
	// Set up points in z direction
	gridx[i] = stepx*(i - gridnx/2);
	gridx2[i] = gridx[i]*gridx[i];
	// Set up potential
	potential[i] = -gridx2[i];
	// Set up wavefunction
	psi[i] = exp(-0.5*gridx2[i]);
	// Output initial wavefunction
	fprintf(outinit,"%lf   %lf\n", gridx[i], psi[i]);
      }
      break;

    case 1:
      for (i = 0; i < gridnx; i++) {
	// Set up points in z direction
	gridx[i] = stepx*(i - gridnx/2);
	gridx2[i] = gridx[i]*gridx[i];
	// Set up potential
	potential[i] = -vdepth*gridx2[i]/(rhot*rhot)*exp(-gridx2[i]/(rhot*rhot));
	// Set up wavefunction
	psi[i] = exp(-0.5*gridx2[i]);
	// Output initial wavefunction
	fprintf(outinit,"%lf   %lf\n", gridx[i], psi[i]);
      }
      break;

    default:
      printf("Potential type not defined.\n");
      exit(1);
      break;
  }

}

void Grid1D::Compute(int method, double nonlin, int iterations, int outnum) {

  FILE *outfptr;

  switch(method) {
     case 0:

       // Real time
       if (reality) {
	 double simp1, simp2, simp3;
	 double psi2norm[gridnx];

	 complexDouble tdD, tdS, bconst, istept, istepx2;
	 complexVector alpha, beta, gamma, psicomp, psi2;

	 /***************************
	  * CHECK THESE RESIZES
	  * 
	  * They originally were gridnx-1
	  * and gridnx, respectively, but
	  * this caused memory leakage.
	  * 
	  * FIX.
	  *
	  ***************************/

	 alpha.resize(gridnx);
	 beta.resize(gridnx);
	 gamma.resize(gridnx);

	 psicomp.resize(gridnx + 1);
	 psi2.resize(gridnx + 1);


	 // Initialize tridiagonal coefficients, where tdS is the super and sub 
	 // diagonals, and tdD is the main diagonal. Alpha and gamma are given 
	 // solutions at the ends to start recursion relation.
	 istept.imag() = stept;
	 istepx2 = stepx*stepx;
	 tdD = istept/(istepx2);
	 tdD += 1.0;
	 tdS = -0.5*istept/(istepx2);
	 alpha[gridnx-1] = 0.0;
	 gamma[gridnx-1] = -1.0/tdD;

	 // Actually compute alpha and gamma
	 for (int i = gridnx-1; i > 0; i--) {
	   alpha[i-1] = gamma[i]*tdS;
	   gamma[i-1] = -1.0/(tdD + tdS*alpha[i-1]);
	 }

	 /* 
	    Make sure we allow continuation of realtime
	    simulation through input files at some point, 
	    not just input Psi from the above initialization 
	    with the potential.
	 */

	 // Copy Psi in
	 for (int i = 0; i <= gridnx; i++) psi2[i].real() = psi[i]*psi[i];


	 // Normalize the wavefunction

	 for (int i = 0; i <= gridnx; i++) psi2norm[i] = pow(abs(psi2[i]),2);

	 // Employ Simpson's rule to integrate
	 simp1 = psi2norm[1] + psi2norm[gridnx-1];
	 simp2 = psi2norm[2];
	 for (int i = 3; i <= gridnx-3; i+=2) {
	   simp1 += psi2norm[i];
	   simp2 += psi2norm[i+1];
	 }
	 simp3 = sqrt(stepx*(psi2norm[0] + 4.0*simp1 + 2.0*simp2 + psi2norm[gridnx])/3.0);

	 // Get the proper wavefunction from the integration and
	 // copy wavefunction to complexVector.
	 for (int i = 0; i <= gridnx; i++) {
	   //	   psi[i] = psi[i]/simp3;
	   psicomp[i] = psi2norm[i]/simp3;
	 }

	 // Now loop through all of time
	 for (int t = 1; t < iterations; t++) {


	   // Calculate potential and nonlinear terms
	   for (int i = 1; i <= gridnx; i++) psicomp[i] *= exp(-istept*(potential[i] + nonlin*psi2norm[i]));
	 
	   // Calculate spatial derivatives
	   beta[gridnx-1] = psi[gridnx];
	   for (int i = gridnx-1; i > 0; i--) {
	     bconst = psicomp[i] - (psicomp[i+1] - 2.0*psicomp[i] + psicomp[i-1])*tdS;
	     beta[i-1] = gamma[i]*(tdS*beta[i] - bconst);
	   }
	 
	   //
	   //     This is very suspicious, make sure to look at this later.
	   //
	   psicomp[0] = psi[0] = 1.0e-16;
	   psicomp[0] = psi[0] = 0.0;

	   for (int i = 0; i < gridnx; i++) psicomp[i+1] = alpha[i]*psicomp[i] + beta[i];

	   // Do some release testing
	   if (t == 500) for (int i = 0; i < gridnx; i++) potential[i] = 0;

	   // Employ Simpson's rule to integrate
	   simp1 = psi2norm[1] + psi2norm[gridnx-1];
	   simp2 = psi2norm[2];
	   for (int i = 3; i <= gridnx-3; i+=2) {
	     simp1 += psi2norm[i];
	     simp2 += psi2norm[i+1];
	   }
	   simp3 = sqrt(stepx*(psi2norm[0] + 4.0*simp1 + 2.0*simp2 + psi2norm[gridnx])/3.0);
	   
	   // Get the proper wavefunction from the integration and
	   // copy over for output.
	   for (int i = 0; i<= gridnx; i++) {
	     psicomp[i] = psi2norm[i]/simp3;
	     psir[i] = psicomp[i].real();
	     psii[i] = psicomp[i].imag();
	   }

	   // Output some frames
	   OutputFrames(t, iterations, outnum, reality);
	 }

       // Imaginary time
       } else {
	 double stepx2, tdD, tdS, bconst, simp1, simp2, simp3;
	 double alpha[gridnx-1], beta[gridnx-1], gamma[gridnx-1],
	   psi2[gridnx];


	 // Initialize tridiagonal coefficients, where tdS is the super and sub 
	 // diagonals, and tdD is the main diagonal. Alpha and gamma are given 
	 // solutions at the ends to start recursion relation.
	 stepx2 = stepx*stepx;
	 tdD = 1 + stept/(stepx2);
	 tdS = -0.5*stept/(stepx2);
	 alpha[gridnx-1] = 0.0;
	 gamma[gridnx-1] = -1.0/tdD;

	 // Actually compute alpha and gamma
	 for (int i = gridnx-1; i > 0; i--) {
	   alpha[i-1] = gamma[i]*tdS;
	   gamma[i-1] = -1.0/(tdD + tdS*alpha[i-1]);
	 }


	 // Normalize the wavefunction

	 for (int i = 0; i <= gridnx; i++) psi2[i] = psi[i]*psi[i];

	 // Employ Simpson's rule to integrate
	 simp1 = psi2[1] + psi2[gridnx-1];
	 simp2 = psi2[2];
	 for (int i = 3; i <= gridnx-3; i+=2) {
	   simp1 += psi2[i];
	   simp2 += psi2[i+1];
	 }
	 simp3 = sqrt(stepx*(psi2[0] + 4.0*simp1 + 2.0*simp2 + psi2[gridnx])/3.0);

	 // Get the proper wavefunction from the integration
	 for (int i = 0; i<= gridnx; i++) psi[i] = psi[i]/simp3;


	 // Now loop through all of time
	 for (int t = 1; t < iterations; t++) {


	   // Calculate potential and nonlinear terms
	   for (int i = 1; i <= gridnx; i++) psi[i] *= exp(-stept*(potential[i] + nonlin*psi[i]*psi[i]));
	 
	   // Calculate spatial derivatives
	   beta[gridnx-1] = psi[gridnx];
	   for (int i = gridnx-1; i > 0; i--) {
	     bconst = psi[i] - (psi[i+1] - 2.0*psi[i] + psi[i-1])*tdS;
	     beta[i-1] = gamma[i]*(tdS*beta[i] - bconst);
	   }
	 
	   //
	   //     This is very suspicious, make sure to look at this later.
	   //
	   psi[0] = 1.0e-16;
	   psi[0] = 0.0;
	 
	   for (int i = 0; i < gridnx; i++) psi[i+1] = alpha[i]*psi[i] + beta[i];

	   // Do some release testing
	   if (t == 500) for (int i = 0; i < gridnx; i++) potential[i] = 0;

	   // Normalize each timestep if imaginary time
	   for (int i = 0; i <= gridnx; i++) psi2[i] = psi[i]*psi[i];

	     // Employ Simpson's rule to integrate
	     simp1 = psi2[1] + psi2[gridnx-1];
	     simp2 = psi2[2];
	     for (int i = 3; i <= gridnx-3; i+=2) {
	       simp1 += psi2[i];
	       simp2 += psi2[i+1];
	     }
	     simp3 = sqrt(stepx*(psi2[0] + 4.0*simp1 + 2.0*simp2 + psi2[gridnx])/3.0);

	     // Get the proper wavefunction from the integration
	     for (int i = 0; i<= gridnx; i++) psi[i] = psi[i]/simp3;

	     // Output some frames
	     OutputFrames(t, iterations, outnum, reality);
	 }
       }

       // Output final state
       if ((outfptr = fopen("data/final", "w")) == NULL) {
	 printf("Cannot open file for final frame output.\n");
	 exit(1);
       }

       // Real time   
       if (reality) {
	 for (int i = 0; i < gridnx; i++) fprintf(outfptr,"%lf   %lf   %lf\n", gridx[i], psir[i], psii[i]);

	 // Imaginary time
       } else {
	 for (int i = 0; i < gridnx; i++) fprintf(outfptr,"%lf   %lf\n", gridx[i], psi[i]);
       }

       fclose(outfptr);

       break;
     default:
       printf("Method type not defined.\n");
       exit(1);
       break;
  }

}

void Grid1D::OutputFrames(int t, int iterations, int outnum, int reality) {
  FILE *outfptr;

  if ( (t % (iterations/outnum)) == 0 ) {
    std::string str, framename;
    std::stringstream convert;
    convert << std::setw(4) << std::setfill('0') << t/(iterations/outnum);
    framename = "data/frame";
    str = convert.str();
    framename += str;

    if ((outfptr = fopen(framename.c_str(), "w")) == NULL) {
      printf("Cannot open output frame file.\n");
      exit(1);
    }

    // Real time   
    if (reality) {
      for (int i = 0; i < gridnx; i++) fprintf(outfptr,"%lf   %lf   %lf\n", gridx[i], psir[i], psii[i]);

    // Imaginary time
    } else {
      for (int i = 0; i < gridnx; i++) fprintf(outfptr,"%lf   %lf\n", gridx[i], psi[i]);
    }

    fclose(outfptr);
  }
}
