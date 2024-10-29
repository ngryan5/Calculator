/*
 * Author: Ryan Nguyen
 * Description: This program is a simple terminal-based calculator supporting basic arithmetic operations
(addition, subtraction, multiplication, and division).
 */

#include <iostream>

#include "calculator.hpp"

/*
 * Function: print_error
 * Description: Prints an error message explaining that the user supplied
 * 		an invalid arithmetic expression.
 */
void print_error() {
	std::cout << "That isn't a valid arithmetic expression." << std::endl;
}

/*
 * Function: prompt_for_arithmetic_expression
 * Description: Prompts the user for an arithmetic expression and returns
 * 		whatever text they supply, whether that text represents a valid
 * 		arithmetic expression or not (validity should be checked elsewhere;
 * 		checking validity is not this particular function's responsibility).
 * Returns: An entire line of text supplied by the user via the terminal when
 * 		prompted for an arithmetic expression
 */
std::string prompt_for_arithmetic_expression() {
	std::cout << "Enter a valid arithmetic expression: ";
	std::string user_input;
	std::getline(std::cin, user_input);
	return user_input;
}

/*
 * Function: prompt_for_retry
 * Description: Asks the user whether they'd like to enter another arithmetic
 * 		expression and returns their entire line-of-text response
 * Returns: An entire line of text supplied by the user via the terminal when
 * 		asked if they'd like to enter another arithmetic expression. If this
 * 		function returns the string "Y", then that means the user would like
 * 		to enter another arithmetic expression.
 */
std::string prompt_for_retry() {
	std::cout << "Would you like to enter another expression? Enter Y for "
		"yes: ";

	std::string user_input;
	std::getline(std::cin, user_input);
	return user_input;
}

/*
 * Function: print_history_header
 * Description: Prints the header that precedes the history of values to be
 * 		printed at the end of the program.
 */
void print_history_header() {
	std::cout << "History of computed values:" << std::endl;
}

/* Function: check expression
	Description: checks whether the user input was a valid expression
	returns: true or false
*/
bool check_expression(std::string user_expression) {
	if (user_expression.empty()) {
		return false;
	} else if (!isdigit(user_expression[0]) && user_expression[0] != '-') {
		return false;
	} else if (!isdigit(user_expression[user_expression.length() - 1])) {
		return false;
	} else {
		bool number_flag = true;
		bool operator_flag = false;

		for (int i = 0; i < user_expression.length(); i++) {
			if (isspace(user_expression[i])) {
				if (!number_flag) {
					number_flag = true;
				} else if (number_flag) {
					number_flag = false;
				}
				continue;
			} 

			if (number_flag) {
				if (!isdigit(user_expression[i]) && user_expression[i] != '-' && user_expression[i] != '.') {
					return false;
				}

				operator_flag = false;
			} else if (!number_flag) {
				if (operator_flag)
					return false;

				if (user_expression[i] != '+' && user_expression[i] != '-' && user_expression[i] != '*' && user_expression[i] != '/') {
					return false;
				}
				operator_flag = true;
			}
		}
	}
	return true;
}
/* 
	Function: perform_operations
	Description: performs the mathematical operation
	Returns (double): performed operation
*/
double perform_operations(char op, double num, double result) {
	if (op == '+') {
		result += num;
	} else if (op == '-') {
		result -= num;
	} else if (op == '*') {
		result  *= num;
	} else if (op == '/') {
		result /= num;
	} else {
		std::cout << "Error: dividion by zero" << std::endl;
		return 0;
	}

	return result;
}

/*
	Function: calculated_expression
	Description: Calculates the mathmatical expression given by the user
	Returns (double): resultant number
*/
double calculated_expression(std::string user_expression) {
	double result = 0;
	char op = '+';
	double num = 0;
	bool number_flag = true;
	std::string temp_string;

	for (int i = 0; i < user_expression.length(); i++) {
		if (isspace(user_expression[i])) {
			if (!number_flag) {
				number_flag = true;
			} else if (number_flag)
				number_flag = false;

			continue;
		}
		if (!number_flag) {
			num = std::stod(temp_string);
			result = perform_operations(op, num, result);
			temp_string.clear();
			op = user_expression[i];
		} else if (number_flag) {
			temp_string += user_expression[i];
		}
	}

	num = std::stod(temp_string);
	result = perform_operations(op, num, result);

	return result;
}
/*
	Function: print_history
	Description: prints the history of inputs and results
*/
void print_history (double computed_values[], int num_computed_values) {
	print_history_header();
	for (int i = 0; i < num_computed_values; i++) {
		std::cout << computed_values[i] << std::endl;
	}
}
int main() {
	// Array to represent history of at most 100 computed values
    double computed_values[100];
    int num_computed_values = 0;

    std::string go_again;
    do {
        std::string expression = prompt_for_arithmetic_expression();

        // Check if expression is valid by using the is_valid_expression
        // function, to be defined in calculator.cpp
        if (is_valid_expression(expression)) {
            // Expression is valid. Compute its value by using the
            // compute_value function, to be defined in calculator.cpp.
            double value = compute_value(expression);

            // Print value
            std::cout << value << std::endl;

            // Append value to array containing history of computed values
            // and increment the array's corresponding size variable
            if (num_computed_values < 100) {
                computed_values[num_computed_values++] = value;
            }

            go_again = prompt_for_retry();
        } else {
            std::cout << "That isn't a valid arithmetic expression." << std::endl;
            go_again = "Y";
        }
    } while(go_again == "Y");

    // Print history of computed values
    print_history(computed_values, num_computed_values);

    return 0;
}
