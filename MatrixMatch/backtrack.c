/**
 *  @file      backtrack.c
 *  @brief     Implementation of the "Backtrack" algorithm.
 *  @details   This file contains the implementation of the "Backtrack"
 * algorithm, a solution for calculating the maximum possible sum of integers
 *             from a matrix of integers with any dimensions, so that none of
 *             the selected integers share the same row or column.
 *  @author    Enrique Rodrigues
 *  @date      11.03.2024
 *  @copyright © Enrique Rodrigues, 2024.
 *             All right reserved.
 */
#include "backtrack.h"

#include <stdio.h>
#include <stdlib.h>

#include "error_codes.h"
#include "matrix_core.h"

/**
 * @brief Copy the selected elements to the array.
 * @param params - Parameters of type `ExploreParams`.
 */
static void CopySelectedValues(ExploreParams* params) {
  *(params->selectionCount) = 0;

  for (int i = 0; i < params->matrix->height; i++) {
    if (params->usedColumns[i]) {
      int colIndex = params->usedColumns[i] - 1;

      MatrixRowNode* currentRowNode = GetRowNode(params->matrix, i);

      const MatrixElement* currentElement =
          GetElementCol(currentRowNode, colIndex);

      // Copy values to array
      params->selectionValues[*(params->selectionCount)].row = i;
      params->selectionValues[*(params->selectionCount)].col = colIndex;
      params->selectionValues[*(params->selectionCount)].value =
          currentElement->value;

      (*(params->selectionCount))++;
    }
  }
}

/**
 * @brief Recursively explore the possible combinations.
 * @param params - Parameters of type `ExploreParams`.
 */
static void Explore(ExploreParams* params) {
  if (params->currentRow == params->matrix->height) {
    if (params->currentSum > *(params->maxSum)) {
      *(params->maxSum) = params->currentSum;
      CopySelectedValues(params);  // Copy selected values to array
    }
    return;
  }

  for (int i = 0; i < params->matrix->width; i++) {
    if (!params->usedRows[params->currentRow] && !params->usedColumns[i]) {
      // Mark as used
      params->usedRows[params->currentRow] = 1;
      params->usedColumns[i] = params->currentRow + 1;

      MatrixRowNode* currentRowNode =
          GetRowNode(params->matrix, params->currentRow);
      const MatrixElement* currentElement = GetElementCol(currentRowNode, i);
      ExploreParams nextParams = {
          .matrix = params->matrix,
          .currentRow = params->currentRow + 1,
          .currentSum = params->currentSum + currentElement->value,
          .maxSum = params->maxSum,
          .selectionValues = params->selectionValues,
          .usedRows = params->usedRows,
          .usedColumns = params->usedColumns,
          .selectionCount = params->selectionCount};
      Explore(&nextParams);

      // Backtrack: Mark element as not used
      params->usedRows[params->currentRow] = 0;
      params->usedColumns[i] = 0;
    }
  }
}

/**
 * @brief "Backtrack" algorithm, a solution for calculating the maximum possible
 * sum of integers from a matrix of integers with any dimensions, so that none
 *        of the selected integers share the same row or column.
 * @param matrix                        - The matrix.
 * @param maxSum                        - Maximum total sum possible.
 * @param selectionCount                - Number of elements chosen for the
 * result.
 * @param maxSelection                  - Array containing the chosen values.
 * @retval `INVALID_MATRIX_OR_INDICES`  - The matrix or the provided indices are
 * invalid.
 * @retval `MEMORY_ALLOCATION_FAILURE`  - Memory allocation failure for the new
 * matrix element.
 * @retval `SUCCESS`                    - Operation successful.
 */
int BacktrackAlgorithm(Matrix* matrix, int* maxSum, int* selectionCount,
                       SelectedElement** selectionValues) {
  // Check if matrix is valid
  if (!matrix || matrix->width <= 0 || matrix->height <= 0) {
    return INVALID_MATRIX_OR_INDICES;
  }

  *maxSum = 0;                                            // Total sum
  int* usedRows = calloc(matrix->height, sizeof(int));    // Rows used
  int* usedColumns = calloc(matrix->width, sizeof(int));  // Columns used
  if (!usedRows || !usedColumns) {
    free(usedRows);
    free(usedColumns);
    return MEMORY_ALLOCATION_FAILURE;
  }

  *selectionCount = 0;  // Number of chosen elements
  int maxPossibleSelections = matrix->height * matrix->width;
  *selectionValues = malloc(maxPossibleSelections * sizeof(SelectedElement));
  if (!*selectionValues) {
    free(usedRows);
    free(usedColumns);
    return MEMORY_ALLOCATION_FAILURE;
  }

  ExploreParams params = {.matrix = matrix,
                          .currentRow = 0,
                          .currentSum = 0,
                          .maxSum = maxSum,
                          .usedRows = usedRows,
                          .usedColumns = usedColumns,
                          .selectionCount = selectionCount,
                          .selectionValues = *selectionValues};
  Explore(&params);  // Recursively iterate over possibilities

  free(usedRows);
  free(usedColumns);

  return SUCCESS;
}
