/**
 *  @file      matrix_io.h
 *  @brief     Header file that defines input/output operations for matrices.
 *  @details   This file contains the definitions of functions for manipulating
 *             matrices through input/output. Specifically, it allows populating
 *             a matrix with data from a file, printing a matrix, etc.
 *  @author    Enrique Rodrigues
 *  @date      1.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 */
#ifndef MATRIX_IO_H
#define MATRIX_IO_H

#include "matrix_core.h"

/**
 *  @brief Displays a matrix on the screen.
 *  @param matrix - The matrix to be displayed.
 */
__declspec(dllexport) void PrintMatrix(const Matrix* matrix);

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
__declspec(dllexport) int GetMatrixSizeFromFile(const char* filename,
                                                int* width, int* height);

/**
 *  @brief  Fills a matrix with data from a file.
 *  @param  filename - The name of the file.
 *  @param  matrix   - The matrix to be filled with data from the file.
 *  @retval `MEMORY_ALLOCATION_FAILURE` - Memory allocation failure.
 *  @retval `CANNOT_OPEN_FILE`          - Failure to open the file.
 *  @retval `FILE_READ_ERROR`           - Error reading data from the file.
 *  @retval `SUCCESS`                   - Operation successful.
 */
__declspec(dllexport) int PopulateMatrixFromFile(const char* filename,
                                                 Matrix** matrix);

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
__declspec(dllexport) int CreateMatrixFromFile(const char* filename,
                                               Matrix** matrix);

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
__declspec(dllexport) int InsertRow(Matrix* matrix, const int* newRow,
                                    const int sizeArray);

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
__declspec(dllexport) int InsertColumn(Matrix* matrix, const int* newColumn,
                                       const int sizeArray);

/**
 *  @brief  Deletes a row from the matrix.
 *  @param  matrix   - The matrix.
 *  @param  rowIndex - The row of the matrix to be deleted.
 *  @retval `OUT_OF_BOUNDS` - Position does not exist.
 *  @retval `SUCCESS`       - Operation successful.
 */
__declspec(dllexport) int DeleteRow(Matrix* matrix, int rowIndex);

/**
 *  @brief  Deletes a column from the matrix.
 *  @param  matrix   - The matrix.
 *  @param  colIndex - The column to be deleted.
 *  @retval `OUT_OF_BOUNDS` - Position does not exist.
 *  @retval `SUCCESS`       - Operation successful.
 */
__declspec(dllexport) int DeleteColumn(Matrix* matrix, int colIndex);

#endif  // !MATRIX_IO_H
