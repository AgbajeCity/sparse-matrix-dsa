# Sparse Matrix Operations

This project implements a sparse matrix data structure and operations including addition, subtraction, and multiplication.

## File Structure

- `dsa/sparse_matrix/code/src/sparse_matrix.cpp`: Main source code
- `dsa/sparse_matrix/sample_inputs/`: Directory containing sample input files

## Compilation

To compile the program, use the following command:

g++ -std=c++11 dsa/sparse_matrix/code/src/sparse_matrix.cpp -o sparse_matrix

## Usage

Run the compiled program:
./sparse_matrix

Follow the prompts to select an operation and input file paths.

## Input File Format

Input files should follow this format:
rows=<number_of_rows>
cols=<number_of_columns>
(<row>, <column>, <value>)
(<row>, <column>, <value>)
...

## Output

The program will save the result to a file named `result.txt` in the same format as the input files.
