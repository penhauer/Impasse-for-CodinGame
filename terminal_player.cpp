#include "ai_agent.h"
#include <iostream>


void TerminalPlayer::decideOnBoard(Board &board) {
  // Keep tract of elapsed time
  board.printBoard();
  std::cout << "----------------------------YOUR TURN----------------------------" << std::endl;
  bool turnEnd = false;
  PieceBoard pieceboard;
  // While player has not made a valid move, keep asking for input
  while (turnEnd == false)
  {
      std::cout << "Select position: ";
      std::string notation;
      std::cin >> notation;
      if (notation == "help")
      {
          const char *text =
              "----------------------------HELP----------------------------\n"
              "board: show board\n"
              "moves: show legal moves\n"
              "undo: undo last move\n";
          std::cout << text << std::endl;
      }
      else if (notation == "undo")
      {
          undoPlayerMove(board);
          board.generateLegalMoves();
          board.printBoard();
      }
      else if (notation == "moves")
      {
          std::cout << "----------------------------LEGAL MOVES----------------------------\n";
          for (const auto pieceboard : board.possiblepieceboards) {
              printMove(pieceboard.lastmove);
          }
          std::cout << "\n";
      }
      else if (notation == "board")
      {
          std::cout << "----------------------------BOARD----------------------------\n";
          board.printBoard();
          std::cout << "\n";
      } else {
          Pos pos = parseMove(notation);
          if (pos == EMPTY_POSE) {
              std::cout << "Invalid position, enter 'help' for help" << std::endl;
          }
          else {
              std::tie(turnEnd, pieceboard) = trySelect(board, pos, pieceboard);
          }
      }
  }
  board.doMove(pieceboard);
  board.printBoard();
}



// Check if a move exists among the current legal moves with the selected position
// If only one move exists, make that move, else ask for the next position
// If no move exists with the selected position, reset move and ask for input again
std::tuple<bool, PieceBoard> TerminalPlayer::trySelect(Board board, Pos pos, PieceBoard pb) {
    std::vector<PieceBoard>  onlymoves;

    if (pb.lastmove.from == EMPTY_POSE)
    {
        std::copy_if(board.possiblepieceboards.begin(), board.possiblepieceboards.end(), std::inserter(onlymoves, onlymoves.end()), [&](const PieceBoard &pieceboard)
                     { return pieceboard.lastmove.from == pos; });
        if (onlymoves.size() == 1)
        {
            return std::make_tuple(true, onlymoves.back());
        }
        else if (onlymoves.size() > 1)
        {
            pb.lastmove.from = pos;
            return std::make_tuple(false, pb);
        };
    }
    else if (pb.lastmove.to == EMPTY_POSE)
    {
        std::vector<PieceBoard>  onlymoves;
        std::copy_if(board.possiblepieceboards.begin(), board.possiblepieceboards.end(), std::inserter(onlymoves, onlymoves.end()), [&](const PieceBoard &pieceboard)
                     { return pieceboard.lastmove.from == pb.lastmove.from && pieceboard.lastmove.to == pos; });
        if (onlymoves.size() == 1)
        {
            return std::make_tuple(true, onlymoves.back());
        }
        else if (onlymoves.size() > 1)
        {
            pb.lastmove.to = pos;
            return std::make_tuple(false, pb);
        };
    }
    else if (pb.lastmove.remove == EMPTY_POSE)
    {
        for (PieceBoard pieceboard : board.possiblepieceboards)
        {
            if (pb.lastmove.from == pieceboard.lastmove.from && pb.lastmove.to == pieceboard.lastmove.to && pieceboard.lastmove.remove == pos) {
                return std::make_tuple(true, pieceboard);
            };
        };
    };
    std::cout << "No legal move available for selected position.\nEnter 'help' for help or 'moves' for valid moves" << std::endl;
    return std::make_tuple(false, PieceBoard());
}


// Undo the last AI and player move if possible
void TerminalPlayer::undoPlayerMove(Board &board)
{
    if (board.pieceboardhistory.size() > 1) {
        board.undoMove();
        board.undoMove();
        board.generateLegalMoves();
        std::cout << "Last player move undone" << std::endl;
    }
    else {
        std::cout << "No player moves to undo" << std::endl;
    }
}
