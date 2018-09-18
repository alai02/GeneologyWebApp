/**
 * @file DATATYPEutilities.h
 * @date January 2018
 * @brief function definitions for functions that determine the type of informatoin for a given type
 * @Name: Alex Lai
 * @Student Number: 0920158
 */

#ifndef DATATYPEUTILITIES_H
#define DATATYPEUTILITIES_H

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/** Function to get the type multi line data in a buffer
 *@pre a buffer containing record information
 *@post An integer that corresponds to a multi line type type
 *@return int - number from -1 to 2
 *@param buffer - a pointer to an character array containing the data
 **/
int getMultiLineDataType(const char* buffer);

/** Function to get the type of event in a gedcom object
 *@pre a buffer containing event information
 *@post An integer that corresponds to a event type
 *@return int - number from -1 to 3
 *@param buffer - a pointer to an character array containing the event info
 **/
int getEventDataType(const char* buffer);

/** Function to get the header data type in a character string
 *@pre a buffer containing header type information
 *@post An integer that corresponds to a header data type
 *@return int - number from -1 to 5
 *@param buffer - a pointer to a character array containing the header data info
 **/
int getHeaderDataType(const char* buffer);

/** Function to get the submitter data type in a character string
 *@pre a buffer containing submitter type information
 *@post An integer that corresponds to a submitter data type
 *@return int - number from -1 to 3
 *@param buffer - a pointer to a character array containing the submitter data info
 **/
int getSubmitterDataType(const char* buffer);

/** Function to get the individual data type in a character string
 *@pre a buffer containing individual type information
 *@post An integer that corresponds to a individual data type
 *@return int - number from -1 to 5
 *@param buffer - a pointer to a character array containing the individual data info
 **/
int getIndiDataType(const char* buffer);

/** Function to get the family data type in a character string
 *@pre a buffer containing family type information
 *@post An integer that corresponds to a family data type
 *@return int - number from -1 to 5
 *@param buffer - a pointer to a character array containing the family data info
 **/
int getFamDataType(const char* buffer);

/** Function to get the type of record in a gedcom file
 *@pre a buffer containing record information
 *@post An integer that corresponds to a record type
 *@return int - number from -1 to 5
 *@param buffer - a pointer to an character array containing the record info
 **/
int getRecordType(const char* buffer);

#endif