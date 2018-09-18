/**
 * @file CREATEutilities.h
 * @date January 2018
 * @brief File containing the function definitions for create functions for the members of the GEDCOM object
 * @Name: Alex Lai
 * @Student Number: 0920158
 */

#ifndef CREATEUTILITIES_H
#define CREATEUTILITIES_H

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "DATATYPEutilities.h"
#include "GEDCOMutilities.h"
#include "OBJECTutilities.h"
#include "GEDCOMparser.h"
#include "HashMap.h"
#include "LinkedListAPI.h"

/** Function to create a field object from a given character data buffer
 *@pre a buffer containing all information to create a field object
 *@post A field object is created and it will be added to the given list
 *@return GEDCOMerror - an integer representing if the field was valid and added to the list
 *@param buffer - a pointer to a character array containing the field data
 *@param a pointer to a list where the field should be added to
 **/
int createField(const char* buffer, List* list);

/** Function to create a event object from a given character data buffer
 *@pre a buffer containing all information to create a event object
 *@post A event object is created and it will be added to the given list
 *@return GEDCOMerror - an integer representing if the event was valid and added to the list
 *@param buffer - a pointer to a character array containing the event data
 *@param a pointer to a list where the event should be added to
 **/
int createEvent(const char* buffer, List* list);

/** Function to create a header object from a given character data buffer
 *@pre a buffer containing all information to create a header object
 *@post A header object is created and the GEDCOMobjects header will point to it
 *@return GEDCOMerror - a error structure that contains the error information while creating a header
 *@param buffer - a pointer to a character array containing the header data
 *@param a double pointer to a Header struct that needs to be allocated
 **/
GEDCOMerror createHeader(const char* buffer, Header** header, HashTable* hashMap);

/** Function to create a submitter object from a given character data buffer
 *@pre a buffer containing all information to create a submitter object
 *@post A submitter object is created and the GEDCOMobjects submitter will point to it
 *@return GEDCOMerror - a error structure that contains the error information while creating a submitter
 *@param buffer - a pointer to a character array containing the submitter data
 *@param a double pointer to a submitter struct that needs to be allocated
 **/
GEDCOMerror createSubmitter(const char* buffer, Submitter** submitter, HashTable* hashMap);

/** Function to create a individual object from a given character data buffer
 *@pre a buffer containing all information to create a individual object
 *@post A individual object is created and the GEDCOMobjects individual will point to it
 *@return GEDCOMerror - a error structure that contains the error information while creating a individual
 *@param buffer - a pointer to a character array containing the individual data
 *@param a double pointer to a individual struct that needs to be allocated
 **/
GEDCOMerror createIndividual(const char* buffer, Individual** individualObj, HashTable* hashMap);

/** Function to create a family object from a given character data buffer
 *@pre a buffer containing all information to create a family object
 *@post A family object is created and the GEDCOMobjects family will point to it
 *@return GEDCOMerror - a error structure that contains the error information while creating a family
 *@param buffer - a pointer to a character array containing the family data
 *@param a double pointer to a family struct that needs to be allocated
 **/
GEDCOMerror createFamily(const char* buffer, Family** family, HashTable* hashMap);

/** Function to link the family and individual objects in a gedcom file
 *@pre a file valid file containing atleast one record
 *@post A GEDCOMobject that has its family members and individuals linked
 *@return GEDCOMerror - a error structure that contains the error information about linking the members
 *@param file - A file pointer to the file that is already opened to the gedcom contents
 *@param familyRecord - a pointer to the GEDCOMobject that needs its members linked
 *@param hashMap - a pointer to the hashmap that stores the keys and addresses of he objects
 **/
GEDCOMerror linkIndividualsToFamilies(FILE* file, GEDCOMobject* familyRecord, HashTable* hashMap);

#endif