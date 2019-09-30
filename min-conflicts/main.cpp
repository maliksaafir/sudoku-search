#include "solver.h"

int main() {
  Board* sudoku = new Board;
  initialize_board(sudoku);
  print_board(sudoku);
  solve_board(sudoku);
  return 0;
}
