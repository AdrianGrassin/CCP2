#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <string>
#include <vector>
#include "PDA.h"

class InputHandler {
public:
    static bool loadAutomatonDefinition(PDA& pda, const std::string& filename);
    static std::vector<std::string> loadInputStrings(const std::string& filename);
    static void writeTraceOutput(const std::string& trace, const std::string& filename);
};

#endif // INPUTHANDLER_H
