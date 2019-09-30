#include <cstdlib>
#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

struct Pos {
  int r;
  int c;
};

struct Space {
  bool fixed;
  int val;
  Space& operator=(const Space& s) {
    fixed = s.fixed;
    val = s.val;
    return *this;
  }
};

struct Board {
  Space state[9][9];
  Board& operator=(const Board& b) {
    // cout << "ENTERING OVERLOADED OPERATOR\n";
    for (int r = 0; r < 9; r++) {
      for (int c = 0; c < 9; c++) {
        state[r][c] = b.state[r][c];
      }
    }
    // cout << "LEAVING OVERLOADED OPERATOR\n";
    return *this;
  }
};

bool is_solved(Board*);
int calculate_conflicts(Board*, int, int);
void initialize_board(Board*);
void print_board(Board*);
void solve_board(Board*);

bool is_solved(Board* board) {
  for (int r = 0; r < 9; r++) {
    for (int c = 0; c < 9; c++) {
      if (calculate_conflicts(board, r, c) > 0) {
        return false;
      }
    }
  }
  return true;
}

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

void initialize_board(Board* board) {
  int n;
  srand(time(0));
  cout << "enter below the initial board:\n";
  for (int r = 0; r < 9; r++) {
    for (int c = 0; c < 9; c++) {
      cin >> n;
      if (n == 0) {
        board->state[r][c].val = (rand() % 9) + 1;
        board->state[r][c].fixed = false;
      } else {
        board->state[r][c].val = n;
        board->state[r][c].fixed = true;
      }
    }
  }
}

void print_board(Board* board) {
  cout << "   ";
  for (int i = 0; i < 9; i++) {
    cout << i << " ";
  }
  cout << endl;
  cout << "  ------------------\n";
  for (int r = 0; r < 9; r++) {
    cout << r << "| ";
    for (int c = 0; c < 9; c++) {
      cout << board->state[r][c].val << " ";
    }
    cout << endl;
  }
}

void solve_board(Board* board) {
  int chosen_row, chosen_col, min_conflicts, n, neighbor_conflicts, num_mins, num_neighbors, original_conflicts, next_conflicts;
  Board next_board;
  // Board* temp;
  vector<Board> neighbors;
  while (!is_solved(board)) {
    // cout << "ENTERED LOOP\n";
    // pick a random conflicting space
    vector<Pos> conflicting_spaces;
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (calculate_conflicts(board, i, j) > 0 && !board->state[i][j].fixed) {
          Pos p = {i, j};
          conflicting_spaces.push_back(p);
        }
      }
    }
    Pos chosen_pos = conflicting_spaces[rand() % conflicting_spaces.size()];
    chosen_row = chosen_pos.r;
    chosen_col = chosen_pos.c;
    // do {
    //   chosen_row = rand() % 9;
    //   chosen_col = rand() % 9;
    //   original_conflicts = calculate_conflicts(board, chosen_row, chosen_col);
    // } while (original_conflicts == 0 || board->state[chosen_row][chosen_col].fixed);
    n = board->state[chosen_row][chosen_col].val;
    // expand neighbors for chosen space
    num_neighbors = 0;
    neighbors.clear();
    for (int i = 1; i <= 9; i++) {
      Board temp = *board;
      temp.state[chosen_row][chosen_col].val = i;
      neighbors.push_back(temp);
      // neighbors[i-1].state[chosen_row][chosen_col].val = i;
      // temp = new Board;
      // *temp = *board;
      // temp->state[chosen_row][chosen_col].val = i;
      // neighbors.push_back(temp);
      num_neighbors++;
    }
    // cout << "LINE 143 (num_neighbors: " << num_neighbors << ")\n";
    // pick neighbor with minimum conflicts for chosen space
    min_conflicts = original_conflicts;
    for (int i = 0; i < num_neighbors; i++) {
      neighbor_conflicts = calculate_conflicts(&neighbors[i], chosen_row, chosen_col);
      // if (neighbor_conflicts == 0) {
        // cout << "found 0 conflicting value\n";
      // }
      // cout << "neighbor conflicts: " << neighbor_conflicts << endl;
      if (neighbor_conflicts < min_conflicts) {
        min_conflicts = neighbor_conflicts;
      }
    }
    // cout << "LINE 152 (minimum conflicts: " << min_conflicts << ")\n";
    vector<Board> mins;
    num_mins = 0;
    for (int i = 0; i < num_neighbors; i++) {
      neighbor_conflicts = calculate_conflicts(&neighbors[i], chosen_row, chosen_col);
      // cout << "LINE 159 (neighbor_conflicts: " << neighbor_conflicts << ")\n";
      if (neighbor_conflicts == min_conflicts) {
        mins.push_back(neighbors[i]);
        num_mins++;
      }
    }
    // cout << "LINE 162 (num_mins is " << num_mins << ")\n";
    // do {
      // next_board = neighbors[rand() % num_neighbors];
    next_board = mins[rand() % num_mins];
    // cout << "LINE 166\n";
      // next_conflicts = calculate_conflicts(&next_board, chosen_row, chosen_col);
    // } while (next_conflicts > min_conflicts);
    *board = next_board;
    // cout << "LINE 169\n";
    neighbors.clear(); // DOES NOT FREE UP MEMORY
    mins.clear();
    // cout << "END OF LOOP\n";
  }
  cout << "Solved board:\n";
  print_board(board);
}
