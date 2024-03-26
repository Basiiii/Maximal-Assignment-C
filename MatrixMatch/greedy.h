/**
 *  @file      greedy.h
 *  @brief     Header file for the "Greedy" algorithm.
 *  @details   This header file contains function declarations and
 *             data structures related to the "Greedy" algorithm.
 *  @author    Enrique Rodrigues
 *  @date      10.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 */
#ifndef GREEDY_H
#define GREEDY_H

#include "matrix_core.h"

 /**
  * @brief  Solve the problem with a "Greedy" algorithm.
  * @param  matrix               - The matrix.
  * @param  maxSum               - Pointer to store the maximum sum.
  * @param  maxSelection         - Pointer to store the number of selected elements.
  * @param  currentSelectionSize - Pointer to store the selected elements.
  * @retval `INVALID_MATRIX_OR_INDICES` - The matrix or the provided indices are
  *                                       invalid.
  * @retval `MEMORY_ALLOCATION_FAILURE` - Memory allocation failure for the new
  *                                       matrix element.
  * @retval `SUCCESS`                    - Operation successful.
  */
__declspec(dllexport) int GreedyAlgorithm(Matrix* matrix, int* maxSum,
                                          int* maxSelection,
                                          int* currentSelectionSize);

#endif  // !GREEDY_H