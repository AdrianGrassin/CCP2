// File: InputHandler.cpp

#include "../include/InputHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

bool InputHandler::loadAutomatonDefinition(PDA& pda, const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo de definición del autómata: " << filename << std::endl;
    return false;
  }

  std::string line;
  int lineNumber = 0;

  // Lambda function to skip comments and empty lines
  auto skipCommentsAndEmptyLines = [&]() {
    while (std::getline(file, line)) {
      lineNumber++;
      // Remove carriage returns and whitespace from both ends
      line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
      line.erase(0, line.find_first_not_of(" \t")); // Trim leading whitespace
      line.erase(line.find_last_not_of(" \t") + 1); // Trim trailing whitespace
      if (line.empty() || line[0] == '#') {
        continue;
      }
      else {
        break;
      }
    }
    };

  std::istringstream ss;
  std::string stateName;
  char symbol;

  // Load States
  skipCommentsAndEmptyLines();
  if (file.eof()) {
    std::cerr << "Error: Se esperaba una lista de estados en la línea " << lineNumber << "." << std::endl;
    return false;
  }
  ss.clear();
  ss.str(line);
  while (ss >> stateName) {
    pda.addState(stateName);
  }
  if (pda.getStates().empty()) {
    std::cerr << "Error: No se definieron estados en el autómata." << std::endl;
    return false;
  }

  // Load Input Alphabet
  skipCommentsAndEmptyLines();
  if (file.eof()) {
    std::cerr << "Error: Se esperaba un alfabeto de entrada en la línea " << lineNumber << "." << std::endl;
    return false;
  }
  ss.clear();
  ss.str(line);
  while (ss >> symbol) {
    pda.addInputSymbol(symbol);
  }

  // Load Stack Alphabet
  skipCommentsAndEmptyLines();
  if (file.eof()) {
    std::cerr << "Error: Se esperaba un alfabeto de pila en la línea " << lineNumber << "." << std::endl;
    return false;
  }
  ss.clear();
  ss.str(line);
  while (ss >> symbol) {
    pda.addStackSymbol(symbol);
  }

  // Load Initial State
  skipCommentsAndEmptyLines();
  if (file.eof()) {
    std::cerr << "Error: Se esperaba un estado inicial en la línea " << lineNumber << "." << std::endl;
    return false;
  }
  ss.clear();
  ss.str(line);
  ss >> stateName;
  if (!pda.getStates().count(State(stateName))) {
    std::cerr << "Error: El estado inicial '" << stateName << "' no está definido en los estados del autómata." << std::endl;
    return false;
  }
  pda.setInitialState(stateName);

  // Load Initial Stack Symbol
  skipCommentsAndEmptyLines();
  if (file.eof()) {
    std::cerr << "Error: Se esperaba un símbolo inicial de pila en la línea " << lineNumber << "." << std::endl;
    return false;
  }
  ss.clear();
  ss.str(line);
  ss >> symbol;
  if (!pda.getStackAlphabet().count(symbol)) {
    std::cerr << "Error: El símbolo inicial de pila '" << symbol << "' no está definido en el alfabeto de pila." << std::endl;
    return false;
  }
  pda.setInitialStackSymbol(symbol);

  // Load Final States (if APf)
  if (pda.getUseFinalStateAcceptance()) {
    skipCommentsAndEmptyLines();
    if (file.eof()) {
      std::cerr << "Error: Se esperaba una lista de estados finales en la línea " << lineNumber << "." << std::endl;
      return false;
    }
    ss.clear();
    ss.str(line);
    bool hasFinalState = false;
    while (ss >> stateName) {
      if (!pda.getStates().count(State(stateName))) {
        std::cerr << "Error: El estado final '" << stateName << "' no está definido en los estados del autómata." << std::endl;
        return false;
      }
      pda.addFinalState(stateName);
      hasFinalState = true;
    }
    if (!hasFinalState) {
      std::cerr << "Error: No se definieron estados finales en el autómata." << std::endl;
      return false;
    }
  }

  // Load Transitions
  while (std::getline(file, line)) {
    lineNumber++;
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(0, line.find_first_not_of(" \t")); // Trim leading whitespace
    line.erase(line.find_last_not_of(" \t") + 1); // Trim trailing whitespace
    if (line.empty() || line[0] == '#') {
      continue;
    }
    ss.clear();
    ss.str(line);
    std::string currentState, inputSymStr, stackSymStr, nextState, symbolsToPush;
    if (!(ss >> currentState >> inputSymStr >> stackSymStr >> nextState >> symbolsToPush)) {
      std::cerr << "Error: Formato incorrecto en la línea " << lineNumber << ": '" << line << "'" << std::endl;
      return false;
    }

    // Validate currentState and nextState
    if (!pda.getStates().count(State(currentState))) {
      std::cerr << "Error: El estado actual '" << currentState << "' no está definido en los estados del autómata (línea " << lineNumber << ")." << std::endl;
      return false;
    }
    if (!pda.getStates().count(State(nextState))) {
      std::cerr << "Error: El estado siguiente '" << nextState << "' no está definido en los estados del autómata (línea " << lineNumber << ")." << std::endl;
      return false;
    }

    // Handle epsilon represented as '.'
    char inputSym = inputSymStr == "." ? '\0' : inputSymStr[0];
    char stackSym = stackSymStr == "." ? '\0' : stackSymStr[0];

    // Validate input symbol if not epsilon
    if (inputSym != '\0' && !pda.getInputAlphabet().count(inputSym)) {
      std::cerr << "Error: El símbolo de entrada '" << inputSym << "' no está en el alfabeto de entrada (línea " << lineNumber << ")." << std::endl;
      return false;
    }

    // Validate stack symbol if not epsilon
    if (stackSym != '\0' && !pda.getStackAlphabet().count(stackSym)) {
      std::cerr << "Error: El símbolo de pila '" << stackSym << "' no está en el alfabeto de pila (línea " << lineNumber << ")." << std::endl;
      return false;
    }

    // Validate symbols to push
    if (symbolsToPush == ".") {
      symbolsToPush = "";
    }
    else {
      for (char c : symbolsToPush) {
        if (!pda.getStackAlphabet().count(c)) {
          std::cerr << "Error: El símbolo a apilar '" << c << "' no está en el alfabeto de pila (línea " << lineNumber << ")." << std::endl;
          return false;
        }
      }
    }

    pda.addTransition(currentState, inputSym, stackSym, nextState, symbolsToPush);
  }

  file.close();
  return true;
}


std::vector<std::string> InputHandler::loadInputStrings(const std::string& filename) {
  std::vector<std::string> inputStrings;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening input strings file: " << filename << std::endl;
    return inputStrings;
  }

  std::string line;
  while (std::getline(file, line)) {
    // Remove whitespace and comments
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    if (!line.empty() && line[0] != '#') {
      inputStrings.push_back(line);
    }
  }

  file.close();
  return inputStrings;
}

void InputHandler::writeTraceOutput(const std::string& trace, const std::string& filename) {
  std::ofstream file(filename);
  if (file.is_open()) {
    file << trace;
    file.close();
  }
  else {
    std::cerr << "Error opening output file: " << filename << std::endl;
  }
}
