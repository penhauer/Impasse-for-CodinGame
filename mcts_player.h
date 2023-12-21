#pragma once

#include "common.h"
#include "game.h"
#include "player.h"
#include "random_player.h"
#include "state.h"

#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>


using namespace std;

class Node {
  private:
    double C = 1;
  public:
    int visits = 0;
    int wins = 0;
    State *state;
    Node *parent = NULL;
    std::vector<Node*> children;

    Node(State *state, Node *parent) : visits(0), state(state), parent(parent) {}

    double calculateUCBValue(bool ourTurn) {
      if (visits == 0) {
        return std::numeric_limits<double>::infinity();
      }
      double exploitation = 1.0 * wins / visits;
      if (!ourTurn)
        exploitation = 1 - exploitation;
      double exploration = C * sqrt(log(parent->visits) / visits);
      return exploitation + exploration;
    }
};


class MonteCarloTreeSearch {
  private:
    Player *randomPlayer1 = new RandomPlayer(WHITE);
    Player *randomPlayer2 = new RandomPlayer(BLACK);
    int color;
    Node* root;
    int inf = 10 * 1000 * 1000;
  public:
    MonteCarloTreeSearch() {}


    MonteCarloTreeSearch(int color, State *initialState) {
      this->color = color;
      root = new Node(initialState, NULL);
    }


    void updateRoot(std::string lastMove) {
      Node *newRoot = NULL;
      if (root->children.empty()) {
        expandNode(root);
      }
      for (auto x: root->children) {
        if (x->state->pieceboard.lastmove.toStr() == lastMove) {
          newRoot = x;
          break;
        }
      }
      assert(newRoot != NULL);
      root = newRoot;
    }

    void runMCTS(int iterations) {
      for (int i = 0; i < iterations; i++) {
        Node* selectedNode = selectNode(root, true);
        Game game = Game(selectedNode->state);
        int winner = game.gameLoop(randomPlayer1, randomPlayer2);
        backpropagate(selectedNode, winner);
      }
    }

    Node* selectNode(Node* node, bool ourTurn) {
      if (node->visits == 0) {
        return node;
      }

      if (node->children.empty()) {
        expandNode(node);
      }
      if (node->children.empty()) {
        assert(node->visits >= inf);
        return node;
      }

      double maxUCBValue = -std::numeric_limits<double>::infinity();
      Node* selectedChild = NULL;


      for (Node* child : node->children) {
        double ucbValue = child->calculateUCBValue(ourTurn);
        if (child->visits == 0)
          return child;
        if (ucbValue > maxUCBValue || selectedChild == NULL) {
          maxUCBValue = ucbValue;
          selectedChild = child;
        }
      }
      assert(selectedChild != NULL);
      return selectNode(selectedChild, !ourTurn);
    }


    void expandNode(Node* node) {
      for (auto pb : node->state->possiblepieceboards) {
        int t = node->state->turn ^ WHITE ^ BLACK;
        auto childState = new State(t, &pb);
        auto childNode = new Node(childState, node);
        auto winner = childState->pieceboard.winner;
        if (winner != BOARD_GAME_ONGOING) {
          childNode->visits = inf;
          childNode->wins = winner == color ? inf : 0;
        }
        node->children.push_back(childNode);
      }
    }

    void backpropagate(Node* node, int winner) {
      node->visits += 1;
      if (winner == color)
        node->wins += 1;
      if (node->parent != NULL)
        backpropagate(node->parent, winner);
    }

    int getBestMove() {
      int ind = -1;
      int maxi = 0;
      for (int i = 0; i < root->children.size(); i++)
        if (ind == -1 || root->children[i]->wins > maxi) {
          maxi = root->children[i]->wins;
          ind = i;
        }
      return ind;
    }

    void ourMove(int ind) {
      root = root->children[ind];
    }
};


class MCTSPlayer: public Player {
  private:
    MonteCarloTreeSearch mcts;

  public:
    MCTSPlayer(int color, State *state) : Player(color) {
      mcts = MonteCarloTreeSearch(color, state);
    };

    int decideOnBoard(State state, std::string lastMove);
};

