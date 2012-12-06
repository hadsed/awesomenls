/*
 * Grid3D.h
 *
 * Declare 3D child class from parent Grid class.
 * Define forward declarations for member functions.
 *
 * Author: Hadayat Seddiqi
 *
 */
#ifndef GRID3D_H
#define GRID3D_H

class Grid3D : public Grid {
 private:
  int gridnx, gridny, gridnz;
  float stepx, stepy, stepz;

  std::vector<float> gridx;
  std::vector<float> gridy;
  std::vector<float> gridz;
  std::vector<float> potential;

 public:
  Grid3D(int, int, int, float, float, float,
	 int, int, float, float);
  ~Grid3D();
  void Initialize();

};

#endif

