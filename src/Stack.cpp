
#include "../include/Stack.h"

Stack::Stack(char initialSymbol) {
    stackContainer.push_back(initialSymbol);
}

void Stack::push(const std::string& symbols) {
    // Push symbols onto the stack in reverse order
    for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
        stackContainer.push_back(*it);
    }
}

char Stack::pop() {
    if (!stackContainer.empty()) {
        char top = stackContainer.back();
        stackContainer.pop_back();
        return top;
    } else {
        // Return a special character or handle error
        return '\0'; // Assuming '\0' signifies an empty stack
    }
}

char Stack::peek() const {
    if (!stackContainer.empty()) {
        return stackContainer.back();
    } else {
        return '\0'; // Stack is empty
    }
}

bool Stack::isEmpty() const {
    return stackContainer.empty();
}

void Stack::display() const {
    std::cout << "Stack contents (top to bottom): ";
    for (auto it = stackContainer.rbegin(); it != stackContainer.rend(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
}
