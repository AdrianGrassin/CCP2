#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <iostream>

void displayHelp() {
    std::cout << "Usage: pda_simulator [options] <automaton_definition_file> <input_strings_file>\n";
    std::cout << "\nOptions:\n";
    std::cout << "  -h, --help            Display this help message and exit.\n";
    std::cout << "  -m, --mode <mode>     Specify the execution mode. Available modes:\n";
    std::cout << "                        none       : Regular execution mode (default).\n";
    std::cout << "                        trace      : Trace mode, output to console.\n";
    std::cout << "                        tracefile  : Trace mode, output to specified file.\n";
    std::cout << "  -o, --output <file>   Specify the output file for trace mode.\n";
    std::cout << "  -a, --acceptance <type> Specify acceptance criteria. Available types:\n";
    std::cout << "                        apf        : Acceptance by final state (default).\n";
    std::cout << "                        apv        : Acceptance by empty stack.\n";
    std::cout << "\nExamples:\n";
    std::cout << "  pda_simulator -m trace automaton.txt input.txt\n";
    std::cout << "  pda_simulator -m tracefile -o trace_output.txt automaton.txt input.txt\n";
    std::cout << "  pda_simulator -a apv automaton.txt input.txt\n";
    std::cout << "\n";
}


#endif // UTILITY_H