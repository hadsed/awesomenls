/*
 * Grid2D.h
 *
 * Declare 2D child class from parent Grid class.
 * Define forward declarations for member functions.
 *
 * Author: Hadayat Seddiqi
 *
 */
#ifndef GRID2D_H
#define GRID2D_H

class Grid2D : public Grid {
 private:
  int gridnx, gridny;
  float stepx, stepy;

  std::vector<float> gridx;
  std::vector<float> gridy;
  std::vector<float> potential;

 public:
  Grid2D(int, int, float, float, int, int, float, float);
  ~Grid2D();
  void Initialize();

};

#endif
