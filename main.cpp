#include <iostream>
#include <climits>
using namespace std;

// Forward declaration
 bool parseExpression(const char* expText,
                            int& position, int& value);

// Skip spaces in an expression
 void skipSpaces(const char* expText, int& position) {
    while (expText[position] == ' ' ||
           expText[position] == '\t' ||
           expText[position] == '\n' ||
           expText[position] == '\r') {
        ++position;
    }
}

// Read a number or an expression inside parentheses
 bool parseOperand(const char* expText,
                         int& position, int& value) {
    skipSpaces(expText, position);

    // Case 1: Read a number
    if (expText[position] >= '0' && expText[position] <= '9') {
        int number = 0;

        while (expText[position] >= '0' &&
               expText[position] <= '9') {

            int digit = expText[position] - '0';
            // Check for integer overflow
            if (number > (INT_MAX - digit) / 10) {
                return false;
            }
            number = number * 10 + digit;
            ++position;
        }

        value = number;
        return true;
    }

    // Case 2: Read an expression inside parentheses
    if (expText[position] == '(') {
        ++position; // Move past '('

        // Evaluate the expression inside parentheses
        if (!parseExpression(expText, position, value)) {
            return false;
        }

        skipSpaces(expText, position);

        // Check for closing parenthesis
        if (expText[position] != ')') {
            return false;
        }

        ++position; // Move past ')'
        return true;
    }

    return false; // Invalid or missing operand
}

// Perform the mathematical operation
 bool applyOperator(char op, int lhs,
                          int rhs, int& result) {
       long long operand1 = lhs;
       long long operand2 = rhs;
       long long answer = 0;

    if (op == '+') {
        answer = operand1 + operand2;
    }
    else if (op == '-') {
        answer = operand1 - operand2;
    }
    else if (op == '*') {
        answer = operand1 * operand2;
    }
    else {
        if (operand2 == 0) {//Invalid condition
            return false;
        }
        answer = operand1 / operand2;
    }
     // Check if result fits in an integer
     if (answer < INT_MIN || answer > INT_MAX) {
         return false;
     }
     result = static_cast<int>(answer);
    return true;
}

// Evaluate expression strictly from left to right
 bool parseExpression(const char* exp_text,
                            int& position, int& value) {

    // Read first operand
    if (!parseOperand(exp_text, position, value)) {
        return false;
    }

    while (true) {
        skipSpaces(exp_text, position);

        char op = exp_text[position];

        // Stop if the next character is not an operator
        if (op != '+' && op != '-' &&
            op != '*' && op != '/') {
            return true;
        }

        ++position; // Move past operator

        // Read the next operand
        int rhs = 0;

        if (!parseOperand(exp_text, position, rhs)) {
            return false;
        }
        int lhs= value;
        // Calculate immediately (left-to-right)
        if (!applyOperator(op, lhs, rhs, value)) {
            return false;
        }
    }
}

// Main evaluation function
bool evaluate(const char* expression, int& result) {

    if (expression == nullptr) {
        return false;
    }

    // Start reading at index 0
    int position = 0;
    int value = 0;

    // Evaluate expression
    if (!parseExpression(expression, position, value)) {
        return false;
    }

    // Ignore any trailing spaces
    skipSpaces(expression, position);

    // Check that the entire expression was processed
    if (expression[position] != '\0') {
        return false;
    }

    // Only update result when evaluation succeeds
    result = value;
    return true;
}


// Execute one test and compare with expected output
 void runTest(const char* expression) {

    int value = 0;
    bool valid = evaluate(expression, value);

     if (valid) {
         cout << expression<<" = "<<value<<" "<<boolalpha<<valid<<endl;
     }
     else {
         cout << (expression != nullptr
             ? expression : "(null)");
         cout <<" "<<boolalpha<<valid<<endl;
     }
}

int main() {

     const char* tests[] = {

         // Examples from specification
         "1 + 3",
         "(1 + 3) * 2",
         "(4 / 2) + 6",
         "4 + (12 / (1 * 2))",
         "(1 + (12 * 2)",

         // Left-to-right calculations
         "1 + 3 * 4",

         // Numbers, parentheses and whitespace
         "42",
         "((7))",
         "   1 + 2   ",

         // Integer results
         "1 - 5",
         "7 / 2",

         // Invalid expressions
         nullptr,
         "",
         "1 +",
         "+ 1",
         "1 + + 2",
         "1 2",
         "()",
         "(1 + 2))",
         "-1",

         // Invalid characters
         "1 + a",
         "1 + 2abc",

         // Arithmetic errors
         "1 / 0",
         "2147483648",
         "2147483647 + 1",
     };


    for (const  char* expression : tests) {
        runTest(expression);
    }

    return 0 ;
}
