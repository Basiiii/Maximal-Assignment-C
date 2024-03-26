/**
 *
 *  @file      greedy.c
 *  @brief     Implementation of the "Greedy" algorithm.
 *  @details   This file contains the implementation of functions related to
 *             the "Greedy" algorithm.
 *  @author    Enrique Rodrigues
 *  @date      10.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 *
 */
#include "greedy.h"

#include <stdio.h>
#include <stdlib.h>

#include "error_codes.h"
#include "matrix_core.h"

/**
 * @brief Solve the problem with a "Greedy" algorithm.
 * @param matrix               - The matrix.
 * @param maxSum               - Pointer to store the maximum sum.
 * @param maxSelection         - Pointer to store the selected numbers.
 * @param currentSelectionSize - Pointer to store the selected elements.
 * @retval `INVALID_MATRIX_OR_INDICES` - The matrix or the provided indices are
 *                                       invalid.
 * @retval `MEMORY_ALLOCATION_FAILURE` - Memory allocation failure for the new
 *                                       matrix element.
 * @retval `SUCCESS`                    - Operation successful.
 */
int GreedyAlgorithm(Matrix* matrix, int* maxSum, int* maxSelection,
                    int* currentSelectionSize) {
  if (matrix == NULL || matrix->width <= 0) {
    return INVALID_MATRIX_OR_INDICES;
  }

  *maxSum = 0;

  int* usedRows = calloc(matrix->height, sizeof(int));
  int* usedColumns = calloc(matrix->width, sizeof(int));
  if (usedRows == NULL || usedColumns == NULL) {
    free(usedRows);
    free(usedColumns);
    return MEMORY_ALLOCATION_FAILURE;
  }

  MatrixRowNode* currentRow = matrix->head;
  *currentSelectionSize = 0;
  int rowIndex = 0;
  while (currentRow != NULL) {
    MatrixElement* currentElement = currentRow->row;
    int maxElementValue = INT_MIN;
    const MatrixElement* maxElement = NULL;

    // Find largest number that you can get in current row
    while (currentElement != NULL) {
      if (!usedRows[rowIndex] && !usedColumns[currentElement->column] &&
          currentElement->value > maxElementValue) {
        maxElementValue = currentElement->value;
        maxElement = currentElement;
      }
      currentElement = currentElement->nextCol;
    }

    // If an element has been found, add it to the selection
    if (maxElement != NULL) {
      maxSelection[*currentSelectionSize] = maxElement->value;
      (*currentSelectionSize)++;
      *maxSum += maxElement->value;
      usedRows[rowIndex] = 1;               // Mark row as used
      usedColumns[maxElement->column] = 1;  // Mark column as used
    }

    currentRow = currentRow->nextRow;
    rowIndex++;
  }

  free(usedRows);
  free(usedColumns);

  return SUCCESS;
}
