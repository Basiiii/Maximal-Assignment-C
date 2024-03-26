/**
 *
 *  @file      matrix_io.c
 *  @brief     Input/output operations for matrices.
 *  @details   This file contains functions for input/output manipulation of matrices.
 *             Specifically, it allows populating a matrix with data from a file,
 *             printing a matrix, etc.
 *  @author    Enrique Rodrigues
 *  @date      1.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 *
 */
#define _CRT_SECURE_NO_WARNINGS

#include "matrix_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "error_codes.h"
#include "matrix_core.h"

/**
 *  @brief Displays a matrix on the screen.
 *  @param matrix - The matrix to be displayed.
 */
void PrintMatrix(const Matrix* matrix) {
  MatrixRowNode* currentRow = matrix->head;

  while (currentRow != NULL) {
    MatrixElement* currentElement = currentRow->row;

    while (currentElement != NULL) {
      printf("%d\t", currentElement->value);
      currentElement = currentElement->nextCol;
    }

    printf("\n");
    currentRow = currentRow->nextRow;
  }
}

/**
 *  @brief  Obtains the size of a matrix stored in a file.
 *  @param  filename - The name of the file containing the matrix.
 *  @param  width    - A variable of type `int` that will hold the number of
 *                     columns of the matrix.
 *  @param  height   - A variable of type `int` that will hold the number of
 *                     rows of the matrix.
 *  @retval `CANNOT_OPEN_FILE` - Failure to open the file.
 *  @retval `SUCCESS`          - Operation successful.
 */
int GetMatrixSizeFromFile(const char* filename, int* width, int* height) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return CANNOT_OPEN_FILE;
  }

  *width = 0;
  *height = 0;

  // Count number of columns in the first line
  int c;
  int colCount = 0;
  int rowCount = 0;

  // Read the first line to determine the number of columns
  while ((c = fgetc(file)) != EOF && c != '\n') {
    if (c == ';') {
      colCount++;
    }
  }
  // Add 1 to count the last column that does not have a separator
  *width = colCount + 1;

  // Count the number of lines
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      rowCount++;
    }
  }
  *height = rowCount + 1;

  fclose(file);
  return SUCCESS;
}

/**
 *  @brief  Fills a matrix with data from a file.
 *  @param  filename - The name of the file.
 *  @param  matrix   - The matrix to be filled with data from the file.
 *  @retval `MEMORY_ALLOCATION_FAILURE` - Memory allocation failure.
 *  @retval `CANNOT_OPEN_FILE`          - Failure to open the file.
 *  @retval `FILE_READ_ERROR`           - Error reading data from the file.
 *  @retval `SUCCESS`                   - Operation successful.
 */
int PopulateMatrixFromFile(const char* filename, Matrix** matrix) {
  if (matrix == NULL || *matrix == NULL) {
    return NULL_POINTER;
  }

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return CANNOT_OPEN_FILE;
  }

  int row = 0;        // line of the matrix
  int col = 0;        // column of the matrix
  int value = 0;      // current value
  char* line = NULL;  // line of the text file

  // Store the file's line of text in heap
  line = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
  if (line == NULL) {
    fclose(file);
    return MEMORY_ALLOCATION_FAILURE;
  }

  while (fgets(line, MAX_LINE_SIZE, file)) {
    const char* token = NULL;
    char* nextToken = NULL;

    token = strtok_s(line, ELEMENT_SEPARATOR, &nextToken);

    while (token != NULL) {
      value = atoi(token);
      if (row >= (*matrix)->height || col >= (*matrix)->width) {
        fclose(file);
        free(line);
        return OUT_OF_BOUNDS;
      }

      ReplaceValueAtPosition(*matrix, row, col, value);

      col++;
      if (col == (*matrix)->width) {
        col = 0;
        row++;
      }

      token = strtok_s(NULL, ELEMENT_SEPARATOR, &nextToken);
    }
  }

  fclose(file);
  free(line);
  return SUCCESS;
}

/**
 *  @brief  Creates a matrix from data in a file.
 *  @param  filename - The name of the file.
 *  @param  matrix   - Matrix that will contain the data of the matrix.
 *  @retval `CANNOT_OPEN_FILE` - Failure to open the file.
 *  @retval `INVALID_MATRIX_OR_INDICES` - The matrix or the provided indices are
 *                                       invalid.
 *  @retval `MEMORY_ALLOCATION_FAILURE` - Failure to allocate memory for the
 *                                       new matrix element.
 *  @retval `FILE_READ_ERROR`  - Error reading data from the file.
 *  @retval `SUCCESS`          - Operation successful.
 */
int CreateMatrixFromFile(const char* filename, Matrix** matrix) {
  int width;
  int height;
  if (GetMatrixSizeFromFile(filename, &width, &height) != SUCCESS) {
    return CANNOT_OPEN_FILE;
  }

  int creationResult = CreateMatrix(width, height, matrix);
  if (creationResult != SUCCESS) {
    return creationResult;
  }

  int populationResult = PopulateMatrixFromFile(filename, matrix);
  if (populationResult != SUCCESS) {
    return populationResult;
  }

  return SUCCESS;
}

/**
 *  @brief  Inserts a row into the matrix.
 *  @param  matrix       - The matrix where we will insert a new row.
 *  @param  newRow       - Array with the values of the new row.
 *  @param  sizeArray    - Size of the array.
 *  @retval `OUT_OF_BOUNDS`  - Size of the array is different from the size of
 *                             the matrix.
 *  @retval `MEMORY_ALLOCATION_FAILURE` - Memory allocation failure.
 *  @retval `SUCCESS`                   - Operation successful.
 */
int InsertRow(Matrix* matrix, const int* newRow, const int sizeArray) {
  if (matrix->width != sizeArray) {
    return OUT_OF_BOUNDS;
  }

  MatrixRowNode* newRowNode = malloc(sizeof(MatrixRowNode));
  if (!newRowNode) {
    return MEMORY_ALLOCATION_FAILURE;
  }

  newRowNode->row = NULL;

  // Add a new element to each column of the new line
  for (int col = 0; col < matrix->width; col++) {
    newRowNode->row = AddElementToRow(newRowNode->row, newRow[col], col);
    if (!newRowNode->row) {
      free(newRowNode);
      return MEMORY_ALLOCATION_FAILURE;
    }
  }

  // Insert the new line to beginning of matrix
  newRowNode->nextRow = matrix->head;
  matrix->head = newRowNode;
  matrix->height++;

  return SUCCESS;
}

/**
 *  @brief  Inserts a column at the end of a row in the matrix.
 *  @param  matrix          - The matrix where we will insert the new column.
 *  @param  newColumn       - Array with the values of the new column.
 *  @param  sizeArray       - Size of the array.
 *  @retval `OUT_OF_BOUNDS`  - Size of the array is different from the size of
 *                             the matrix.
 *  @retval `MEMORY_ALLOCATION_FAILURE` - Memory allocation failure.
 *  @retval `SUCCESS`                   - Operation successful.
 */
int InsertColumn(Matrix* matrix, const int* newColumn, const int sizeArray) {
  if (matrix->height != sizeArray) {
    return OUT_OF_BOUNDS;
  }

  // Calculate the new column index
  int newColumnIndex = matrix->width;

  // Iterate over the matrix to insert the new columns
  MatrixRowNode* currentRowNode = matrix->head;
  while (currentRowNode) {
    // Add new element to the end of the line with the value of `newColumn`
    currentRowNode->row =
        AddElementToRow(currentRowNode->row, *newColumn, newColumnIndex);
    if (!currentRowNode->row) {
      return MEMORY_ALLOCATION_FAILURE;
    }

    currentRowNode = currentRowNode->nextRow;
    newColumn++;
  }

  matrix->width++;

  return SUCCESS;
}

/**
 *  @brief  Deletes a row from the matrix.
 *  @param  matrix   - The matrix.
 *  @param  rowIndex - The row of the matrix to be deleted.
 *  @retval `OUT_OF_BOUNDS` - Position does not exist.
 *  @retval `SUCCESS`       - Operation successful.
 */
int DeleteRow(Matrix* matrix, int rowIndex) {
  if (rowIndex < 0 || rowIndex >= matrix->height) {
    return OUT_OF_BOUNDS;
  }

  MatrixRowNode* currentRowNode = matrix->head;
  MatrixRowNode* previousRowNode = NULL;

  for (int i = 0; i < rowIndex; i++) {
    previousRowNode = currentRowNode;
    currentRowNode = currentRowNode->nextRow;
  }

  // If the line to be deleted is the first
  if (previousRowNode == NULL) {
    matrix->head = currentRowNode->nextRow;
  } else {
    previousRowNode->nextRow = currentRowNode->nextRow;
  }

  // Free memory of the line to be deleted
  MatrixElement* currentElement = currentRowNode->row;
  while (currentElement != NULL) {
    MatrixElement* temp = currentElement;
    currentElement = currentElement->nextCol;
    free(temp);
  }
  free(currentRowNode);

  matrix->height--;

  return SUCCESS;
}

/**
 *  @brief  Deletes a column from the matrix.
 *  @param  matrix   - The matrix.
 *  @param  colIndex - The column to be deleted.
 *  @retval `OUT_OF_BOUNDS` - Position does not exist.
 *  @retval `SUCCESS`       - Operation successful.
 */
int DeleteColumn(Matrix* matrix, int colIndex) {
  if (matrix == NULL || colIndex < 0 || colIndex >= matrix->width) {
    return OUT_OF_BOUNDS;
  }

  // Iterate over each line of the matrix
  MatrixRowNode* currentRowNode = matrix->head;
  while (currentRowNode != NULL) {
    if (currentRowNode->row == NULL) {
      currentRowNode = currentRowNode->nextRow;
      continue;
    }

    // Iterate over elements in line
    MatrixElement* currentElement = currentRowNode->row;
    MatrixElement* previousElement = NULL;
    for (int i = 0; i < colIndex; i++) {
      // Check if element exists
      if (currentElement == NULL) {
        break;  // Column was not found, we skip to next line
      }
      previousElement = currentElement;
      currentElement = currentElement->nextCol;
    }

    // If column was not found, we pass to next line
    if (currentElement == NULL) {
      currentRowNode = currentRowNode->nextRow;
      continue;
    }

    if (previousElement == NULL) {
      // Here, `currentElement` is the first to be deleted
      currentRowNode->row = currentElement->nextCol;
    } else {
      previousElement->nextCol = currentElement->nextCol;
    }

    free(currentElement);

    // Skip to next line
    currentRowNode = currentRowNode->nextRow;
  }

  // Decrement the matrix width
  matrix->width--;

  return SUCCESS;
}
