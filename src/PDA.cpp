#include "../include/PDA.h"
#include <iostream>
#include <fstream>
#include <algorithm>


// Constructor
PDA::PDA(bool useFinalStateAcceptance)
  : useFinalStateAcceptance(useFinalStateAcceptance)
{
}

// Methods to build the PDA
void PDA::addState(const std::string& stateName)
{
  states.insert(State(stateName));
}

void PDA::addInputSymbol(char symbol)
{
  inputAlphabet.insert(symbol);
}

void PDA::addStackSymbol(char symbol)
{
  stackAlphabet.insert(symbol);
}

void PDA::setInitialState(const std::string& stateName)
{
  initialState = State(stateName);
}

void PDA::setInitialStackSymbol(char symbol)
{
  initialStackSymbol = symbol;
}

void PDA::addFinalState(const std::string& stateName)
{
  finalStates.insert(State(stateName));
}

void PDA::addTransition(const std::string& currentState, char inputSymbol, char stackSymbol,
  const std::string& nextState, const std::string& symbolsToPush)
{
  transitions.push_back(Transition(State(currentState), inputSymbol, stackSymbol,
    State(nextState), symbolsToPush));
}

std::set<char> PDA::getInputAlphabet() const
{
  return inputAlphabet;
}

std::set<State> PDA::getStates() const
{
  return states;
}

std::set<char> PDA::getStackAlphabet() const
{
  return stackAlphabet;
}

bool PDA::getUseFinalStateAcceptance() const
{
return useFinalStateAcceptance;
}
// Processing input strings
bool PDA::processInput(const std::string& input)
{
  Stack stack(initialStackSymbol);
  return processRecursive(initialState, input, stack);
}


bool PDA::processRecursive(const State& currentState, const std::string& remainingInput, Stack& stack)
{
  // Base case: check acceptance
  if (remainingInput.empty())
  {
    if (useFinalStateAcceptance)
    {
      if (finalStates.count(currentState) > 0)
      {
        return true;
      }
    }
    else
    { // Stack-empty acceptance
      if (stack.isEmpty())
      {
        return true;
      }
    }
  }

  // Get the possible transitions from the current state
  char inputSymbol = remainingInput.empty() ? '\0' : remainingInput[0];
  char stackSymbol = stack.isEmpty() ? '\0' : stack.peek();

  // Consider both the current input symbol and epsilon (represented as '\0')
  std::vector<Transition> possibleTransitions = getPossibleTransitions(currentState, inputSymbol, stackSymbol);
  if (inputSymbol != '\0')
  {
    std::vector<Transition> epsilonTransitions = getPossibleTransitions(currentState, '\0', stackSymbol);
    possibleTransitions.insert(possibleTransitions.end(), epsilonTransitions.begin(), epsilonTransitions.end());
  }

  // Explore each possible transition
  for (const auto& transition : possibleTransitions)
  {
    // Copy the stack and input for the new path
    Stack newStack = stack;
    std::string newRemainingInput = remainingInput;

    // Input consumption
    if (transition.getInputSymbol() != '\0')
    {
      if (!newRemainingInput.empty() && newRemainingInput[0] == transition.getInputSymbol())
      {
        newRemainingInput = newRemainingInput.substr(1);
      }
      else
      {
        continue; // Input symbol does not match
      }
    }

    // Stack operation
    if (transition.getStackSymbol() != '\0')
    {
      if (!newStack.isEmpty() && newStack.peek() == transition.getStackSymbol())
      {
        newStack.pop();
      }
      else
      {
        continue; // Stack symbol does not match
      }
    }

    // Push symbols onto the stack
    if (!transition.getSymbolsToPush().empty())
    {
      newStack.push(transition.getSymbolsToPush());
    }

    // Recursive call
    if (processRecursive(transition.getNextState(), newRemainingInput, newStack))
    {
      return true;
    }
  }

  // No accepting path found
  return false;
}

bool PDA::processInputTrace(const std::string& input, const std::string& outputMode)
{
  Stack stack(initialStackSymbol);
  std::string trace;
  bool accepted = processRecursiveTrace(initialState, input, stack, trace);

  // Output the trace
  if (outputMode == "console")
  {
    std::cout << trace;
  }
  else
  {
    // Assume outputMode is a filename
    std::ofstream file(outputMode);
    if (file.is_open())
    {
      file << trace;
      file.close();
    }
    else
    {
      std::cerr << "Error opening output file: " << outputMode << std::endl;
    }
  }
  return accepted;
}

bool PDA::processRecursiveTrace(const State& currentState, const std::string& remainingInput, Stack& stack,
  std::string& trace, int depth)
{
  // Indentation for the current depth
  std::string indent(depth * 2, ' ');

  // Record the current configuration
  trace += indent + "State: " + currentState.getName() + ", Input: " + remainingInput +
    ", Stack: " + stack.display();

  trace += "\n";

  // Base case: check acceptance
  if (remainingInput.empty())
  {
    if (useFinalStateAcceptance && finalStates.count(currentState) > 0)
    {
      trace += indent + "Accepted (final state reached)\n";
      return true;
    }
    if (!useFinalStateAcceptance && stack.isEmpty())
    {
      trace += indent + "Accepted (stack empty)\n";
      return true;
    }
  }

  bool accepted = false;

  // Get the possible transitions from the current state
  char inputSymbol = remainingInput.empty() ? '\0' : remainingInput[0];
  char stackSymbol = stack.isEmpty() ? '\0' : stack.peek();

  // Consider both the current input symbol and epsilon
  std::vector<Transition> possibleTransitions = getPossibleTransitions(currentState, inputSymbol, stackSymbol);
  if (inputSymbol != '\0')
  {
    std::vector<Transition> epsilonTransitions = getPossibleTransitions(currentState, '\0', stackSymbol);
    possibleTransitions.insert(possibleTransitions.end(), epsilonTransitions.begin(), epsilonTransitions.end());
  }

  // Explore each possible transition
  for (const auto& transition : possibleTransitions)
  {
    // Copy the stack and input for the new path
    Stack newStack = stack;
    std::string newRemainingInput = remainingInput;
    std::string transitionInfo = indent + "Applying transition: (" + transition.getCurrentState().getName() + ", ";

    // Input consumption
    if (transition.getInputSymbol() != '\0')
    {
      transitionInfo += transition.getInputSymbol();
      if (!newRemainingInput.empty() && newRemainingInput[0] == transition.getInputSymbol())
      {
        newRemainingInput = newRemainingInput.substr(1);
      }
      else
      {
        continue; // Input symbol does not match
      }
    }
    else
    {
      transitionInfo += "ε";
    }

    transitionInfo += ", ";

    // Stack operation
    if (transition.getStackSymbol() != '\0')
    {
      transitionInfo += transition.getStackSymbol();
      if (!newStack.isEmpty() && newStack.peek() == transition.getStackSymbol())
      {
        newStack.pop();
      }
      else
      {
        continue; // Stack symbol does not match
      }
    }
    else
    {
      transitionInfo += "ε";
    }

    transitionInfo += ") -> (" + transition.getNextState().getName() + ", ";

    // Symbols to push
    transitionInfo += transition.getSymbolsToPush().empty() ? "ε" : transition.getSymbolsToPush();
    transitionInfo += ")";

    trace += transitionInfo + "\n";

    // Push symbols onto the stack
    if (!transition.getSymbolsToPush().empty())
    {
      newStack.push(transition.getSymbolsToPush());
    }

    // Recursive call
    if (processRecursiveTrace(transition.getNextState(), newRemainingInput, newStack, trace, depth + 1))
    {
      accepted = true;
      break;
    }
  }

  if (!accepted)
  {
    trace += indent + "Backtracking...\n";
  }

  return accepted;
}

std::vector<Transition> PDA::getPossibleTransitions(const State& state, char inputSymbol, char stackSymbol)
{
  std::vector<Transition> possibleTransitions;
  for (const auto& transition : transitions)
  {
    if (transition.getCurrentState() == state)
    {
      bool inputMatches = (transition.getInputSymbol() == inputSymbol);
      bool stackMatches = (transition.getStackSymbol() == stackSymbol);

      // Handle epsilon transitions
      if (transition.getInputSymbol() == '\0')
      {
        inputMatches = true;
      }
      if (transition.getStackSymbol() == '\0')
      {
        stackMatches = true;
      }

      if (inputMatches && stackMatches)
      {
        possibleTransitions.push_back(transition);
      }
    }
  }
  return possibleTransitions;
}
