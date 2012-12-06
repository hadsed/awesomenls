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
#include "macros.h"

Grid::Grid(int d, int p, float v, float rh) {

  dim = d;
  potentialtype = p;
  vdepth = v;
  rhot = rh;

}

Grid::~Grid() {

}
