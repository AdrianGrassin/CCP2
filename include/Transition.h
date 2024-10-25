#ifndef TRANSITION_H
#define TRANSITION_H

#include <string>
#include "State.h"

class Transition {
  public:
    Transition(const State& currentState,
               char inputSymbol,
               char stackSymbol,
               const State& nextState,
               const std::string& symbolsToPush);

    State getCurrentState() const;
    char getInputSymbol() const;
    char getStackSymbol() const;
    State getNextState() const;
    std::string getSymbolsToPush() const;


    private:
      State currentState;
      char inputSymbol;
      char stackSymbol;
      State nextState;
      std::string symbolsToPush;
};

#endif // TRANSITION_H