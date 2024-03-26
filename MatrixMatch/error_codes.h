/**
 *  @file      error_codes.h
 *  @brief     Defines error codes used in the program.
 *  @details   This header file contains definitions of error codes that may
 *             occur during program execution.
 *             Each error code is defined with a unique value to facilitate
 *             error identification and handling.
 *  @author    Enrique Rodrigues
 *  @date      2.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 */
#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#define SUCCESS 0  // Operation Success

#define MEMORY_ALLOCATION_FAILURE -1  // Memory allocation failure
#define UNKNOWN_ARGUMENT -2           // Unknown argument
#define NO_FILENAME_PROVIDED -3       // No filename provided
#define INVALID_MATRIX_OR_INDICES -4  // Invalid matrix or matrix indices
#define CANNOT_OPEN_FILE -5           // Unable to open the file
#define FILE_READ_ERROR -6            // File read error
#define UNABLE_REPLACE_VALUE -7       // Unable to replace the value of an element
#define OUT_OF_BOUNDS -8              // Position is out of bounds of the matrix
#define NULL_POINTER -9               // Pointer is NULL

#endif  // !ERROR_CODES_H
