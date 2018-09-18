/**
 * @file GEDCOMutilities.h
 * @date January 2018
 * @brief function definitions for helper functions to parse the GEDCOM objects
 * @Name: Alex Lai
 * @Student Number: 0920158
 */

#ifndef GEDCOMUTILITIES_H
#define GEDCOMUTILITIES_H

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "GEDCOMparser.h"
#include "DATATYPEutilities.h"

/** Function to create a copy of the type for a given character buffer
 *@pre char pointer buffer which cannot be null or empty, the location to store the type
 *@post The type of the line representing the type of the data
 *@return void - no return value
 *@param buffer - a character pointer to an array of characters which hold the type
 *@param type - a double pointer to the location to store the type
 **/
void extractType(const char* buffer, char** type);

/** Function to create a copy of the data for a given character buffer
 *@pre char pointer buffer which cannot be null or empty, the location to store the data
 *@post The data that a tag or type holds is stored
 *@return void - no return value
 *@param buffer - a character pointer to an array of characters which hold the data
 *@param data - a double pointer to the location to store the data
 **/
void extractData(const char* buffer, char** data);

/** Function to create a copy of the data for a given character buffer with secondary data on the next line
 *@pre char pointer buffer which cannot be null or empty, the location to store the data
 *@post The data that a tag or type holds is stored
 *@return void - no return value
 *@param buffer - a character pointer to an array of characters which hold the data
 *@param data - a double pointer to the location to store the data
 **/
void extractSecondLineData(const char* buffer, char** data);

/** Function to create a copy of the data that has more than one line
 *@pre char pointer buffer which cannot be null or empty, the location to store the data
 *@post The data is extracted from the buffer and stored
 *@return void - no return value
 *@param buffer - a character pointer to an array of characters which hold the data
 *@param data - a double pointer to the location to store the data
 **/
void extractMultiLineData(const char* buffer, char**data);

/** Function to get the float value of the corresponding character representation
 *@pre char pointer buffer which cannot be null or empty
 *@post A valid header version in the form of a float type
 * 		If a version passed in has more than one decimal point, the version will be truncated when turned into a float
 *		If a invalid version is sent in, the return value will be 0.0
 *@return the version of the gedcom file
 *@param buffer - a character pointer to an array of characters which hold the version
 **/
float extractFloatValue(const char* buffer);

/** Function to get the corresponding encoding value from its character representation
 *@pre char pointer buffer which cannot be null or empty
 *@post An encoding enum value for the header object of the gedcom file
		If a encoding value is not valid, the default will be set to ASCII
 *@return the encoding type of the file
 *@param buffer - a character pointer to an array of characters which hold the encoding
 **/
CharSet extractEncodingValue(const char* buffer);

/** Function to extract first name of the individual
 *@pre char pointer buffer which cannot be null or empty
 *@post The extracted first name will be stored at the giveName pointer
 *@return void - no data is returned
 *@param buffer - a character pointer to an array of characters which hold the name
 *@param givenName - a double pointer to the location that the given name will be stored at
 **/
void extractFirstName(const char* buffer, char** givenName);

/** Function to extract last name of the individual
 *@pre char pointer buffer which cannot be null or empty
 *@post The extracted last name will be stored at the giveName pointer
 *@return void - no data is returned
 *@param buffer - a character pointer to an array of characters which hold the name
 *@param givenName - a double pointer to the location that the given name will be stored at
 **/
void extractSurname(const char* buffer, char** surname);

/** Function to convert a record with CONT and CONC tags to compressed buffers
 *@pre char pointer buffer which cannot be null or empty
 *@post The extracted last name will be stored at the giveName pointer
 *@return void - no data is returned
 *@param buffer - a character pointer to an array of characters which hold the record data
 *@param givenName - a double pointer to the location that the data will be stored
 **/
void handleContinuedLines(const char* buffer, char** dataResult);

#endif