/**
 * @file GEDCOMwriter.h
 * @date Feb 2018
 * @brief function definitions writing a gedcom to a file
 * @Name: Alex Lai
 * @Student Number: 0920158
 */

#ifndef GEDCOMWRITER_H
#define GEDCOMWRITER_H

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "HashMap.h"
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

/** Function for creating a unique XREF key for a record
 *@pre a unique index and type that will be used to create the key 
 *@post An xref key has been created and allocted
 *@return char pointer - A pointer to the address of the allocated key
 *@param index - An integer that will be used to represent the number of the record
 *@param type - A character that will be used to represent the type of record
 **/
char* writeXREF(int index, char type);

/** Function for creating the name portion of an Individual record
 *@pre A pointer to a Individual object that is not NULL 
 *@post A string representation fo the name tags for an individual
 *@return char pointer - A pointer to the address of the allocated string
 *@param person - A pointer to a Individual person object
 **/
char* writeName(const Individual* person);

/** Function for converting a tag value to multi line tag element
 *@pre A pointer to a char* buffer containing the string to be written
 *@post A char string of the buffer that has been changed to add a cont 
 *tag if the buffer contains a new line character. 
 * If no new line character, the original buffer is returned
 *@return char pointer - A pointer to the address of the allocated string
 *@param buffer - A pointer to a character buffer with gedcom data
 **/
char* writeContValue(const char* buffer);

/** Function for creating the field tags and values for a List of field objects
 *@pre A List of field objects that has been initialized
 *@post A string representation of field list in gedcom format
 *@return char pointer - A pointer to the address of the allocated string
 *@param fieldList - A List of field objects
 **/
char* writeFields(const List fieldsList);

/** Function for creating the event tags and values for a List of event objects
 *@pre A List of event objects that has been initialized
 *@post A string representation of event list in gedcom format
 *@return char pointer - A pointer to the address of the allocated string
 *@param eventList - A List of event objects
 **/
char* writeEvents(const List eventList);

/** Function for creating a string representation of a header object
 *@pre A void pointer to a header object that is not NULL
 *@post A string representation of header object in gedcom format or NULL if invalid
 *@return char pointer - A pointer to the address of the allocated string
 *@param headerObj - A valid header object
 **/
char* writeHeader(const void* headerObj);

/** Function for creating a string representation of a submitter object
 *@pre A void pointer to a submitter object that is not NULL
 *@post A string representation of submitter object in gedcom format or NULL if invalid
 *@return char pointer - A pointer to the address of the allocated string
 *@param submitterObj - A valid submitter object
 **/
char* writeSubmitter(const void* submitterObj);

/** Function for creating a string representation of a list of individuals
 *@pre A list of individuals that is not empty and a hashmap that has mapped 
 * the keys of the families and individuals
 *@post A string representation of individuals list in gedcom format or NULL if invalid
 *@return char pointer - A pointer to the address of the allocated string
 *@param personList - A valid list of individuals
 *@param hashMap - A valid HashTable filled with gedcom objects and unique keys
 **/
char* writeIndividuals(const List personList, HashTable* hashMap);

/** Function for creating a string representation of a list of families
 *@pre A list of families that is not empty and a hashmap that has mapped 
 * the keys of the families and individuals
 *@post A string representation of families list in gedcom format or NULL if invalid
 *@return char pointer - A pointer to the address of the allocated string
 *@param familyList - A valid list of families
 *@param hashMap - A valid HashTable filled with gedcom objects and unique keys
 **/
char* writeFamilys(const List familyList, HashTable* hashMap);

#endif