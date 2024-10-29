#include <iostream>
#include <string>
#include <cmath>

/* 
    Function: is_number
    Description: Checks if the user input has numbers
    Returns(bool): True or false
*/
bool is_number(std::string str) {
    int num_digits = 0;
    int num_points = 0;
    for (int i = 0; i < str.length(); i++) {
        bool is_point = str.at(i) == '.';
        bool is_negative_sign = str.at(i) == '-' && i == 0;
        bool is_number = str.at(i) >= '0' && str.at(i) <= '9';

        if (is_point) {
            num_points++;
            if (num_points > 1) {
                return false;
            }
        }
        if (is_number) {
            num_digits++;
        }

        if (!is_point && !is_negative_sign && !is_number) {
            return false;
        }
    }

    return num_digits > 0;
}

/*
 * Function: precedence
 * Description: Returns the precedence of the given operator
 */
int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    if (op == '^') {
        return 3;
    }
    return 0;
}

/*
 * Function: apply_op
 * Description: Applies the given operator to two operands and returns the result
 */
double apply_op(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return std::pow(a, b);
    }
    return 0;
}

/*
 * Function: is_valid_expression
 * Description: Determines whether a given string holds a valid arithmetic expression
 * Returns: True if the given string holds a valid arithmetic expression, or false otherwise.
 */
bool is_valid_expression(std::string expression) {
    if (expression.empty()) return false;

    bool expect_number = true;
    bool expect_operator = false;
    bool last_char_was_space = false;
    int parentheses_balance = 0;
    std::string token;

    for (size_t i = 0; i <= expression.length(); ++i) {
        if (i == expression.length() || expression[i] == ' ') {
            if (last_char_was_space) return false;
            last_char_was_space = true;
            if (!token.empty()) {
                if (expect_number) {
                    if (!is_number(token)) {
                        return false;
                    }
                    expect_number = false;
                    expect_operator = true;
                } else if (expect_operator) {
                    if (token != "+" && token != "-" && token != "*" && token != "/" && token != "^") {
                        return false;
                    }
                    expect_number = true;
                    expect_operator = false;
                }
                token.clear();
            }
        } else {
            last_char_was_space = false;
            if (expression[i] == '(') {
                if (!expect_number) return false;
                ++parentheses_balance;
                expect_number = true;
                expect_operator = false;
            } else if (expression[i] == ')') {
                if (expect_number || parentheses_balance == 0) return false;
                --parentheses_balance;
                expect_operator = true;
            } else {
                token += expression[i];
            }
        }
    }

    return parentheses_balance == 0 && !expect_number;
}

/*
 * Function: compute_value
 * Description: Computes and returns the value of the given arithmetic expression.
 * Parameters:
 *      expression (std::string): A string holding the arithmetic expression to be evaluated.
 * Returns (double): The value of the given arithmetic expression.
 */
double compute_value(std::string expression) {
    double values[100];  // Array to store values
    char ops[100];       // Array to store operators
    int values_index = 0; // Index for values array
    int ops_index = 0;    // Index for operators array

    std::string token;

    for (size_t i = 0; i <= expression.length(); i++) {
        if (i == expression.length() || expression[i] == ' ') {
            if (!token.empty()) {
                if (is_number(token)) {
                    values[values_index++] = std::stod(token);
                } else {
                    while (ops_index > 0 && precedence(ops[ops_index - 1]) >= precedence(token[0])) {
                        double val2 = values[--values_index];
                        double val1 = values[--values_index];
                        char op = ops[--ops_index];
                        values[values_index++] = apply_op(val1, val2, op);
                    }
                    ops[ops_index++] = token[0];
                }
                token.clear();
            }
        } else if (expression[i] == '(') {
            ops[ops_index++] = expression[i];
        } else if (expression[i] == ')') {
            while (ops_index > 0 && ops[ops_index - 1] != '(') {
                double val2 = values[--values_index];
                double val1 = values[--values_index];
                char op = ops[--ops_index];
                values[values_index++] = apply_op(val1, val2, op);
            }
            if (ops_index > 0) {
                ops_index--;
            }
        } else {
            token += expression[i];
        }
    }

    while (ops_index > 0) {
        double val2 = values[--values_index];
        double val1 = values[--values_index];
        char op = ops[--ops_index];
        values[values_index++] = apply_op(val1, val2, op);
    }

    return values[0];
}
