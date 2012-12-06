/*
 * Grid.h
 *
 * Declare Grid class and variables. Define forward declarations
 * for member functions.
 *
 * Author: Hadayat Seddiqi
 *
 */
#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid {

 protected:
  int dim, potentialtype;
  float vdepth, rhot;

 public:
  /*
    Some day when I decide to make values as proper
    enumerated types...

  enum Potentials {Harmonic, Spherical, Ring}
  enum Methods {CrankNicolson, FFT}

  */
  Grid(int, int, float, float);
  ~Grid();
  //  void Initialize();
  //  void Compute();

};

#endif
