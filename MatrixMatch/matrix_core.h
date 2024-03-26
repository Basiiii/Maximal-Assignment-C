/**
 *  @file      matrix_core.h
 *  @brief     Header file containing the definitions of structures and
 *             main functions for matrix operations.
 *  @details   This file contains the main structures and functions
 *             related to matrix manipulation, such as creation and
 *             population of matrices, memory deallocation, etc.
 *  @author    Enrique Rodrigues
 *  @date      1.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 */
#ifndef MATRIX_H
#define MATRIX_H

#include "constants.h"
#include "error_codes.h"

 /**
  * @struct MatrixElement
  * @brief Represents an element of the matrix.
  *
  * Each element contains its integer value, its current column, and a pointer
  * to the next element in the same row.
  */
typedef struct MatrixElement {
  int value;                      // Element value as integer
  int column;                     // Current column
  struct MatrixElement* nextCol;  // Next element in the line (next col)
} MatrixElement;

/**
 * @struct MatrixRowNode
 * @brief Represents a row as a node of the matrix.
 *
 * A row of the matrix containing a pointer to the first element of the
 * row and a pointer to the next row.
 */
typedef struct MatrixRowNode {
  MatrixElement* row;             // Pointer to the first element of the row
  struct MatrixRowNode* nextRow;  // Pointer to the next row node
} MatrixRowNode;

/**
 * @struct Matrix
 * @brief A matrix.
 *
 * The matrix contains the `head`, which is a pointer to the first row of the matrix,
 * and the size of the matrix.
 */
typedef struct Matrix {
  MatrixRowNode* head;  // Pointer to first row of matrix
  int width;            // Matrix width
  int height;           // Matrix height
} Matrix;

/**
 * @brief Swap the value of an element at a given position in the matrix.
 * @param matrix - The matrix containing the element.
 * @param row    - The row of the matrix where the element is.
 * @param col    - The column of the matrix where the element is.
 * @param value  - The new value to place in the element.
 * @retval `OUT_OF_BOUNDS` - Invalid matrix position.
 * @retval `SUCCESS`       - Operation successful.
 */
__declspec(dllexport) int ReplaceValueAtPosition(Matrix* matrix, int row,
                                                 int col, int value);

/**
 * @brief Create a new element with a given value.
 * @param value  - The value to be stored in the `MatrixElement`.
 * @retval       - A pointer to the new element, or NULL in case of
 *                  memory allocation error.
 */
__declspec(dllexport) MatrixElement* CreateMatrixElement(int value, int column);

/**
 * @brief Initialize a matrix row with default values.
 * @param width  - Row size (number of columns).
 * @retval       - Pointer to the last element of the row, or NULL in case
 *                  of memory allocation error.
 */
__declspec(dllexport) MatrixElement* InitializeRow(int width);

/**
 * @brief Initialize a row node of the matrix with a given size,
 *         filling it with default values.
 * @param width  - Row size (number of columns).
 * @retval       - A pointer to the new row node, or NULL in case of
 *                  memory allocation error.
 */
__declspec(dllexport) MatrixRowNode* InitializeRowNode(int width);

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
__declspec(dllexport) int CreateMatrix(int width, int height, Matrix** matrix);

/**
 * @brief Create and add an element to the end of a row in the matrix.
 * @param head   - The first element of the row.
 * @param value  - The value the element will have.
 * @retval       - `NULL` in case of error or the pointer to the beginning of
 *                  the row.
 */
__declspec(dllexport) MatrixElement* AddElementToRow(MatrixElement* head,
                                                     int value, int column);

/**
 * @brief Free allocated memory of a matrix.
 * @param matrix - The matrix to be freed.
 */
__declspec(dllexport) void FreeMatrix(Matrix* matrix);

/**
 * @brief Get the row node at a specific index.
 * @param matrix    - The matrix.
 * @param rowIndex  - The index of the desired row.
 * @retval          - The corresponding row node if the index is valid, NULL
 *                     otherwise.
 */
__declspec(dllexport) MatrixRowNode* GetRowNode(Matrix* matrix, int rowIndex);

/**
 * @brief Get an element in a specific column.
 * @param rowNode   - The row node of the matrix.
 * @param colIndex  - The column index of the matrix.
 * @retval          - The matrix element.
 */
__declspec(dllexport) MatrixElement* GetElementCol(MatrixRowNode* rowNode,
                                                   int colIndex);

#endif  // !MATRIX_H
