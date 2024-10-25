#ifndef STATE_H
#define STATE_H

#include <string>

class State {
  public:
    // Default constructor
    State();

    // Parameterized constructor
    State(const std::string& name);

    // Getter
    std::string getName() const;

    // Overloaded operators for comparison (needed for use in sets)
    bool operator<(const State& other) const;
    bool operator==(const State& other) const;

  private:
    std::string name;
};

#endif // STATE_H
