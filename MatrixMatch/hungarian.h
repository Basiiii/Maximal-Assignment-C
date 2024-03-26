/**
 *  @file      hungarian.h
 *  @brief     Header file for the Hungarian algorithm.
 *  @details   This header file contains function declarations and
 *             data structures related to the Hungarian algorithm.
 *  @author    Enrique Rodrigues
 *  @date      14.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 */
#ifndef HUNGARIAN_ALGORITHM
#define HUNGARIAN_ALGORITHM

#include "matrix_core.h"

/**
 * @struct Zero
 * @brief Structure representing a zero in the matrix.
 */
typedef struct Zero {
  int row;  // Row where the zero is
  int col;  // Column where the zero is
} Zero;

/**
 * @brief Implements the Hungarian algorithm to find the solution to the
 * problem.
 * @param matrix         - Pointer to the input matrix.
 * @param chosenElements - Pointer to a pointer of integers, which will be
 *                         allocated and filled with the chosen elements.
 * @param result         - Pointer to an integer, which will be filled with the
 *                         result of the sum of the chosen elements.
 * @retval `INVALID_MATRIX_OR_INDICES` - The matrix or the provided indices are
 *                                       invalid.
 * @retval `MEMORY_ALLOCATION_FAILURE` - Memory allocation failure for the new
 *                                       matrix element.
 * @retval `SUCCESS`                    - Operation successful.
 */
__declspec(dllexport) int HungarianAlgorithm(Matrix* matrix,
                                             int** chosenElements, int* result);

#endif  // !HUNGARIAN_ALGORITHM
