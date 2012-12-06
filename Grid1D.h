/*
 * Grid1D.h
 *
 * Declare 1D child class from parent Grid class.
 * Define forward declarations for member functions.
 *
 * Author: Hadayat Seddiqi
 *
 */

#ifndef GRID1D_H
#define GRID1D_H

class Grid1D : public Grid {
 public:
  Grid1D(int, double, double, int, int, int, double, double);
  ~Grid1D();
  void Initialize();
  void Compute(int, double, int, int);
  void OutputFrames(int, int, int, int);

 private:
  int gridnx, reality;
  double stepx, stept;

  std::vector<double> gridx;
  std::vector<double> potential;
  std::vector<double> psi;
  std::vector<double> psir;
  std::vector<double> psii;

};

#endif
