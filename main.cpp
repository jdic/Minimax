#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <limits>
#include <cmath>

const char PLAYER1 = 'X';
const char PLAYER2 = 'O';

std::vector<std::vector<int>> generate_win_combinations(int n)
{
  std::vector<std::vector<int>> win_combinations;

  for (int row = 0; row < n; ++row)
  {
    std::vector<int> combination;

    for (int col = 0; col < n; ++col)
    {
      combination.push_back((row * n) + col);
    }

    win_combinations.push_back(combination);
  }

  for (int col = 0; col < n; ++col)
  {
    std::vector<int> combination;

    for (int row = 0; row < n; ++row)
    {
      combination.push_back((row * n) + col);
    }

    win_combinations.push_back(combination);
  }

  std::vector<int> diagonal1;

  for (int i = 0; i < n; ++i)
  {
    diagonal1.push_back((i * n) + i);
  }

  std::vector<int> diagonal2;

  for (int i = 0; i < n; ++i)
  {
    diagonal2.push_back((i * n) + (n - i - 1));
  }

  win_combinations.push_back(diagonal1);
  win_combinations.push_back(diagonal2);

  return win_combinations;
}

bool is_winner(const std::vector<char> &board, char player, const std::vector<std::vector<int>> &win_combinations)
{
  for (const auto &combination : win_combinations)
  {
    bool result = true;

    for (int idx : combination)
    {
      if (board[idx] != player)
      {
        result = false;
        break;
      }
    }

    if (result)
    {
      return true;
    }
  }

  return false;
}

bool is_board_full(const std::vector<char> &board)
{
  for (char cell : board)
  {
    if (cell != PLAYER1 && cell != PLAYER2)
    {
      return false;
    }
  }

  return true;
}

int minimax(std::vector<char> &board, bool isMaximizing, const std::vector<std::vector<int>> &win_combinations)
{
  if (is_winner(board, PLAYER1, win_combinations))
  {
    return 10;
  }

  if (is_winner(board, PLAYER2, win_combinations))
  {
    return -10;
  }

  if (is_board_full(board))
  {
    return 0;
  }

  if (isMaximizing)
  {
    int best_score = std::numeric_limits<int>::min();

    for (int i = 0; i < board.size(); ++i)
    {
      if (board[i] != PLAYER1 && board[i] != PLAYER2)
      {
        board[i] = PLAYER1;
        int score = minimax(board, false, win_combinations);
        board[i] = ' ';
        best_score = std::max(score, best_score);
      }
    }

    return best_score;
  }

  else
  {
    int best_score = std::numeric_limits<int>::max();

    for (int i = 0; i < board.size(); ++i)
    {
      if (board[i] != PLAYER1 && board[i] != PLAYER2)
      {
        board[i] = PLAYER2;
        int score = minimax(board, true, win_combinations);
        board[i] = ' ';
        best_score = std::min(score, best_score);
      }
    }

    return best_score;
  }
}

int get_best_move(std::vector<char> &board, char current, const std::vector<std::vector<int>> &win_combinations)
{
  int best_move = -1;
  int best_score = (current == PLAYER1)
    ? std::numeric_limits<int>::min()
    : std::numeric_limits<int>::max();

  for (int i = 0; i < board.size(); ++i)
  {
    if (board[i] != PLAYER1 && board[i] != PLAYER2)
    {
      board[i] = current;
      int score = minimax(board, !(current == PLAYER1), win_combinations);
      board[i] = ' ';

      if (current == PLAYER1)
      {
        if (score > best_score)
        {
          best_score = score;
          best_move = i;
        }
      }

      else
      {
        if (score < best_score)
        {
          best_score = score;
          best_move = i;
        }
      }
    }
  }

  return best_move;
}

void print_board(const std::vector<char> &board, int n)
{
  std::cout << "\033[H";

  for (int i = 0; i < board.size(); i++)
  {
    std::cout << board[i] << " ";

    if ((i + 1) % n == 0)
    {
      std::cout << std::endl;
    }
  }
}

int main()
{
  int n = 3;
  std::vector<char> board(pow(n, 2), ' ');
  char current = PLAYER1;

  auto win_combinations = generate_win_combinations(n);

  while (true)
  {
    print_board(board, n);

    if (is_winner(board, PLAYER1, win_combinations))
    {
      std::cout << "Winner: Player" << std::endl;
      break;
    }

    if (is_winner(board, PLAYER2, win_combinations))
    {
      std::cout << "Winner: Opponent" << std::endl;
      break;
    }

    if (is_board_full(board))
    {
      std::cout << std::endl << "Tie!" << std::endl;
      break;
    }

    int move = get_best_move(board, current, win_combinations);
    board[move] = current;

    current = current == PLAYER1
      ? PLAYER2
      : PLAYER1;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
