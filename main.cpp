#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <limits>

const char PLAYER = 'X';
const char OPPONENT = 'O';

bool is_winner(const std::vector<char> &board, char player)
{
  const int WIN_COMBINATIONS[8][3] =
  {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6}
  };

  for (int i = 0; i < 8; i++)
  {
    bool result =
      board[WIN_COMBINATIONS[i][0]] == player &&
      board[WIN_COMBINATIONS[i][1]] == player &&
      board[WIN_COMBINATIONS[i][2]] == player;

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
    if (cell != PLAYER && cell != OPPONENT)
    {
      return false;
    }
  }

  return true;
}

int minimax(std::vector<char> &board, bool isMaximizing)
{
  if (is_winner(board, PLAYER))
  {
    return 10;
  }

  if (is_winner(board, OPPONENT))
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

    for (int i = 0; i < 9; ++i)
    {
      if (board[i] != PLAYER && board[i] != OPPONENT)
      {
        board[i] = PLAYER;
        int score = minimax(board, false);
        board[i] = ' ';
        best_score = std::max(score, best_score);
      }
    }

    return best_score;
  }

  else
  {
    int best_score = std::numeric_limits<int>::max();

    for (int i = 0; i < 9; ++i)
    {
      if (board[i] != PLAYER && board[i] != OPPONENT)
      {
        board[i] = OPPONENT;
        int score = minimax(board, true);
        board[i] = ' ';
        best_score = std::min(score, best_score);
      }
    }

    return best_score;
  }
}

int get_best_move(std::vector<char> &board, char current)
{
  int best_move = -1;
  int best_score = (current == PLAYER)
    ? std::numeric_limits<int>::min()
    : std::numeric_limits<int>::max();

  for (int i = 0; i < 9; ++i)
  {
    if (board[i] != PLAYER && board[i] != OPPONENT)
    {
      board[i] = current;
      int score = minimax(board, !(current == PLAYER));
      board[i] = ' ';

      if (current == PLAYER)
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

void print_board(const std::vector<char> &board)
{
  std::cout << "\033[H";

  for (int i = 0; i < 9; i++)
  {
    std::cout << board[i] << " ";

    if ((i + 1) % 3 == 0)
    {
      std::cout << std::endl;
    }
  }
}

int main()
{
  std::vector<char> board(9, ' ');
  char current = PLAYER;

  while (true)
  {
    print_board(board);

    if (is_winner(board, PLAYER))
    {
      std::cout << std::endl << "Winner: Player" << std::endl;
      break;
    }

    if (is_winner(board, OPPONENT))
    {
      std::cout << std::endl << "Winner: Opponent" << std::endl;
      break;
    }

    if (is_board_full(board))
    {
      std::cout << std::endl << "Tie!" << std::endl;
      break;
    }

    int move = get_best_move(board, current);
    board[move] = current;

    current = current == PLAYER
      ? OPPONENT
      : PLAYER;

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}
