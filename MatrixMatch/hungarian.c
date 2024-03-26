/**
 *
 *  @file      hungarian.c
 *  @brief     Implementation of the Hungarian Algorithm to solve the problem.
 *  @details   This file contains the necessary functions for the implementation
 *             of the Hungarian Algorithm.
 *  @author    Enrique Rodrigues
 *  @date      15.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 *
 */
#include "hungarian.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix_core.h"
#include "matrix_io.h"

/**
 * @brief Gets the index of a row in the matrix, given a specific row node.
 * @param matrix  - Pointer to the matrix representing the problem.
 * @param rowNode - Pointer to the row node whose index should be found.
 * @return The index of the row corresponding to the provided node.
 */
static int GetRowIndex(Matrix* matrix, const MatrixRowNode* rowNode) {
  int index = 0;
  for (MatrixRowNode* temp = matrix->head; temp != rowNode;
       temp = temp->nextRow) {
    index++;
  }
  return index;
}

/**
 * @brief Creates a new structure to represent a zero in the matrix,
 *        with the row and column indices.
 * @param row - Index of the row where the zero is located.
 * @param col - Index of the column where the zero is located.
 * @return A pointer to the newly created `Zero` structure, or terminates the
 *         program in case of memory allocation failure.
 */
static Zero* CreateZero(int row, int col) {
  Zero* zero = (Zero*)malloc(sizeof(Zero));
  if (zero == NULL) {
    exit(MEMORY_ALLOCATION_FAILURE);
  }
  zero->row = row;
  zero->col = col;
  return zero;
}

/**
 * @brief Destroys a zero.
 * @param zero - The zero to be destroyed.
 */
static void DestroyZero(Zero* zero) { free(zero); }

/**
 * @brief Checks if a specific cell in the matrix is covered.
 * @param zero        - Pointer to the structure representing the zero to be checked.
 * @param coveredRows - Pointer to an array of integers indicating which rows are covered (assigned).
 * @param coveredCols - Pointer to an array of integers indicating which columns are covered (assigned).
 * @retval `true` if the cell is covered, `false` otherwise.
 */
static bool IsCovered(static Zero* zero, int* coveredRows, int* coveredCols) {
  return coveredRows[zero->row] || coveredCols[zero->col];
}

/**
 * @brief Marks a specific element in the matrix as covered, updating the arrays of covered rows and columns.
 * @param zero        - Pointer to the structure representing the zero to be covered.
 * @param coveredRows - Pointer to an array of integers indicating which rows are covered (assigned).
 * @param coveredCols - Pointer to an array of integers indicating which columns are covered (assigned).
 */
static void CoverZero(static Zero* zero, int* coveredRows, int* coveredCols) {
  coveredRows[zero->row] = 1;
  coveredCols[zero->col] = 1;
}

/**
 * @brief Counts the number of zeros in the matrix.
 * @param matrix - Pointer to the matrix representing the problem.
 * @retval The number of zeros found in the matrix.
 */
static int CountZeros(Matrix* matrix) {
  int count = 0;
  for (MatrixRowNode* rowNode = matrix->head; rowNode != NULL;
       rowNode = rowNode->nextRow) {
    for (MatrixElement* element = rowNode->row; element != NULL;
         element = element->nextCol) {
      if (element->value == 0) {
        count++;
      }
    }
  }
  return count;
}

/**
 * @brief Inverts the sign of all elements in the matrix, turning positive values into negative and vice versa.
 * @param matrix - Pointer to the matrix representing the problem.
 */
static void NegateAllElements(Matrix* matrix) {
  for (MatrixRowNode* rowNode = matrix->head; rowNode != NULL;
       rowNode = rowNode->nextRow) {
    for (MatrixElement* element = rowNode->row; element != NULL;
         element = element->nextCol) {
      element->value = -element->value;
    }
  }
}

/**
 * @brief Makes all values in the matrix non-negative by subtracting the minimum value of the matrix from all elements.
 * @param matrix - Pointer to the matrix representing the problem.
 */
static void MakeMatrixNonnegative(Matrix* matrix) {
  int minValue = INT_MAX;
  for (MatrixRowNode* rowNode = matrix->head; rowNode != NULL;
       rowNode = rowNode->nextRow) {
    for (MatrixElement* element = rowNode->row; element != NULL;
         element = element->nextCol) {
      minValue = element->value < minValue ? element->value : minValue;
    }
  }
  if (minValue < 0) {
    for (MatrixRowNode* rowNode = matrix->head; rowNode != NULL;
         rowNode = rowNode->nextRow) {
      for (MatrixElement* element = rowNode->row; element != NULL;
           element = element->nextCol) {
        element->value -= minValue;
      }
    }
  }
}

/**
 * @brief Subtracts the minimum value of each row from all elements in the same row, making all values positive.
 * @param matrix - Pointer to the matrix representing the problem.
 */
static void SubtractRowMinima(Matrix* matrix) {
  for (MatrixRowNode* rowNode = matrix->head; rowNode != NULL;
       rowNode = rowNode->nextRow) {
    int minRowValue = INT_MAX;
    for (MatrixElement* element = rowNode->row; element != NULL;
         element = element->nextCol) {
      minRowValue = element->value < minRowValue ? element->value : minRowValue;
    }
    for (MatrixElement* element = rowNode->row; element != NULL;
         element = element->nextCol) {
      element->value -= minRowValue;
    }
  }
}

/**
 * @brief Subtracts the minimum value of each column from all elements in the same column, making all values positive.
 * @param matrix - Pointer to the matrix representing the problem.
 */
static void SubtractColumnMinima(Matrix* matrix) {
  for (int col = 0; col < matrix->width; col++) {
    int minColValue = INT_MAX;
    // Find the minimum value in the current column
    for (MatrixRowNode* rowNode = matrix->head; rowNode != NULL;
         rowNode = rowNode->nextRow) {
      const MatrixElement* element = GetElementCol(rowNode, col);
      if (element != NULL && element->value < minColValue) {
        minColValue = element->value;
      }
    }
    // Subtract the minimum value from all elements in the column
    for (MatrixRowNode* rowNode = matrix->head; rowNode != NULL;
         rowNode = rowNode->nextRow) {
      MatrixElement* element = GetElementCol(rowNode, col);
      if (element != NULL) {
        element->value -= minColValue;
      }
    }
  }
}

/**
 * @brief Checks if the current solution is optimal, i.e., if all rows and columns of the matrix are covered.
 * @param matrix - Pointer to the matrix representing the problem.
 * @retval `true` if the current solution is optimal, `false` otherwise.
 */
static bool IsOptimalSolution(Matrix* matrix) {
  int numRows = matrix->height;
  int numCols = matrix->width;
  int* coveredRows = (int*)calloc(numRows, sizeof(int));
  int* coveredCols = (int*)calloc(numCols, sizeof(int));

  // Mark rows and columns covered by lines
  for (MatrixRowNode* rowNode = matrix->head; rowNode != NULL;
       rowNode = rowNode->nextRow) {
    for (MatrixElement* element = rowNode->row; element != NULL;
         element = element->nextCol) {
      int rowIdx = GetRowIndex(matrix, rowNode);  // Get the row index
      int colIdx = element->column;               // Get the column index
      if (element->value == 0 && !coveredRows[rowIdx] && !coveredCols[colIdx]) {
        coveredRows[rowIdx] = 1;
        coveredCols[colIdx] = 1;
      }
    }
  }

  // Count the number of covered rows and columns
  int numCoveredRows = 0;
  int numCoveredCols = 0;
  for (int i = 0; i < numRows; i++) {
    if (coveredRows[i]) {
      numCoveredRows++;
    }
  }
  for (int j = 0; j < numCols; j++) {
    if (coveredCols[j]) {
      numCoveredCols++;
    }
  }

  // Free memory
  free(coveredRows);
  free(coveredCols);

  // Check if the number of covered rows or columns equals the matrix size
  return numCoveredRows == numRows || numCoveredCols == numCols;
}

/**
 * @brief Prints the coverage status of rows and columns in the matrix.
 * @param coveredRows    - Pointer to an array of booleans indicating which rows are covered (assigned).
 * @param coveredCols    - Pointer to an array of booleans indicating which columns are covered (assigned).
 * @param numRows        - Number of rows in the matrix.
 * @param numCols        - Number of columns in the matrix.
 */
static void PrintCoveredStatus(bool* coveredRows, bool* coveredCols,
                               int numRows, int numCols) {
  printf("Covered Status:\n");
  printf("Rows: ");
  for (int i = 0; i < numRows; i++) {
    printf("%d ", coveredRows[i] ? 1 : 0);
  }
  printf("\nColumns: ");
  for (int j = 0; j < numCols; j++) {
    printf("%d ", coveredCols[j] ? 1 : 0);
  }
  printf("\n");
}

/**
 * @brief Prints the covered matrix, indicating which rows and columns are covered.
 * @param coveredRows    - Pointer to an array of booleans indicating which rows are covered (assigned).
 * @param coveredCols    - Pointer to an array of booleans indicating which columns are covered (assigned).
 * @param numRows        - Number of rows in the matrix.
 * @param numCols        - Number of columns in the matrix.
 */
static void PrintCoveredMatrix(bool* coveredRows, bool* coveredCols,
                               int numRows, int numCols) {
  printf("Covered Matrix:\n");
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      // Check if the row or column is covered
      bool isCovered = coveredRows[row] || coveredCols[col];
      // Print the cell status
      printf("%s ", isCovered ? "X" : "-");
    }
    printf("\n");
  }
}

/**
 * @brief Covers zeros in the matrix, identifying if there are multiple zeros in the same
 *        row or column.
 * @param matrix         - Pointer to the matrix representing the problem.
 * @param coveredRows    - Pointer to an array of booleans indicating which rows are covered (assigned).
 * @param coveredCols    - Pointer to an array of booleans indicating which columns are covered (assigned).
 */
static void CoverZeros(Matrix* matrix, bool** coveredRows, bool** coveredCols) {
  int numRows = matrix->height;
  int numCols = matrix->width;

  if (*coveredRows == NULL || *coveredCols == NULL) {
    exit(MEMORY_ALLOCATION_FAILURE);
  }

  // Iterate through the matrix and find zeros that are not covered
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      if (!(*coveredRows)[row] && !(*coveredCols)[col] &&
          GetElementCol(GetRowNode(matrix, row), col)->value == 0) {
        // Check if there are more zeros in the same column
        bool multipleZerosInColumn = false;
        for (int r = 0; r < numRows; r++) {
          if (r != row &&
              GetElementCol(GetRowNode(matrix, r), col)->value == 0) {
            multipleZerosInColumn = true;
            break;
          }
        }

        // Check if there are more zeros in the same row
        bool multipleZerosInRow = false;
        for (int c = 0; c < numCols; c++) {
          if (c != col &&
              GetElementCol(GetRowNode(matrix, row), c)->value == 0) {
            multipleZerosInRow = true;
            break;
          }
        }

        // If there are more zeros in the same column, cover the column
        if (multipleZerosInColumn) {
          (*coveredCols)[col] = true;
        }
        // If there are more zeros in the same row, cover the row
        else if (multipleZerosInRow) {
          (*coveredRows)[row] = true;
        }
        else {
          // If there is only one zero, do not cover the row or column
        }
      }
    }
  }
}

/**
 * @brief Creates additional zeros in the matrix, adjusting the values of the elements
 *        to ensure the existence of a zero in each row and column.
 * @param matrix         - Pointer to the matrix representing the problem.
 * @param coveredRows    - Pointer to an array of booleans indicating which rows are covered (assigned).
 * @param coveredCols    - Pointer to an array of booleans indicating which columns are covered (assigned).
 */
static void CreateAdditionalZeros(Matrix* matrix, bool* coveredRows,
                                  bool* coveredCols) {
  int numRows = matrix->height;
  int numCols = matrix->width;

  // Find minimum value in uncovered elements
  int minUncovered = INT_MAX;
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      if (!coveredRows[row] && !coveredCols[col] &&
          GetElementCol(GetRowNode(matrix, row), col) != NULL &&
          GetElementCol(GetRowNode(matrix, row), col)->value < minUncovered) {
        minUncovered = GetElementCol(GetRowNode(matrix, row), col)->value;
      }
    }
  }

  // Subtract this minimum value from all uncovered elements
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      if (!coveredRows[row] && !coveredCols[col] &&
          GetElementCol(GetRowNode(matrix, row), col) != NULL) {
        GetElementCol(GetRowNode(matrix, row), col)->value -= minUncovered;
      }
    }
  }

  // Add this value to covered elements twice
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      if (coveredRows[row] && coveredCols[col] &&
          GetElementCol(GetRowNode(matrix, row), col) != NULL) {
        GetElementCol(GetRowNode(matrix, row), col)->value += minUncovered;
      }
    }
  }
}

/**
 * @brief Creates a copy of the provided matrix.
 * @param originalMatrix - Pointer to the original matrix to be copied.
 * @retval Pointer to the new copied matrix, or NULL in case of memory 
           allocation failure.
 */
static Matrix* CopyMatrix(Matrix* originalMatrix) {
  Matrix* newMatrix = (Matrix*)malloc(sizeof(Matrix));
  if (newMatrix == NULL) {
    return NULL;
  }

  newMatrix->width = originalMatrix->width;
  newMatrix->height = originalMatrix->height;
  newMatrix->head = NULL;

  MatrixRowNode* originalRow = originalMatrix->head;
  MatrixRowNode* newRow = NULL;
  MatrixRowNode* lastNewRow = NULL;
  while (originalRow != NULL) {
    newRow = (MatrixRowNode*)malloc(sizeof(MatrixRowNode));
    if (newRow == NULL) {
      return NULL;
    }
    newRow->nextRow = NULL;

    MatrixElement* originalElement = originalRow->row;
    MatrixElement* newElement = NULL;
    MatrixElement* lastNewElement = NULL;
    while (originalElement != NULL) {
      newElement = (MatrixElement*)malloc(sizeof(MatrixElement));
      if (newElement == NULL) {
        return NULL;
      }
      newElement->value = originalElement->value;
      newElement->column = originalElement->column;
      newElement->nextCol = NULL;

      if (lastNewElement == NULL) {
        newRow->row = newElement;
      } else {
        lastNewElement->nextCol = newElement;
      }
      lastNewElement = newElement;

      originalElement = originalElement->nextCol;
    }

    if (lastNewRow == NULL) {
      newMatrix->head = newRow;
    } else {
      lastNewRow->nextRow = newRow;
    }
    lastNewRow = newRow;

    originalRow = originalRow->nextRow;
  }

  return newMatrix;
}

/**
 * @brief Extracts the final solution of the problem, identifying the chosen
 *        elements and calculating the total sum.
 * @param originalMatrix - Pointer to the original matrix.
 * @param finalMatrix    - Pointer to the matrix after applying the algorithm.
 * @param chosenElements - Pointer to a pointer of integers, which will be
 *                         allocated and filled with the chosen elements.
 * @param result         - Pointer to an integer, which will be filled with the
 *                         result of the sum of the chosen elements.
 * @retval `MEMORY_ALLOCATION_FAILURE` - Memory allocation failure for the new
 *                                       matrix element.
 * @retval `SUCCESS`                    - Operation successful.
 */
static int ExtractFinalSolution(Matrix* originalMatrix, Matrix* finalMatrix,
                                int** chosenElements, int* result) {
  int numRows = originalMatrix->height;
  int numCols = originalMatrix->width;
  int sum = 0;
  int assignments = 0;
  bool* coveredCols = (bool*)calloc(numCols, sizeof(bool));
  if (coveredCols == NULL) {
    return MEMORY_ALLOCATION_FAILURE;
  }

  *chosenElements = (int*)malloc(numRows * sizeof(int));
  if (*chosenElements == NULL) {
    free(coveredCols);
    return MEMORY_ALLOCATION_FAILURE;
  }

  // Find the zeros
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      if (GetElementCol(GetRowNode(finalMatrix, row), col)->value == 0 &&
          !coveredCols[col]) {
        int assignmentValue =
            GetElementCol(GetRowNode(originalMatrix, row), col)->value;
        sum += assignmentValue;
        assignments++;
        coveredCols[col] = true;
        (*chosenElements)[assignments - 1] = assignmentValue;
        break;
      }
    }
  }

  *result = sum;

  free(coveredCols);

  return SUCCESS;
}

/**
 * @brief Implements the Hungarian algorithm to find the solution to the problem.
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
int HungarianAlgorithm(Matrix* matrix, int** chosenElements, int* result) {
  Matrix* matrixCopy = CopyMatrix(matrix);
  if (matrixCopy == NULL) {
    return MEMORY_ALLOCATION_FAILURE;
  }

  int numRows = matrixCopy->height;
  int numCols = matrixCopy->width;

  NegateAllElements(matrixCopy);

  MakeMatrixNonnegative(matrixCopy);

  SubtractRowMinima(matrixCopy);

  SubtractColumnMinima(matrixCopy);

  // Cover zeros with minimum amount of lines
  bool* coveredRows = (bool*)calloc(numRows, sizeof(bool));
  if (coveredRows == NULL) {
    return MEMORY_ALLOCATION_FAILURE;
  }
  bool* coveredCols = (bool*)calloc(numCols, sizeof(bool));
  if (coveredCols == NULL) {
    return MEMORY_ALLOCATION_FAILURE;
  }

  while (!IsOptimalSolution(matrixCopy)) {
    // TODO: It chooses the wrong rows/cols to mark sometimes
    CoverZeros(matrixCopy, &coveredRows, &coveredCols);

    CreateAdditionalZeros(matrixCopy, coveredRows, coveredCols);
  }

  int resultExtraction =
      ExtractFinalSolution(matrix, matrixCopy, chosenElements, result);
  if (resultExtraction != SUCCESS) {
    exit(resultExtraction);
  }

  FreeMatrix(matrixCopy);
  free(coveredRows);
  free(coveredCols);

  return SUCCESS;
}
