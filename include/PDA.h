#ifndef PDA_H
#define PDA_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include "State.h"
#include "Transition.h"
#include "Stack.h"

class PDA {
private:
    // Automaton components
    std::set<State> states;
    std::set<char> inputAlphabet;
    std::set<char> stackAlphabet;
    State initialState;
    char initialStackSymbol;
    std::set<State> finalStates;
    std::vector<Transition> transitions;

    // Acceptance criteria
    bool useFinalStateAcceptance; // true for APf, false for APv (stack-empty acceptance)

public:
    // Constructor
    PDA(bool useFinalStateAcceptance = true);

    // Methods to build the PDA
    void addState(const std::string& stateName);
    void addInputSymbol(char symbol);
    void addStackSymbol(char symbol);
    void setInitialState(const std::string& stateName);
    void setInitialStackSymbol(char symbol);
    void addFinalState(const std::string& stateName);
    void addTransition(const std::string& currentState, char inputSymbol, char stackSymbol,
                       const std::string& nextState, const std::string& symbolsToPush);
    std::set<char> getInputAlphabet() const;
    std::set<State> getStates() const;
    std::set<char> getStackAlphabet() const;
    bool getUseFinalStateAcceptance() const;

    // Processing input strings
    bool processInput(const std::string& input);
    bool processInputTrace(const std::string& input, const std::string& outputMode);



private:
    // Helper methods
    bool processRecursive(const State& currentState, const std::string& remainingInput, Stack& stack);
    bool processRecursiveTrace(const State& currentState, const std::string& remainingInput, Stack& stack,
                               std::string& trace, int depth = 0);

    std::vector<Transition> getPossibleTransitions(const State& state, char inputSymbol, char stackSymbol);
};

#endif // PDA_H
