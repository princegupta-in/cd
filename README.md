# Compile Design Assignment 1
## Design a custom instruction for a given equation in compiler.

This project implements a simple compiler and interpreter for mathematical expressions. It parses expressions into instructions and executes them to produce results.

## Features

- Parse and evaluate arithmetic expressions
- Evaluate polynomial expressions with variable substitution
- Support for basic operations: addition, subtraction, multiplication, division
- Support for power operations (^)
- Implicit multiplication (e.g., "2x" or "x(y+1)")
- Step-by-step execution display

## How to Compile

```bash
g++ -o complier complier.cpp -std=c++11
```
## How to Run
```bash
./complier
```
## Usage
The program offers two modes:

- Arithmetic Expression Mode: Parses and displays the instructions for an expression
- Polynomial Evaluation Mode: Evaluates a polynomial with user-provided values for variables
## Example 1: Arithmetic Expression
```bash
Enter 1 for arithmetic expression or 2 for polynomial: 1
Enter an arithmetic equation: 2*x + 3*y^2
Generated Instructions:
MUL 2 x -> temp0
POW y 2 -> temp1
MUL 3 temp1 -> temp2
ADD temp0 temp2 -> temp3
```
## Example 2: Polynomial Evaluation
```bash
Enter 1 for arithmetic expression or 2 for polynomial: 2
Enter a polynomial equation: 7*x + 5*y
Enter value for x: 8
Enter value for y: 6
Generated Instructions and Results:
MUL 7 x(8) -> temp0 = 56
MUL 5 y(6) -> temp1 = 30
ADD temp0 temp1 -> temp2 = 86
Final result: 86
```
## Example 3: Expression with Power Operator
```bash
Enter 1 for arithmetic expression or 2 for polynomial: 2
Enter a polynomial equation: x^2 + y^2
Enter value for x: 3
Enter value for y: 4
Generated Instructions and Results:
POW x(3) 2 -> temp0 = 9
POW y(4) 2 -> temp1 = 16
ADD temp0 temp1 -> temp2 = 25
Final result: 25
```
## How It Works
-  The SimpleParser class parses expressions into a series of instructions.
-  It follows a recursive descent parsing approach with proper operator precedence.
-  Temporary variables (like temp0, temp1) are used to store intermediate results.
-  For polynomial evaluation, the program substitutes values for variables and executes the instructions.
## Implementation Details
- Parsing Hierarchy: Expression → Term → Power → Factor

    - Expression: Addition and subtraction operations
    - Term: Multiplication and division operations
    - Power: Exponentiation operations
    - Factor: Numbers, variables, or expressions in parentheses
- Instruction Types:

    - ADD: Addition operation
    - SUB: Subtraction operation
    - MUL: Multiplication operation
    - DIV: Division operation
    - POW: Power/Exponentiation operation

## Error Handling
The program includes error handling for:

- Invalid input expressions
- Division by zero
- Missing parentheses
- Invalid characters
