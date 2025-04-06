#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath> // For pow function

// Structure to represent a custom instruction
struct Instruction
{
    std::string operation; // e.g., "ADD", "MUL", etc.
    std::string operand1;  // First operand
    std::string operand2;  // Second operand (optional)
    std::string result;    // Result variable
};

class SimpleParser
{
private:
    const char *input;
    int pos;
    int tempVarCount; // Temporary variable counter
    std::vector<Instruction> instructions;

    // Helper function to get the current character
    char currentChar() const
    {
        return input[pos];
    }

    // Helper function to advance to the next character
    void advance()
    {
        pos++;
    }

    // Helper function to consume whitespace
    void skipWhitespace()
    {
        while (currentChar() == ' ')
        {
            advance();
        }
    }

    // Function to generate a new temporary variable
    std::string getTempVar()
    {
        return "temp" + std::to_string(tempVarCount++);
    }

    // Parses a number and returns it as a string
    std::string parseNumber()
    {
        skipWhitespace();
        std::string number;
        while (isdigit(currentChar()) || currentChar() == '.')
        {
            number += currentChar();
            advance();
        }
        return number; // Return the parsed number as a string
    }

    // Parses factors (numbers, variables, or expressions in parentheses)
    std::string parseFactor()
    {
        skipWhitespace();
        std::string result;

        if (currentChar() == '(')
        {
            advance();                              // Skip '('
            result = parseExpression();             // Parse the inner expression
            if (currentChar() == ')')
            {
                advance(); // Skip ')'
            }
            else
            {
                throw std::runtime_error("Error: Missing closing parenthesis");
            }
        }
        else if (isdigit(currentChar()) || currentChar() == '.')
        {
            result = parseNumber(); // Parse and return a number

            // Check for implicit multiplication (e.g., "2x" or "2(x)")
            skipWhitespace();
            if (isalpha(currentChar()) || currentChar() == '(')
            {
                std::string temp = getTempVar();
                std::string right = parseFactor(); // Parse the next factor
                instructions.push_back({"MUL", result, right, temp});
                result = temp; // Update result to the temporary variable
            }
        }
        else if (isalpha(currentChar())) // Handle variables
        {
            result = "";
            // Only capture a single character for variable names
            result += currentChar();
            advance();

            // Check for implicit multiplication (e.g., "x(y)" or "xy")
            skipWhitespace();
            if (currentChar() == '(' || isalpha(currentChar()))
            {
                std::string temp = getTempVar();
                std::string right = parseFactor(); // Parse the next factor
                instructions.push_back({"MUL", result, right, temp});
                result = temp; // Update result to the temporary variable
            }
        }
        else if (currentChar() == '-') // Handle negative numbers and expressions
        {
            advance(); // Skip '-'
            std::string operand = parseFactor();
            
            std::string temp = getTempVar();
            instructions.push_back({"MUL", "-1", operand, temp});
            result = temp;
        }
        else
        {
            throw std::runtime_error(std::string("Error: Invalid character '") + currentChar() + "' in input");
        }

        return result;
    }

    // Add support for the power operator (^)
    std::string parsePower()
    {
        std::string base = parseFactor(); // Parse the base
        skipWhitespace();
        if (currentChar() == '^')
        {
            advance();                         // Skip '^'
            std::string exponent = parseFactor(); // Parse the exponent

            std::string temp = getTempVar(); // Generate a temporary variable
            instructions.push_back({"POW", base, exponent, temp});
            return temp; // Return the result of the power operation
        }
        return base; // If no '^', return the base
    }

    // Parses multiplication and division
    std::string parseTerm()
    {
        std::string left = parsePower(); // Call parsePower instead of parseFactor

        while (currentChar() == '*' || currentChar() == '/')
        {
            char op = currentChar();
            advance();                         // Move to the next character
            std::string right = parsePower();  // Call parsePower instead of parseFactor

            if (op == '/' && right == "0")
            {
                throw std::runtime_error("Error: Division by zero");
            }

            std::string temp = getTempVar(); // Generate a temporary variable
            instructions.push_back({op == '*' ? "MUL" : "DIV", left, right, temp});
            left = temp; // Update left to the result of this operation
        }

        return left; // Return the final result variable
    }

    // Parses addition and subtraction
    std::string parseExpression()
    {
        std::string left = parseTerm(); // Parse the left term

        while (currentChar() == '+' || currentChar() == '-')
        {
            char op = currentChar();
            advance();                       // Move to the next character
            std::string right = parseTerm(); // Parse the right term

            std::string temp = getTempVar(); // Generate a temporary variable
            instructions.push_back({op == '+' ? "ADD" : "SUB", left, right, temp});
            left = temp; // Update left to the result of this operation
        }

        return left; // Return the final result variable
    }

public:
    SimpleParser(const std::string &expr) : input(expr.c_str()), pos(0), tempVarCount(0) {}

    // Parses the full expression and generates instructions
    void parse()
    {
        parseExpression();
        skipWhitespace(); // Skip trailing whitespace
        if (currentChar() != '\0')
        {
            throw std::runtime_error("Error: Unexpected characters at the end of input");
        }
    }

    // Prints the generated instructions
    void printInstructions() const
    {
        std::cout << "Generated Instructions:\n";
        for (const auto &instr : instructions)
        {
            std::cout << instr.operation << " " << instr.operand1;
            if (!instr.operand2.empty())
            {
                std::cout << " " << instr.operand2;
            }
            std::cout << " -> " << instr.result << "\n";
        }
    }

    // Returns the generated instructions
    const std::vector<Instruction> &getInstructions() const
    {
        return instructions;
    }
};

// Update the computeInstruction function to handle numeric literals
double getOperandValue(const std::string &operand, const std::unordered_map<std::string, double> &variables) 
{
    // Check if the operand is a number
    if (isdigit(operand[0]) || (operand[0] == '-' && operand.length() > 1 && isdigit(operand[1]))) {
        return std::stod(operand);
    }
    
    // Otherwise, look it up in the variables map
    return variables.at(operand);
}

double computeInstruction(const Instruction &instr, const std::unordered_map<std::string, double> &variables) 
{
    double operand1 = getOperandValue(instr.operand1, variables);
    double operand2 = instr.operand2.empty() ? 0 : getOperandValue(instr.operand2, variables);

    if (instr.operation == "ADD")
        return operand1 + operand2;
    else if (instr.operation == "SUB")
        return operand1 - operand2;
    else if (instr.operation == "MUL")
        return operand1 * operand2;
    else if (instr.operation == "DIV")
        return operand1 / operand2;
    else if (instr.operation == "POW")
        return std::pow(operand1, operand2);

    throw std::runtime_error("Error: Unknown operation " + instr.operation);
}

int main()
{
    try
    {
        int choice;
        std::cout << "Enter 1 for arithmetic expression or 2 for polynomial: ";
        std::cin >> choice;
        std::cin.ignore(); // Ignore the newline character after the choice input

        if (choice == 1) // Arithmetic
        {
            std::string input;
            std::cout << "Enter an arithmetic equation: ";
            std::getline(std::cin, input);

            SimpleParser parser(input);
            parser.parse();
            parser.printInstructions();
        }
        else if (choice == 2) // Polynomial
        {
            std::string polynomial;
            std::cout << "Enter a polynomial equation: ";
            std::getline(std::cin, polynomial);

            SimpleParser parser(polynomial);
            parser.parse();

            // Ask for values of user-defined variables only (e.g., x, y)
            std::unordered_map<std::string, double> variables;
            for (const auto &instr : parser.getInstructions())
            {
                // Only ask for single-letter variable values (like x, y, z)
                if (isalpha(instr.operand1[0]) && instr.operand1.length() == 1 && 
                    variables.find(instr.operand1) == variables.end())
                {
                    std::cout << "Enter value for " << instr.operand1 << ": ";
                    std::cin >> variables[instr.operand1];
                }
                
                if (!instr.operand2.empty() && isalpha(instr.operand2[0]) && 
                    instr.operand2.length() == 1 && variables.find(instr.operand2) == variables.end())
                {
                    std::cout << "Enter value for " << instr.operand2 << ": ";
                    std::cin >> variables[instr.operand2];
                }
            }

            // Compute the result step by step
            std::cout << "Generated Instructions and Results:\n";
            for (const auto &instr : parser.getInstructions())
            {
                double result = computeInstruction(instr, variables);
                variables[instr.result] = result; // Store the result in the variable map
                
                std::cout << instr.operation << " ";
                
                // Display operand1 (showing its value if it's a variable)
                if (isalpha(instr.operand1[0]) && instr.operand1.length() == 1) {
                    std::cout << instr.operand1 << "(" << getOperandValue(instr.operand1, variables) << ")";
                } else {
                    std::cout << instr.operand1;
                }
                
                if (!instr.operand2.empty()) {
                    std::cout << " ";
                    // Display operand2 (showing its value if it's a variable)
                    if (isalpha(instr.operand2[0]) && instr.operand2.length() == 1) {
                        std::cout << instr.operand2 << "(" << getOperandValue(instr.operand2, variables) << ")";
                    } else {
                        std::cout << instr.operand2;
                    }
                }
                
                std::cout << " -> " << instr.result << " = " << result << "\n";
            }

            // Output the final result
            std::string finalResultVar = parser.getInstructions().back().result;
            std::cout << "Final result: " << variables[finalResultVar] << "\n";
        }
        else
        {
            std::cout << "Invalid choice. Please enter 1 or 2.\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
