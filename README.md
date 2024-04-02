# Maximal-Assignment-C

## Overview

This repository houses a C library designed to solve assignment problems efficiently. Assignment problems involve finding the optimal assignment of tasks to resources with specific constraints. The library includes implementations of well-known algorithms like the Hungarian, Brute Force, and Backtrack algorithms. Developed initially for academic purposes, this library showcases algorithmic proficiency and serves as a useful reference for understanding and implementing assignment problem-solving algorithms.

## Assignment Problem

In the context of this library, an assignment problem refers to the task of maximizing the sum of elements in a matrix by selecting one element from each row and column without repeating any row or column. The algorithms provided aim to find the optimal solution to such problems.

## Usage

The library is provided as read-only and is intended for demonstration purposes. To utilize the algorithms in your own projects, include the necessary header files in your C project and link against the provided object files.

## Algorithms Included

- **Hungarian Algorithm** : This algorithm efficiently solves assignment problems by finding the optimal assignment using a series of augmenting paths.
- **Greedy Algorithm** : While not the most effective, the Greedy algorithm makes the best local choices without reconsidering previous decisions, potentially resulting in a suboptimal solution.
- **Backtrack Algorithm** : This algorithm employs a systematic search strategy to explore the solution space, backtracking when a dead-end is reached and continuing until the optimal assignment is found.

## How to Use

To use this library in your projects, follow these steps:

1. Clone the repository or download the source files.
2. Include the necessary header files (`hungarian.h`, `greedy.h`, `backtrack.h`) in your C project.
3. Link against the provided object files (`hungarian.o`, `brute_force.o`, `backtrack.o`).

## Known Issues

In certain cases, the Hungarian Algorithm may encounter infinite loop errors, leading to difficulties in producing a solution. These errors typically arise when the algorithm struggles to converge on a solution due to complex matrix structures or ambiguous rules governing zero-covering.

## Contributing

This project is read-only and not open for contributions. It was developed as part of academic coursework and is provided as a showcase of work done during university studies.

## License

This project is not licensed for redistribution or modification. It is provided solely as a showcase of academic work and for educational purposes.
