#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <algorithm>

class SparseMatrix {
private:
    int rows, cols;
    std::vector<std::tuple<int, int, int>> elements; // (row, col, value)

public:
    SparseMatrix(const char* matrixFilePath) {
        loadFromFile(matrixFilePath);
    }

    SparseMatrix(int numRows, int numCols) : rows(numRows), cols(numCols) {}

    int getElement(int currRow, int currCol) const {
        auto it = std::find_if(elements.begin(), elements.end(),
            [currRow, currCol](const auto& e) {
                return std::get<0>(e) == currRow && std::get<1>(e) == currCol;
            });
        return it != elements.end() ? std::get<2>(*it) : 0;
    }

    void setElement(int currRow, int currCol, int value) {
        auto it = std::find_if(elements.begin(), elements.end(),
            [currRow, currCol](const auto& e) {
                return std::get<0>(e) == currRow && std::get<1>(e) == currCol;
            });

        if (it != elements.end()) {
            if (value != 0) {
                std::get<2>(*it) = value;
            } else {
                elements.erase(it);
            }
        } else if (value != 0) {
            elements.emplace_back(currRow, currCol, value);
        }
    }

    SparseMatrix add(const SparseMatrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions do not match for addition");
        }

        SparseMatrix result(rows, cols);
        for (const auto& e : elements) {
            result.setElement(std::get<0>(e), std::get<1>(e), std::get<2>(e));
        }
        for (const auto& e : other.elements) {
            int currRow = std::get<0>(e), currCol = std::get<1>(e);
            result.setElement(currRow, currCol, result.getElement(currRow, currCol) + std::get<2>(e));
        }
        return result;
    }

    SparseMatrix subtract(const SparseMatrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions do not match for subtraction");
        }

        SparseMatrix result(rows, cols);
        for (const auto& e : elements) {
            result.setElement(std::get<0>(e), std::get<1>(e), std::get<2>(e));
        }
        for (const auto& e : other.elements) {
            int currRow = std::get<0>(e), currCol = std::get<1>(e);
            result.setElement(currRow, currCol, result.getElement(currRow, currCol) - std::get<2>(e));
        }
        return result;
    }

    SparseMatrix multiply(const SparseMatrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions are not compatible for multiplication");
        }

        SparseMatrix result(rows, other.cols);
        for (const auto& e1 : elements) {
            int i = std::get<0>(e1), k = std::get<1>(e1), val1 = std::get<2>(e1);
            for (const auto& e2 : other.elements) {
                if (k == std::get<0>(e2)) {
                    int j = std::get<1>(e2), val2 = std::get<2>(e2);
                    int currVal = result.getElement(i, j);
                    result.setElement(i, j, currVal + val1 * val2);
                }
            }
        }
        return result;
    }

    void saveToFile(const char* filename) const {
        std::ofstream outFile(filename);
        if (!outFile) {
            throw std::runtime_error("Unable to open output file");
        }

        outFile << "rows=" << rows << std::endl;
        outFile << "cols=" << cols << std::endl;

        for (const auto& e : elements) {
            outFile << "(" << std::get<0>(e) << ", " << std::get<1>(e) << ", " << std::get<2>(e) << ")" << std::endl;
        }

        outFile.close();
    }

private:
    void loadFromFile(const char* filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            throw std::runtime_error("Unable to open input file");
        }

        std::string line;
        if (!std::getline(inFile, line) || line.substr(0, 5) != "rows=") {
            throw std::invalid_argument("Input file has wrong format");
        }
        rows = std::stoi(line.substr(5));

        if (!std::getline(inFile, line) || line.substr(0, 5) != "cols=") {
            throw std::invalid_argument("Input file has wrong format");
        }
        cols = std::stoi(line.substr(5));

        while (std::getline(inFile, line)) {
            if (line.empty() || line[0] != '(' || line[line.length() - 1] != ')') {
                continue;  // Skip empty lines or lines with incorrect format
            }

            std::istringstream iss(line.substr(1, line.length() - 2));
            std::string token;
            std::vector<int> entry;

            while (std::getline(iss, token, ',')) {
                try {
                    entry.push_back(std::stoi(token));
                } catch (const std::invalid_argument&) {
                    throw std::invalid_argument("Input file has wrong format");
                }
            }

            if (entry.size() != 3) {
                throw std::invalid_argument("Input file has wrong format");
            }

            setElement(entry[0], entry[1], entry[2]);
        }

        inFile.close();
    }
};

int main() {
    try {
        std::cout << "Select operation:\n1. Addition\n2. Subtraction\n3. Multiplication\n";
        int choice;
        std::cin >> choice;

        std::string file1, file2;
        std::cout << "Enter the first input file path: ";
        std::cin >> file1;
        std::cout << "Enter the second input file path: ";
        std::cin >> file2;

        SparseMatrix matrix1(file1.c_str());
        SparseMatrix matrix2(file2.c_str());

        SparseMatrix result(1, 1);  // Placeholder initialization

        switch (choice) {
            case 1:
                result = matrix1.add(matrix2);
                break;
            case 2:
                result = matrix1.subtract(matrix2);
                break;
            case 3:
                result = matrix1.multiply(matrix2);
                break;
            default:
                throw std::invalid_argument("Invalid operation choice");
        }

        std::string outputFile = "result.txt";
        result.saveToFile(outputFile.c_str());
        std::cout << "Result saved to " << outputFile << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
