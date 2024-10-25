/**
 * UNIVERSIDAD DE LA LAGUNA
 * ESCUELA SUPERIOR DE INGENIERÍA Y TECNOLOGÍA
 * Grado en Ingeniería Informática
 * Subject: Complejidad Computacional (CC)
 * Author: Adrián Grassin Luis
 **/

#include <iostream>
#include <string>
#include <vector>
#include "../include/PDA.h"
#include "../include/InputHandler.h"
#include "../include/Utility.h"



int main(int argc, char* argv[]) {
  // Variables to store command-line arguments
  std::string automatonDefinitionFile;
  std::string inputStringsFile;
  std::string mode = "none"; // Default mode
  std::string outputFile;
  std::string acceptanceType = "apf"; // Default acceptance type

  // Parse command-line arguments
  if (argc < 2) {
    displayHelp();
    return 1;
  }

  // Index to keep track of the current argument
  int argIndex = 1;

  while (argIndex < argc) {
    std::string arg = argv[argIndex];

    if (arg == "-h" || arg == "--help") {
      displayHelp();
      return 0;
    }
    else if (arg == "-m" || arg == "--mode") {
      if (argIndex + 1 < argc) {
        mode = argv[++argIndex];
      }
      else {
        std::cerr << "Error: Missing mode after " << arg << "\n";
        return 1;
      }
    }
    else if (arg == "-o" || arg == "--output") {
      if (argIndex + 1 < argc) {
        outputFile = argv[++argIndex];
      }
      else {
        std::cerr << "Error: Missing output file after " << arg << "\n";
        return 1;
      }
    }
    else if (arg == "-a" || arg == "--acceptance") {
      if (argIndex + 1 < argc) {
        acceptanceType = argv[++argIndex];
      }
      else {
        std::cerr << "Error: Missing acceptance type after " << arg << "\n";
        return 1;
      }
    }
    else if (automatonDefinitionFile.empty()) {
      automatonDefinitionFile = arg;
    }
    else if (inputStringsFile.empty()) {
      inputStringsFile = arg;
    }
    else {
      std::cerr << "Error: Unknown or extra argument '" << arg << "'\n";
      displayHelp();
      return 1;
    }
    argIndex++;
  }

  // Check if required files are provided
  if (automatonDefinitionFile.empty() || inputStringsFile.empty()) {
    std::cerr << "Error: Missing automaton definition file or input strings file.\n";
    displayHelp();
    return 1;
  }

  // Determine acceptance criteria
  bool useFinalStateAcceptance = true;
  if (acceptanceType == "apf") {
    useFinalStateAcceptance = true;
  }
  else if (acceptanceType == "apv") {
    useFinalStateAcceptance = false;
  }
  else {
    std::cerr << "Error: Invalid acceptance type '" << acceptanceType << "'. Use 'apf' or 'apv'.\n";
    return 1;
  }

  // Create PDA instance
  PDA pda(useFinalStateAcceptance);

  // Load automaton definition
  if (!InputHandler::loadAutomatonDefinition(pda, automatonDefinitionFile)) {
    std::cerr << "Failed to load automaton definition.\n";
    return 1;
  }

  // Load input strings
  std::vector<std::string> inputStrings = InputHandler::loadInputStrings(inputStringsFile);
  if (inputStrings.empty()) {
    std::cerr << "No input strings to process.\n";
    return 1;
  }

  // Process each input string
  for (const auto& input : inputStrings) {
    std::cout << "Processing input: " << input << "\n";
    bool accepted = false;

    if (mode == "none") {
      accepted = pda.processInput(input);
    }
    else if (mode == "trace") {
      accepted = pda.processInputTrace(input, "console");
    }
    else if (mode == "tracefile") {
      if (outputFile.empty()) {
        std::cerr << "Error: Output file not specified for tracefile mode.\n";
        return 1;
      }
      accepted = pda.processInputTrace(input, outputFile);
    }
    else {
      std::cerr << "Error: Invalid mode '" << mode << "'. Use 'none', 'trace', or 'tracefile'.\n";
      return 1;
    }

    if (accepted) {
      std::cout << "Result: Accepted\n\n";
    }
    else {
      std::cout << "Result: Rejected\n\n";
    }
  }

  return 0;
}
