#ifndef STACK_H
#define STACK_H

#include <vector>
#include <string>
#include <iostream>

class Stack {
private:
  std::vector<char> stackContainer;

public:
  Stack(char initialSymbol);
  void push(const std::string& symbols);
  char pop();
  char peek() const;
  bool isEmpty() const;
  std::string display() const;
};

#endif // STACK_H

