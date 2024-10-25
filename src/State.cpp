#include "../include/State.h"

// Default constructor
State::State() : name("") {
}

// Parameterized constructor
State::State(const std::string& name) : name(name) {
}

// Operator overloading for comparison
bool State::operator<(const State& other) const {
    return name < other.name;
}

bool State::operator==(const State& other) const {
    return name == other.name;
}
