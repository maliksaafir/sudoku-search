#include "sudoku.h"

int main() {
  Board* sudoku = new Board;
  initialize_board(sudoku);
  if (backtracking(sudoku)) {
    cout << "Solution:\n";
    print_board(sudoku);
  } else {
    cout << "Failed\n";
  }
  return 0;
}
