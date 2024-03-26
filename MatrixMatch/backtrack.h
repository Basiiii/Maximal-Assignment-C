/**
 *  @file      backtrack.h
 *  @brief     Header file for the "Backtrack" algorithm.
 *  @details   This header file contains function declarations and
 *             data structures related to the "Backtrack" algorithm.
 *  @author    Enrique Rodrigues
 *  @date      12.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 */
#ifndef BACKTRACK_H
#define BACKTRACK_H

#include "matrix_core.h"

 /**
  * @struct SelectedElement
  * @brief Structure to store elements chosen by the backtrack algorithm.
  *        Each element contains the current row and column in the matrix, and
  *        its integer value.
  */
typedef struct SelectedElement {
    int row;    // Row of the element
    int col;    // Column of the element
    int value;  // Value of the element
} SelectedElement;

/**
 * @struct ExploreParams
 * @brief Parameters passed to the recursive function of the "Backtrack" algorithm.
 */
typedef struct ExploreParams {
    Matrix* matrix;                    // The matrix
    int currentRow;                    // Current row
    int currentSum;                    // Current sum
    int* maxSum;                       // Total maximum sum
    int* usedRows;                     // Used rows
    int* usedColumns;                  // Used columns
    int* selectionCount;               // Number of selected elements
    SelectedElement* selectionValues;  // Chosen values
} ExploreParams;

/**
 * @brief "Backtrack" algorithm, a solution for calculating the maximum possible sum
 *        of integers from a matrix of integers with any dimensions, so that none
 *        of the selected integers share the same row or column.
 * @param matrix                        - The matrix.
 * @param maxSum                        - Maximum total sum possible.
 * @param selectionCount                - Number of elements chosen for the result.
 * @param maxSelection                  - Array containing the chosen values.
 * @retval `INVALID_MATRIX_OR_INDICES`  - The matrix or the provided indices are invalid.
 * @retval `MEMORY_ALLOCATION_FAILURE`  - Memory allocation failure for the new matrix element.
 * @retval `SUCCESS`                    - Operation successful.
 */
__declspec(dllexport) int BacktrackAlgorithm(Matrix* matrix, int* maxSum,
                                             int* selectionCount,
                                             SelectedElement** maxSelection);

#endif  // !BACKTRACK_H
