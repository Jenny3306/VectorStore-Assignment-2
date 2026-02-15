# VectorStore-Assignment-2

C++ implementation of a vector storage system for Assignment 2 of the DSA (Data Structures & Algorithms) course at HCMUT.


## 📋 Overview

This project implements a custom vector storage system in C++, demonstrating fundamental data structures and algorithms concepts. The implementation includes dynamic memory management, vector operations, and utility functions for efficient data storage and manipulation.

## 📚 Key Features

- **Custom Vector Implementation** - A dynamic array-based container with automatic memory management
- **Vector Operations** - Insert, delete, search, and traverse operations
- **Utility Functions** - Helper functions for common operations
- **Unit Testing** - Comprehensive test suite using the Doctest framework
- **Optimized Performance** - Efficient algorithms for data manipulation

## 📁 Project Structure

```
VectorStore-Assignment-2/
├── README.md                           # This file
├── main.cpp                            # Test runner (uses Doctest framework)
├── _251_CO2003___DSA__Assignment_2.pdf # Assignment specification
├── src/
│   ├── VectorStore.h                   # VectorStore class definition
│   ├── VectorStore.cpp                 # VectorStore class implementation
│   ├── main.h                          # Main header file
│   ├── utils.h                         # Utility functions
│   └── newfile.txt                     # Additional notes
└── tests/                              # Test cases directory
```

## 🛠️ Requirements

- **C++11** or later
- **GCC/Clang** compiler or Visual Studio
- **Make** or CMake (optional, for building)

## 📦 Compilation

### Using g++:

```bash
# Compile the project
g++ -std=c++11 -o main main.cpp -I.

# Run the executable
./main
```

### On Windows (using g++):

```bash
g++ -std=c++11 -o main.exe main.cpp -I.
main.exe
```

## Usage

The project uses the **Doctest** framework for unit testing. Tests are defined in `main.cpp` and validate the functionality of the VectorStore implementation.

To run tests:

```bash
./main
```

The program will execute all test cases and display results showing which tests passed or failed.

### Example (pseudo-code):

```cpp
// Tests are written using the Doctest framework
TEST_CASE("VectorStore operations") {
    // Test cases for insert, delete, search, etc.
}
```

## 📝 Assignment Details

This is Assignment 2 for the DSA course (CO2003) at Ho Chi Minh City University of Technology (HCMUT), conducted in the 2025 Fall semester (251).

### Key Concepts Covered:

- **Dynamic Memory Allocation** - Manual memory management in C++
- **Vector Data Structure** - Implementing resizable arrays
- **Algorithm Efficiency** - Time and space complexity optimization
- **Unit Testing** - Writing and running test cases

For detailed assignment specifications, refer to: `_251_CO2003___DSA__Assignment_2.pdf`

## ✅ Notes

- The compiled executable file (`main`) is included in the repository
- Header files are located in the `src/` directory for easy integration
- Utility functions in `utils.h` provide support functions for vector operations
