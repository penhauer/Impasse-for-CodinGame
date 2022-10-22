#include "board.h"

PieceBoard::PieceBoard(){};
// Evaluate board position based on piece count, number of transitions, and distances from the pieces' last rows
int PieceBoard::evaluate(int color) const
{
    int piecevalue = color * (2 * piececount.blackSingles + 3 * piececount.blackDoubles - 2 * piececount.whiteSingles - 3 * piececount.whiteDoubles);
    int distancevalue = 0;
    int transitionvalue = 0;
    int pieceno = 0;
    for (auto x : piecemap)
    {
        const Piece &piece = x.second;
        if (piece.color == color)
        {
            distancevalue += piece.distance;
            transitionvalue += piece.transitions;
            pieceno += 1;
        }
    };
    return 3 * piecevalue + 5 * transitionvalue + distancevalue / pieceno;
};
Board::Board(){};
// Load board from file
void Board::reset(Board savedboard)
{
    turn = savedboard.turn;
    state = savedboard.state;
    pieceboard = savedboard.pieceboard;
    initBoard();
};
// Create new board
void Board::reset()
{
    turn = 1;
    state = 0;
    pieceboard.postocrown.clear();
    newBoard();
    initBoard();
};
// Initialize board and legal moves
void Board::initBoard()
{
    pieceboardhistory.clear();
    getPieceCount();
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    generateLegalMoves();
    end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time to create possible boards: " << duration.count() << " microseconds" << std::endl;
};
// Print board
void Board::printBoard() const
{
    for (int row = 7; row >= 0; row--)
    {
        std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
        std::cout << row + 1 << " ";
        for (int col = 0; col < 8; col++)
        {
            std::cout << "|";
            int pos = row * 8 + col;
            if (pos == pieceboard.lastmove.from)
            {
                std::cout << ">";
            }
            else if (pos == pieceboard.lastmove.to)
            {
                std::cout << "<";
            }
            else if (pos == pieceboard.lastmove.remove)
            {
                std::cout << "x";
            }
            else
            {
                std::cout << " ";
            };
            if (pieceboard.piecemap.count(pos) > 0)
            {
                const Piece &piece = pieceboard.piecemap.at(pos);
                switch (piece.piece)
                {
                case 1:
                    switch (piece.color)
                    {
                    case 1:
                        std::cout << "○";
                        break;
                    case -1:
                        std::cout << "●";
                        break;
                    }
                    break;
                case 2:
                    switch (piece.color)
                    {
                    case 1:
                        std::cout << "◇";
                        break;
                    case -1:
                        std::cout << "◆";
                        break;
                    }
                    break;
                };
            }
            else if ((row + col) % 2 != 0)
            {
                std::cout << "/";
            }
            else
            {
                std::cout << " ";
            };
            if (pos == pieceboard.lastmove.from)
            {
                std::cout << "<";
            }
            else if (pos == pieceboard.lastmove.to)
            {
                std::cout << ">";
            }
            else if (pos == pieceboard.lastmove.remove)
            {
                std::cout << "x";
            }
            else
            {
                std::cout << " ";
            }
        };
        std::cout << "|" << std::endl;
    };
    std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    std::cout << "    A   B   C   D   E   F   G   H " << std::endl;
};
// Generate legal moves for the player in turn
void Board::generateLegalMoves()
{
    possiblepieceboards.clear();
    for (auto x : pieceboard.piecemap)
    {
        const int &pos = x.first;
        const Piece &piece = x.second;
        if (piece.color == turn)
        {
            addPieceMoves(piece, pos);
        }
    };
    if (possiblepieceboards.size() == 0)
    {
        // If no moves are avilable, add impasse moves
        addImpassable();
    };
};
// Update board state with selected move
void Board::doMove(const PieceBoard new_pieceboard)
{
    pieceboardhistory.push_back(pieceboard);
    pieceboard = new_pieceboard;
    turn = turn * -1;
};
// Undo last move
void Board::undoMove()
{
    pieceboard = pieceboardhistory.back();
    pieceboardhistory.pop_back();
    turn = turn * -1;
};
// Turn single piece into double
void Board::crown(PieceBoard &pieceboard, const Piece &p, const int &pos)
{
    Piece &piece = pieceboard.piecemap[pos];
    switch (piece.color)
    {
    case 1:
        pieceboard.piececount.whiteDoubles++;
        pieceboard.piececount.whiteSingles--;
        break;
    case -1:
        pieceboard.piececount.blackDoubles++;
        pieceboard.piececount.blackSingles--;
        break;
    };
    piece.piece = 2;
    piece.getDirection();
    piece.transitions += 1;
};
// Turn double piece into single
void Board::bearOff(PieceBoard &pieceboard, const Piece &p, const int &pos)
{
    Piece &piece = pieceboard.piecemap[pos];
    switch (piece.color)
    {
    case 1:
        pieceboard.piececount.whiteDoubles--;
        pieceboard.piececount.whiteSingles++;
        break;
    case -1:
        pieceboard.piececount.blackDoubles--;
        pieceboard.piececount.blackSingles++;
        break;
    };
    piece.piece = 1;
    piece.getDirection();
    piece.transitions += 1;
};
// Remove / bear-off piece based on its type
void Board::remove(PieceBoard &pieceboard, const Piece &p, const int &pos)
{
    switch (p.piece)
    {
    // Remove single
    case 1:
        switch (p.color)
        {
        case 1:
            pieceboard.piececount.whiteSingles--;
            break;
        case -1:
            pieceboard.piececount.blackSingles--;
            break;
        };
        pieceboard.piecemap.erase(pos);
        break;
    // Bear-off double
    case 2:
        bearOff(pieceboard, p, pos);
        break;
    };
    // Chickendinner
    if (pieceboard.piececount.whiteSingles + pieceboard.piececount.whiteDoubles == 0)
    {
        state = 1;
    }
    else if (pieceboard.piececount.blackSingles + pieceboard.piececount.blackDoubles == 0)
    {
        state = -1;
    };
};
// Create set of singles color matching the player in turn for crowning
PosSet Board::checkSingles(const Piece &piece, const int &pos) const
{
    PosSet singles;
    for (auto x : pieceboard.piecemap)
    {
        const int &ppos = x.first;
        Piece p = x.second;
        if (p.piece == 1 && p.color == turn && (ppos != pos))
        {
            singles.insert(ppos);
        };
    };
    return singles;
};
// Return true if transpose move is legal
bool Board::ifTransposable(const Piece &piece, const int &piecerow, const int &i, const int &topos) const
{
    return (piecerow < 7 && piecerow > 0 && (i == 1 || i == -1) && pieceboard.piecemap.count(topos) > 0 && pieceboard.piecemap.at(topos).color == turn && piece.piece + pieceboard.piecemap.at(topos).piece == 3);
};
// Return true if position is empty
bool Board::ifEmptySquare(const int &pos) const
{
    return pieceboard.piecemap.count(pos) == 0;
};
// Add legal moves for a piece
void Board::addPieceMoves(const Piece &piece, const int &pos)
{
    int piecerow = pos / 8;
    int piececol = pos % 8;
    // Separate left and right search
    for (int sign = 0; sign < 2; sign++)
    {
        int i = piece.direction;
        while (piecerow + i < 8 && piecerow + i >= 0)
        {
            int row = piecerow + i;
            int col = piececol + i * pow(-1, sign);
            int topos = row * 8 + col;
            if (col < 0 || col > 7)
            {
                break;
            };
            // Slide or transpose
            bool transposable = ifTransposable(piece, piecerow, i, topos);
            bool emptysquare = ifEmptySquare(topos);
            if (emptysquare || transposable)
            {
                // Crowning
                if (row == 0 || row == 7)
                {
                    if (piece.piece == 1)
                    {
                        PosSet singles = checkSingles(piece, pos);
                        // If other single available
                        if (singles.size() > 0)
                        {
                            for (auto removepiecepos : singles)
                            {
                                PieceBoard new_pieceboard = pieceboard;
                                Piece &removepiece = new_pieceboard.piecemap.at(removepiecepos);
                                new_pieceboard.lastmove = Move(pos, topos, removepiecepos);
                                crown(new_pieceboard, piece, pos);
                                move(new_pieceboard, pos, topos);
                                remove(new_pieceboard, removepiece, removepiecepos);
                                possiblepieceboards.push_back(new_pieceboard);
                            }
                        }
                        // if no other singles available, just add as normal slide (and add to crownstack)
                        else
                        {
                            PieceBoard new_pieceboard = pieceboard;
                            new_pieceboard.lastmove = Move(pos, topos, -1);
                            move(new_pieceboard, pos, topos);
                            new_pieceboard.postocrown[turn] = topos;
                            possiblepieceboards.push_back(new_pieceboard);
                        }
                    }
                    else
                    {
                        PieceBoard new_pieceboard = pieceboard;
                        bearOff(new_pieceboard, piece, pos);
                        bool crowned = crownIf(new_pieceboard, piece, pos);
                        if (!crowned)
                        {
                            new_pieceboard.lastmove = Move(pos, topos, pos);
                            move(new_pieceboard, pos, topos);
                        }
                        else
                        {
                            new_pieceboard.lastmove = Move(pos, topos, -1);
                        };
                        possiblepieceboards.push_back(new_pieceboard);
                    }
                }
                else
                {
                    PieceBoard new_pieceboard = pieceboard;
                    new_pieceboard.lastmove = Move(pos, topos, -1);
                    move(new_pieceboard, pos, topos);
                    possiblepieceboards.push_back(new_pieceboard);
                };
                // if it was a transpose, break loop with current sign
                if (transposable)
                {
                    break;
                };
            }
            // If the square was not empty or transposable, break the loop
            else
            {
                break;
            };
            // Otherwise increment i and continue loop with next square
            i = i + piece.direction;
        };
    };
};
// Move piece by transposing with the target square
void Board::move(PieceBoard &pieceboard, int pos, int topos)
{
    // If square is not empty, change places (in case of transpose)
    if (pieceboard.piecemap.count(topos) > 0)
    {
        Piece topiece = pieceboard.piecemap[topos];
        pieceboard.piecemap[topos] = pieceboard.piecemap.at(pos);
        pieceboard.piecemap[pos] = topiece;
        pieceboard.piecemap.at(pos).getDistance(pos);
    }
    // Else just move the piece
    else
    {
        pieceboard.piecemap[topos] = pieceboard.piecemap.at(pos);
        pieceboard.piecemap.erase(pos);
    }
    pieceboard.piecemap.at(topos).getDistance(topos);
};
// Check if there's a piece waiting to be crowned, and if so, crown it with the current piece
bool Board::crownIf(PieceBoard &pieceboard, const Piece &p, const int &pos)
{
    bool crowned = false;
    if (pieceboard.postocrown.count(turn) > 0)
    {
        const Piece &removepiece = pieceboard.piecemap.at(pos);
        const int &crownpiecepos = pieceboard.postocrown.at(turn);
        const Piece &crownpiece = pieceboard.piecemap.at(crownpiecepos);
        crown(pieceboard, crownpiece, crownpiecepos);
        pieceboard.postocrown.erase(turn);
        remove(pieceboard, removepiece, pos);
        crowned = true;
    }
    return crowned;
};
// Add all legal impasse moves
void Board::addImpassable()
{
    for (auto x : pieceboard.piecemap)
    {
        int pos = x.first;
        Piece piece = x.second;
        int row = pos / 8;
        int col = pos % 8;
        if (piece.color == turn)
        {
            PieceBoard new_pieceboard = pieceboard;
            // If piece is double, check if it's first row, and a single is waiting to be crowned.
            // If so, in case of impasse of this double, 2 crownings are made possible.
            // If not, just add the double as a normal impasse move.
            if (piece.piece == 2)
            {
                if ((row == 0 || row == 7) && pieceboard.postocrown.count(turn) > 0)
                {
                    int &removepiecepos = pieceboard.postocrown.at(turn);
                    Piece &removepiece = pieceboard.piecemap.at(removepiecepos);
                    remove(new_pieceboard, removepiece, removepiecepos);
                    new_pieceboard.postocrown.erase(turn);
                    crown(new_pieceboard, piece, pos);
                    new_pieceboard.lastmove = Move(pos, -1, removepiecepos);
                    possiblepieceboards.push_back(new_pieceboard);
                    new_pieceboard = pieceboard;
                };
                remove(new_pieceboard, piece, pos);
                crownIf(new_pieceboard, piece, pos);
                new_pieceboard.lastmove = Move(pos, -1, pos);
            }
            // If piece is single, no need to check for crowning, since then wouldn't be a single in the first place
            else
            {
                remove(new_pieceboard, piece, pos);
                new_pieceboard.lastmove = Move(pos, -1, pos);
            };
            possiblepieceboards.push_back(new_pieceboard);
        };
    };
};
// Create new board
void Board::newBoard()
{
    pieceboard.piecemap.clear();
    for (int pos = 0; pos < 64; pos++)
    {
        if (pos == 0 || pos == 4 || pos == 11 || pos == 15)
        {
            pieceboard.piecemap[pos] = Piece(1, 1, pos);
        }
        else if (pos == 50 || pos == 54 || pos == 57 || pos == 61)
        {
            pieceboard.piecemap[pos] = Piece(2, 1, pos);
        }
        else if (pos == 48 || pos == 52 || pos == 59 || pos == 63)
        {
            pieceboard.piecemap[pos] = Piece(1, -1, pos);
        }
        else if (pos == 2 || pos == 6 || pos == 9 || pos == 13)
        {
            pieceboard.piecemap[pos] = Piece(2, -1, pos);
        }
    };
};
// Count the number of pieces
void Board::getPieceCount()
{
    for (auto x : pieceboard.piecemap)
    {
        const Piece &piece = x.second;
        switch (piece.piece)
        {
        case 1:
            switch (piece.color)
            {
            case 1:
                pieceboard.piececount.whiteSingles++;
                break;
            case -1:
                pieceboard.piececount.blackSingles++;
                break;
            };
            break;
        case 2:
            switch (piece.color)
            {
            case 1:
                pieceboard.piececount.whiteDoubles++;
                break;
            case -1:
                pieceboard.piececount.blackDoubles++;
                break;
            };
            break;
        };
    };
};
