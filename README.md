# Compile Design Assignment 1
## Design a custom instruction for a given equation in a compiler.

This project implements a simple compiler and interpreter for mathematical expressions. It parses expressions into instructions and executes them to produce results.

## Features

- Parse and evaluate arithmetic expressions
- Evaluate polynomial expressions with variable substitution
- Support for basic operations: addition, subtraction, multiplication, division
- Support for power operations (^)
- Implicit multiplication (e.g., "2x" or "x(y+1)")
- Custom operation `PRINCE` to calculate the average of two operands
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

- **Arithmetic Expression Mode**: Parses and displays the instructions for an expression.
- **Polynomial Evaluation Mode**: Evaluates a polynomial with user-provided values for variables.

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

## Example 3: Expression with Custom Instruction

```bash
Enter 1 for arithmetic expression or 2 for polynomial: 2
Enter a polynomial equation: 3*x^2 + $(y, z) * 4
Enter value for x: 2
Enter value for y: 10
Enter value for z: 6
Generated Instructions and Results:
POW x(2) 2 -> temp0 = 4
MUL 3 temp0 -> temp1 = 12
PRINCE y(10) z(6) -> temp2 = 8
MUL temp2 4 -> temp3 = 32
ADD temp1 temp3 -> temp4 = 44
Final result: 44
```

## How It Works

- The `SimpleParser` class parses expressions into a series of instructions.
- It follows a **recursive descent parsing** approach with proper operator precedence.
- Temporary variables (like `temp0`, `temp1`) are used to store intermediate results.
- For polynomial evaluation, the program substitutes values for variables and executes the instructions.

## Implementation Details

### Parsing Hierarchy

The parsing process follows a hierarchy to handle operator precedence:

1. **Expression**: Handles addition (`+`) and subtraction (`-`).
2. **Term**: Handles multiplication (`*`) and division (`/`).
3. **Power**: Handles exponentiation (`^`).
4. **Factor**: Handles numbers, variables, or expressions in parentheses.

### Instruction Types

The program generates the following types of instructions:

- **ADD**: Addition operation
- **SUB**: Subtraction operation
- **MUL**: Multiplication operation
- **DIV**: Division operation
- **POW**: Power/Exponentiation operation
- **PRINCE**: Custom operation to calculate the average of two operands

### Custom Operation: PRINCE

The `PRINCE` operation is a custom instruction that calculates the average of two operands. For example:

```bash
$(y, z)
```

This generates the instruction:

```
PRINCE y z -> temp
```

Where `temp` stores the average of `y` and `z`.

## Error Handling

The program includes error handling for:

- Invalid input expressions
- Division by zero
- Missing parentheses
- Invalid characters

## Class Overview: `SimpleParser`

The `SimpleParser` class is the core of the project. It performs the following tasks:

1. **Parsing**: Breaks down the input expression into smaller components (numbers, variables, operators).
2. **Instruction Generation**: Converts the parsed components into a sequence of instructions.
3. **Error Checking**: Ensures the input expression is valid and handles errors like missing parentheses or invalid characters.

### Key Methods

- **`parse()`**: Parses the full expression and generates instructions.
- **`printInstructions()`**: Prints the generated instructions in a human-readable format.
- **`getInstructions()`**: Returns the list of generated instructions for further processing.

### Example Workflow

For the input `2x + 3^2`, the `SimpleParser` class generates the following instructions:

```
MUL 2 x -> temp0
POW 3 2 -> temp1
ADD temp0 temp1 -> temp2
```

These instructions can then be executed step-by-step to evaluate the expression.

```bash
Final result: temp2
```
