// File: InputHandler.cpp

#include "../include/InputHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

bool InputHandler::loadAutomatonDefinition(PDA& pda, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening automaton definition file: " << filename << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    // Skip comment lines (starting with '#') and empty lines
    auto skipCommentsAndEmptyLines = [&]() {
        while (std::getline(file, line)) {
            lineNumber++;
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); // Remove carriage returns
            if (line.empty() || line[0] == '#') {
                continue;
            } else {
                break;
            }
        }
    };

    // Load States
    skipCommentsAndEmptyLines();
    std::istringstream ss(line);
    std::string stateName;
    while (ss >> stateName) {
        pda.addState(stateName);
    }

    // Load Input Alphabet
    skipCommentsAndEmptyLines();
    ss.clear();
    ss.str(line);
    char inputSymbol;
    while (ss >> inputSymbol) {
        pda.addInputSymbol(inputSymbol);
    }

    // Load Stack Alphabet
    skipCommentsAndEmptyLines();
    ss.clear();
    ss.str(line);
    char stackSymbol;
    while (ss >> stackSymbol) {
        pda.addStackSymbol(stackSymbol);
    }

    // Load Initial State
    skipCommentsAndEmptyLines();
    ss.clear();
    ss.str(line);
    ss >> stateName;
    pda.setInitialState(stateName);

    // Load Initial Stack Symbol
    skipCommentsAndEmptyLines();
    ss.clear();
    ss.str(line);
    ss >> stackSymbol;
    pda.setInitialStackSymbol(stackSymbol);

    // Load Final States (if APf)
    skipCommentsAndEmptyLines();
    ss.clear();
    ss.str(line);
    while (ss >> stateName) {
        pda.addFinalState(stateName);
    }

    // Load Transitions
    while (std::getline(file, line)) {
        lineNumber++;
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); // Remove carriage returns
        if (line.empty() || line[0] == '#') {
            continue;
        }
        ss.clear();
        ss.str(line);
        std::string currentState, inputSymStr, stackSymStr, nextState, symbolsToPush;
        ss >> currentState >> inputSymStr >> stackSymStr >> nextState >> symbolsToPush;

        // Handle epsilon represented as '.'
        char inputSym = inputSymStr == "." ? '\0' : inputSymStr[0];
        char stackSym = stackSymStr == "." ? '\0' : stackSymStr[0];
        if (symbolsToPush == ".") {
            symbolsToPush = "";
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
    } else {
        std::cerr << "Error opening output file: " << filename << std::endl;
    }
}
