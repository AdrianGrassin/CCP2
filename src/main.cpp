#include <iostream>
#include <string>
#include <vector>
#include "../include/PDA.h"
#include "../include/InputHandler.h"

int main(int argc, char* argv[]) {
    // Check for proper command-line arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <automaton_definition_file> <input_strings_file> [trace_mode] [output_file]\n";
        return 1;
    }

    std::string automatonDefinitionFile = argv[1];
    std::string inputStringsFile = argv[2];
    std::string traceMode = "none"; // Options: "none", "console", or filename
    if (argc >= 4) {
        if (std::string(argv[3]) == "trace") {
            traceMode = "console";
        } else if (std::string(argv[3]) == "tracefile" && argc >= 5) {
            traceMode = argv[4]; // Output file name
        }
    }

    // Create PDA instance (specify acceptance criteria)
    PDA pda(true); // true for final-state acceptance (APf), false for stack-empty acceptance (APv)

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

        if (traceMode == "none") {
            accepted = pda.processInput(input);
        } else {
            accepted = pda.processInputTrace(input, traceMode);
        }

        if (accepted) {
            std::cout << "Result: Accepted\n\n";
        } else {
            std::cout << "Result: Rejected\n\n";
        }
    }

    return 0;
}
