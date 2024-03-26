/**
 *
 *  @file      matrix_core.c
 *  @brief     Implementations of the main functions for matrix operations.
 *  @details   This file contains the implementations of the main functions
 *             defined in the header file matrix_core.h. It includes functions
 *             for creating matrices, filling matrices with data, freeing
 *             allocated memory for matrices, and other related operations.
 *  @author    Enrique Rodrigues
 *  @date      1.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 *
 */
#define _CRT_SECURE_NO_WARNINGS

#include "matrix_core.h"

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "error_codes.h"

/**
 * @brief Swap the value of an element at a given position in the matrix.
 * @param matrix - The matrix containing the element.
 * @param row    - The row of the matrix where the element is.
 * @param col    - The column of the matrix where the element is.
 * @param value  - The new value to place in the element.
 * @retval `OUT_OF_BOUNDS` - Invalid matrix position.
 * @retval `SUCCESS`       - Operation successful.
 */
int ReplaceValueAtPosition(Matrix* matrix, int row, int col, int value) {
  // Check if position is valid
  if (row < 0 || row >= matrix->height || col < 0 || col >= matrix->width) {
    return OUT_OF_BOUNDS;
  }

  // Go to specific line
  MatrixRowNode* currentRowNode = matrix->head;
  for (int i = 0; i < row; i++) {
    currentRowNode = currentRowNode->nextRow;
  }

  // Go to specific column
  MatrixElement* currentElement = currentRowNode->row;
  for (int j = 0; j < col; j++) {
    currentElement = currentElement->nextCol;
  }

  // Update value at this position
  currentElement->value = value;

  return SUCCESS;
}

/**
 * @brief Create a new element with a given value.
 * @param value  - The value to be stored in the `MatrixElement`.
 * @retval       - A pointer to the new element, or NULL in case of
 *                  memory allocation error.
 */
MatrixElement* CreateMatrixElement(int value, int column) {
  MatrixElement* newElement = (MatrixElement*)malloc(sizeof(MatrixElement));
  if (newElement == NULL) {
    return NULL;  // Memory allocation error
  }
  newElement->value = value;
  newElement->column = column;
  newElement->nextCol = NULL;

  return newElement;
}

/**
 * @brief Initialize a matrix row with default values.
 * @param width  - Row size (number of columns).
 * @retval       - Pointer to the last element of the row, or NULL in case
 *                  of memory allocation error.
 */
MatrixElement* InitializeRow(int width) {
  if (width <= 0) {
    return NULL;
  }

  MatrixElement* head = NULL;
  MatrixElement* currentElement = NULL;

  for (int col = 0; col < width; col++) {
    MatrixElement* newElement = CreateMatrixElement(DEFAULT_MATRIX_VALUE, col);
    if (newElement == NULL) {
      while (head != NULL) {
        MatrixElement* temp = head;
        head = head->nextCol;
        free(temp);
      }
      return NULL;
    }

    if (head == NULL) {
      head = newElement;
    } else {
      currentElement->nextCol = newElement;
    }
    currentElement = newElement;
  }

  return head;
}

/**
 * @brief Initialize a row node of the matrix with a given size,
 *         filling it with default values.
 * @param width  - Row size (number of columns).
 * @retval       - A pointer to the new row node, or NULL in case of
 *                  memory allocation error.
 */
MatrixRowNode* InitializeRowNode(int width) {
  MatrixRowNode* newRowNode = (MatrixRowNode*)malloc(sizeof(MatrixRowNode));
  if (newRowNode == NULL) {
    return NULL;
  }
  newRowNode->nextRow = NULL;

  MatrixElement* rowElements = InitializeRow(width);
  if (rowElements == NULL) {
    free(newRowNode);
    return NULL;
  }
  newRowNode->row = rowElements;

  return newRowNode;
}

/**
 * @brief Create a matrix of a given size filled with default values.
 * @param width  - The number of columns of the matrix.
 * @param height - The number of rows of the matrix.
 * @param matrix - Matrix to hold the matrix data.
 * @retval `INVALID_MATRIX_OR_INDICES` - The matrix or indices provided are
 *                                       invalid.
 * @retval `MEMORY_ALLOCATION_FAILURE` - Memory allocation failure for the
 *                                       new matrix element.
 * @retval `SUCESS`                    - Operation successful.
 */
int CreateMatrix(int width, int height, Matrix** matrix) {
  if (width <= 0 || height <= 0) {
    return INVALID_MATRIX_OR_INDICES;
  }

  *matrix = (Matrix*)malloc(sizeof(Matrix));
  if (*matrix == NULL) {
    return MEMORY_ALLOCATION_FAILURE;
  }
  (*matrix)->width = width;
  (*matrix)->height = height;
  (*matrix)->head = NULL;

  // Create rows of matrix
  MatrixRowNode* currentRowNode = NULL;
  for (int row = 0; row < height; row++) {
    MatrixRowNode* newRowNode = InitializeRowNode(width);
    if (newRowNode == NULL) {
      while (currentRowNode != NULL) {
        MatrixRowNode* temp = currentRowNode;
        currentRowNode = currentRowNode->nextRow;
        free(temp);
      }
      free(*matrix);
      return MEMORY_ALLOCATION_FAILURE;
    }

    if (row == 0) {
      (*matrix)->head = newRowNode;
    } else {
      currentRowNode->nextRow = newRowNode;
    }
    currentRowNode = newRowNode;
  }

  return SUCCESS;
}

/**
 * @brief Create and add an element to the end of a row in the matrix.
 * @param head   - The first element of the row.
 * @param value  - The value the element will have.
 * @retval       - `NULL` in case of error or the pointer to the beginning of
 *                  the row.
 */
MatrixElement* AddElementToRow(MatrixElement* head, int value, int column) {
  MatrixElement* newElement = malloc(sizeof(MatrixElement));
  if (!newElement) {
    return NULL;
  }

  newElement->value = value;
  newElement->column = column;
  newElement->nextCol = NULL;

  // If line is empty we set element as start of line
  if (!head) {
    return newElement;
  }

  // Find last element in line
  MatrixElement* currentElement = head;
  while (currentElement->nextCol) {
    currentElement = currentElement->nextCol;
  }

  // Insert new element
  currentElement->nextCol = newElement;
  return head;
}

/**
 * @brief Free allocated memory of a matrix.
 * @param matrix - The matrix to be freed.
 */
void FreeMatrix(Matrix* matrix) {
  if (!matrix) {
    return;
  }

  MatrixRowNode* currentRowNode = matrix->head;
  MatrixElement* currentElement;

  while (currentRowNode) {
    currentElement = currentRowNode->row;
    while (currentElement) {
      MatrixElement* tempElement = currentElement;
      currentElement = currentElement->nextCol;
      free(tempElement);
    }

    MatrixRowNode* tempRowNode = currentRowNode;
    currentRowNode = currentRowNode->nextRow;
    free(tempRowNode);
  }

  free(matrix);
}

/**
 * @brief Get the row node at a specific index.
 * @param matrix    - The matrix.
 * @param rowIndex  - The index of the desired row.
 * @retval          - The corresponding row node if the index is valid, NULL
 *                     otherwise.
 */
MatrixRowNode* GetRowNode(Matrix* matrix, int rowIndex) {
  // Check if matrix is valid
  if (matrix == NULL || rowIndex < 0 || rowIndex >= matrix->height) {
    return NULL;
  }

  MatrixRowNode* currentRowNode = matrix->head;
  for (int j = 0; j < rowIndex; j++) {
    if (currentRowNode == NULL) {
      return NULL;
    }
    currentRowNode = currentRowNode->nextRow;
  }

  return currentRowNode;
}

/**
 * @brief Get an element in a specific column.
 * @param rowNode   - The row node of the matrix.
 * @param colIndex  - The column index of the matrix.
 * @retval          - The matrix element.
 */
MatrixElement* GetElementCol(MatrixRowNode* rowNode, int colIndex) {
  // Check if row node is valid
  if (rowNode == NULL || colIndex < 0) {
    return NULL;
  }

  MatrixElement* currentElement = rowNode->row;
  for (int j = 0; j < colIndex; j++) {
    if (currentElement == NULL) {
      return NULL;
    }
    currentElement = currentElement->nextCol;
  }

  return currentElement;
}
