/**
 * @file OBJECTutilities.h
 * @date January 2018
 * @brief File containing the function definitions of helper functions 
 * for the members of the GEDCOM objects and the GEDCOM list types
 * @Name: Alex Lai
 * @Student Number: 0920158
 */

#ifndef OBJECTUTILITIES_H
#define OBJECTUTILITIES_H

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

/** Function to create a GEDCOMerror object and initialize it
 *@pre no pre required
 *@post A initialized GEDCOMerror object with default values line: -1 and type:OK
 *@return GEDCOMerror - a initialized GEDCOMerror object
 **/
GEDCOMerror initializeError();

/** Function to recursivly get the children of a individual object
 *@pre a list of families that is not empty and a valid pointer to an individual object
 *@post A list of descendants is returned to the calling function
 *@return List - a list of individual objects that are descendants of a person
 **/
List getChildren(List families, const Individual* person);

/** Function to compare individuals in more depth
 *@pre 2 void pointers to individual objects
 *@post A true or false indicating if the objects are the same
 *@return bool - true if individuals are the same, false otherwise
 **/
bool customCompareIndividuals(const void* first, const void* second);

/** Function to compare individual names
 *@pre 2 void pointers to individual objects
 *@post A true or false indicating if the objects are the same
 *@return bool - true if individuals are the same, false otherwise
 **/
bool customCompareName(const void *first, const void *second);

/** Function to check if an individual has been inserted in a generation
 *@pre a List of generations and a individual object
 *@post A true or false indicating if the object is in the list
 *@return bool - true if individual has been inserted, false otherwise
 **/
bool alreadyInserted(const List genList, const Individual* toInsert);

/** Function to validate the contents of a fields list
 *@pre a List of field objects that has been initialized
 *@post A field list has been validated against the gedcom standard requirements
 *@return ErrorCode - an error code representing the validity of the field list
 **/
ErrorCode validateFieldList(const List fieldList);

/** Function to validate the contents of an events list
 *@pre a List of event objects that has been initialized
 *@post A event list has been validated against the gedcom standard requirements
 *@return ErrorCode - an error code representing the validity of the event list
 **/
ErrorCode validateEventList(const List eventList);

//****************************************** Copy object functions *******************************************

/** Function to create a copy of a field list for a given object
 *@pre a source list containing field objects which is not null
 *@post A complete copy of the field list 
 *@return void - no return value
 *@param source - a list containing field objects
 *@param destination - a list pointer to where the field list is to be copied to
 **/
List copyFieldList(List source);

/** Function to create a copy of a event list for a given object
 *@pre a source list containing event objects which is not null
 *@post A complete copy of the event list 
 *@return void - no return value
 *@param source - a list containing event objects
 *@param destination - a list pointer to where the event list is to be copied to
 **/
List copyEventList(List source);

/** Function to create a copy of a individual object
 *@pre a source individual containing a individual object which is not null
 *@post A complete copy of then individual object
 *@return void - no return value
 *@param source - a pointer to a individual object
 *@param destination - a pointer to where a complete copy of the individual is
 **/
void copyIndividual(Individual** destination, Individual* sourcePerson);
//************************************************************************************************************

//****************************************** Print and delete object functions *******************************************

/** Function to get a humanly readable string of a Individuals full name
 *@pre a void pointer to a Individual object which is not null
 *@post char pointer to a human readable string of the Individuals name
 *@return char pointer - a char pointer to the address of the string
 *@param toBePrinted - a void pointer to a Individual object
 **/
char* printIndividualName(void* toBePrinted);

/** Function to delete a weak refrence to a data type to avoid double free errors
 *@pre a void pointer a object 
 *@post The memory is not destroyed but the pointer to that data is
 *@return void - no return value
 *@param toBeDeleted - a void pointer to an object and submitter objects
 **/
void deleteWeakReference(void* toBeDeleted);

/** Function to release all the memory allocated to the header object
 *@pre a void pointer to a header object which cannot be null
 *@post The memory allocated will be freed back to the system
 *@return void - no return value
 *@param toBeDeleted - a void pointer to a header object
 **/
void deleteHeader(void* toBeDeleted);

/** Function to release all the memory allocated to the submitter object
 *@pre a void pointer to a header object which cannot be null
 *@post The memory allocated will be freed back to the system
 *@return void - no return value
 *@param toBeDeleted - a void pointer to a header object
 **/
void deleteSubmitter(void* toBeDeleted);

/** Function to return a pointer to a string containing a human readable representation
 * of the header object
 *@pre a void pointer to a header object which cannot be null
 *@post Pointer to a human readable string of the header information
 *@return char pointer - a pointer to the location of the string
 *@param toBePrinted - a void pointer to a header object
 **/
char* printHeader(void* toBePrinted);

/** Function to return a pointer to a string containing a human readable representation
 * of the submitter object
 *@pre a void pointer to a submitter object which cannot be null
 *@post Pointer to a human readable string of the submitter information
 *@return char pointer - a pointer to the location of the string
 *@param toBePrinted - a void pointer to a submitter object
 **/
char* printSubmitter(void* toBePrinted);
//************************************************************************************************************

/** Function to convert an individual object to a json string with sex field and num families attributes
 *@pre a Individual pointer to a valid individual object
 *@post Json string allocated representing the persons information
 *@return char* - a char pointer to the location of the JSON string
 *@param Individual* - a individual pointer to a individual object
 **/
char* personToJSON(const Individual* ind);

/** Function that converts an GEDCOM object to a json object string
 *@pre a GEDCOMobject pointer to a valid GEDCOM object
 *@post Json string allocated representing the GEDCOM information
 *@return char* - a char pointer to the location of the JSON string
 *@param GEDCOMobject* - a GEDCOMobject pointer to a GEDCOM object
 **/
char* GEDCOMtoJSON(char* fileName, GEDCOMobject* gedcom);

/** Function that saves an empty gedcom to a file
 *@pre a valid file name and a json string containing GEDCOM information
 *@post A GEDCOM file is created and saved to a file
 *@return void - no return. if invalid file or gedcom, does nothing
 *@param char* - a pointer to a file name of the gedcom
 *@param char* - a pointer to a json string of a GEDCOM object
 **/
void saveGEDCOM(char* fileName, char* jsonGEDCOM);

/** Function that saves an individual to a GEDCOM file
 *@pre a valid file name and a json string containing a persons info
 *@post A individual record is added to the GEDCOM file
 *@return bool - true on success, false on failure
 *@param char* - a pointer to a file name of the gedcom
 *@param char* - a pointer to a json string of a Ind object
 **/
bool saveIndividual(char* fileName, char* jsonInd);

/** Function that generates a json representation of a Generation list
 *@pre a valid file name and a json string of an individual in the file
 *@post A json list of generations of an individual are returned
 *@return char* - a char pointer to the location of the JSON string
 *@param char* - a pointer to a file name of the gedcom
 *@param char* - a pointer to a json string of a Ind object
 *@param bool - a boolean value; true -> getDescendants, false -> getAncestors
 *@param int - a number of generations to return
 **/
char* getGenerations(char* fileName, char* jsonInd, bool getDesc, int numGen);

/** Function that generates a json representation of a GEDCOM file
 *@pre a valid file name pointing to a file on the server
 *@post A json object of a file is returned
 *@return char* - a char pointer to the location of the file JSON string
 *@param char* - a pointer to a file name of the gedcom
 **/
char* getFile(char* filesName);

/** Function that generates a json representation of all individuals in a file
 *@pre a valid file name pointing to a file on the server
 *@post A json list object of individuals is returned
 *@return char* - a char pointer to the location of the object list JSON string
 *@param char* - a pointer to a file name of the gedcom
 **/
char* getIndividuals(char* fileName);

#endif