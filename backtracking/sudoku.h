#include <iostream>
using namespace std;

// wrapper for ordered pairs
struct Pos {
  int r;
  int c;
};

// fixed boolean is true if value is determined from the start state
struct Space {
  bool fixed;
  int val;
};

// node struct
struct Board {
  Space state[9][9];
};

bool backtracking(Board*);
bool is_complete(Board*);
bool recursive_backtracking(Board*);
int calculate_conflicts(Board*, int, int);
void initialize_board(Board*);
void print_board(Board*);

// calls the recursive function
bool backtracking(Board* board) {
  return recursive_backtracking(board);
}

// checks if all spaces have been assigned, which would mean you have a solution
bool is_complete(Board* board) {
  for (int r = 0; r < 9; r++) {
    for (int c = 0; c < 9; c++) {
      if (board->state[r][c].val == 0) {
        return false;
      }
    }
  }
  return true;
}

// uses simple backtracking algorithm to find a solution
bool recursive_backtracking(Board* board) {
  bool conflict, found = false;
  int original_value;
  Pos chosen;
  // check if complete
  if (is_complete(board)) {
    return true;
  }
  // pick next unassigned space
  for (int r = 0; r < 9 && !found; r++) {
    for (int c = 0; c < 9 && !found; c++) {
      if (board->state[r][c].val == 0) {
        chosen.r = r;
        chosen.c = c;
        found = true;
      }
    }
  }
  // test each value for the space to see if it fits the constraints
  for (int i = 1; i <= 9; i++) {
    board->state[chosen.r][chosen.c].val = i;
    conflict = calculate_conflicts(board, chosen.r, chosen.c) > 0;
    // if value i does not cause conflicts call recursive backtracking again
    if (!conflict) {
      if (recursive_backtracking(board)) {
        return true;
      }
    }
    // else reset value
    board->state[chosen.r][chosen.c].val = 0;
  }
  return false;
}

// add up the block, column, and row conflicts
int calculate_conflicts(Board* board, int row, int col) {
  bool in_block, match, same_space;
  int block_r = (row / 3) * 3;
  int block_c = (col / 3) * 3;
  int conflicts = 0;
  int n = board->state[row][col].val;
  // block conflicts
  for (int r = block_r; r < block_r + 3; r++) {
    for (int c = block_c; c < block_c + 3; c++) {
      match = board->state[r][c].val == n;
      same_space = r == row && c == col;
      if (!same_space && match) {
        conflicts++;
      }
    }
  }
  // column conflicts
  for (int r = 0; r < 9; r++) {
    in_block = r >= block_r && r < block_r + 3;
    match = board->state[r][col].val == n;
    if (r != row && !in_block && match) {
      conflicts++;
    }
  }
  // row conflicts
  for (int c = 0; c < 9; c++) {
    in_block = c >= block_c && c < block_c + 3;
    match = board->state[row][c].val == n;
    if (c != col && !in_block && match) {
      conflicts++;
    }
  }
  return conflicts;
}

// set up board from input
void initialize_board(Board* board) {
  int n;
  cout << "enter below the initial board:\n";
  for (int r = 0; r < 9; r++) {
    for (int c = 0; c < 9; c++) {
      cin >> n;
      board->state[r][c].fixed = n != 0;
      board->state[r][c].val = n;
    }
  }
}

// display board
void print_board(Board* board) {
  for (int r = 0; r < 9; r++) {
    for (int c = 0; c < 9; c++) {
      cout << board->state[r][c].val << " ";
    }
    cout << endl;
  }
}
