#include <iostream>
#include <cctype>
#include <string>
#include <vector>

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

    // Parses factors (numbers or expressions in parentheses)
    std::string parseFactor()
    {
        skipWhitespace();
        if (currentChar() == '(')
        {
            advance();                              // Skip '('
            std::string result = parseExpression(); // Parse the inner expression
            if (currentChar() == ')')
            {
                advance(); // Skip ')'
            }
            else
            {
                throw std::runtime_error("Error: Missing closing parenthesis");
            }
            return result;
        }
        else if (isdigit(currentChar()) || currentChar() == '.')
        {
            return parseNumber(); // Parse and return a number
        }
        else
        {
            throw std::runtime_error(std::string("Error: Invalid character '") + currentChar() + "' in input");
        }
    }

    // Parses multiplication and division
    std::string parseTerm()
    {
        std::string left = parseFactor(); // Parse the left factor

        while (currentChar() == '*' || currentChar() == '/')
        {
            char op = currentChar();
            advance();                         // Move to the next character
            std::string right = parseFactor(); // Parse the right factor

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
};

int main()
{
    try
    {
        std::string input;
        std::cout << "Enter an equation: ";
        std::getline(std::cin, input);

        SimpleParser parser(input);
        parser.parse();
        parser.printInstructions();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
