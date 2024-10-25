#include "../include/Transition.h"

Transition::Transition(const State& currentState,
                       char inputSymbol,
                       char stackSymbol,
                       const State& nextState,
                       const std::string& symbolsToPush)
  : currentState(currentState),
    inputSymbol(inputSymbol),
    stackSymbol(stackSymbol),
    nextState(nextState),
    symbolsToPush(symbolsToPush) {}

// Getters
State Transition::getCurrentState() const {
  return currentState;
}

char Transition::getInputSymbol() const {
  return inputSymbol;
}

char Transition::getStackSymbol() const {
  return stackSymbol;
}

State Transition::getNextState() const {
  return nextState;
}

std::string Transition::getSymbolsToPush() const {
  return symbolsToPush;
}

