/**
 *  @file      constantes.h
 *  @brief     Definitions of constants and ANSI colors for the program.
 *  @details   This file contains definitions of constants used
 *             in the program, including ANSI codes for text formatting.
 *  @author    Enrique Rodrigues
 *  @date      1.03.2024
 *  @copyright © Enrique Rodrigues, 2024. All right reserved.
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

// ANSI Codes
#define ANSI_RESET "\033[0m"
#define ANSI_BOLD "\033[1m"
#define ANSI_UNDERLINE "\033[4m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"

// Message Strings
#define MSG_CRITICAL ANSI_BOLD ANSI_RED "CRITICO: " ANSI_RESET
#define MSG_ERROR ANSI_BOLD ANSI_RED "ERRO: " ANSI_RESET
#define MSG_WARNING ANSI_BOLD ANSI_YELLOW "AVISO: " ANSI_RESET
#define MSG_INFO ANSI_BOLD ANSI_GREEN "INFO: " ANSI_RESET

// Text File Constants
#define ELEMENT_SEPARATOR ";"
#define MAX_LINE_SIZE 500

// Default Matrix Values
#define DEFAULT_MATRIX_VALUE 0

#endif  // !CONSTANTS_H
