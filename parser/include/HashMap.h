/**
 * @file HashMap.h
 * @date January 2018
 * @brief function definitions for a hash map implementation
 * @Name: Alex Lai
 * @Student Number: 0920158
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#include "GEDCOMparser.h"

#define SIZE 500

// A data structure to store a unique key and an address to the data structure for that key
typedef struct {
	char* key;
	char* refTo;
	void* data;
} HtNode;

// A data structure to store HtNodes in a hash table format
typedef struct {
	int size;
	HtNode** table;
} HashTable;


/** Function to create a hashmap pointer and allocate memory to the data structure
 *@pre no pre requirements
 *@post A hashtable is created and stored in memory
 *@return HashTable* - A pointer to where the hashtable is located in memory
 **/
HashTable* createHashTable();

/** Function to create a hashmap pointer and allocate memory to the data structure
 *@pre no pre requirements
 *@post A hashtable is created and stored in memory
 *@return HtNode* - A pointer to the location of where the node is located
 *@param key - a pointer to a key value that is used to create the hash key
 *@param data - a pointer an address of data to be stored
 **/
HtNode* createHtNode(char* key, void* data);

/** Function to create a hash value with a given key
 *@pre a valid hashmap and a key which is not null
 *@post A hash value for where to store the HtNode in the table
 *@return int - A integer representing a unique location in the hashtable
 *@param key - a pointer to a key value that is used to create the hash key
 **/
int hashFunction(char* key);

/** Function to insert a HtNode in the hashtable
 *@pre a valid hashmap, a key which is not null and a address of data that is to be referenced
 *@post A HtNode composed of key and data is created and stored in the hashmap
 *@return void - No return value
 *@param hashmap - a pointer to a hashmap that stores data of type HtNode
 *@param key - a pointer to a key value that is used to create the hash key
 *@param data - a pointer to an address of where data is stored
 **/
void insertHtNode(HashTable* hashMap, char* key, void* data);

/** Function to get the data of a HtNode in the hashtable
 *@pre a valid hashmap, a key which is not null
 *@post A void pointer to a data object is returned
 *@return void* - A void pointer to data or NULL if data is not in list
 *@param hashmap - a pointer to a hashmap that stores data of type HtNode
 *@param key - a pointer to a key value that is mapped to a data element
 **/
void* getHtData(HashTable* hashMap, char* key);

/** Function to get a key in the hashtable that corresponds to a data element
 *@pre a valid hashmap, a void pointer to a data element stored in the hashmap
 *@post A key is returned for a data object
 *@return char* - a malloced char pointer to a key in the map, NULL if data is not in map
 *@param hashmap - a pointer to a hashmap that stores data of type HtNode
 *@param data - a void pointer to an address of where data is stored
 **/
char* getHtKey(HashTable* hashMap, void* data);

/** Function to release all the memory allocated to the hash table structure
 *@pre a valid hashmap that is not empty
 *@post Memory of the hashmap is released back to the system
 *@return void - No return value
 *@param hashmap - a pointer to a hashmap that stores data of type HtNode
 **/
void destroyHashTable(HashTable* hashMap);

/** Function to create and map a family gedcom record in a hashmap
 *@pre A valid gedcom object 
 *@post A hashtable is created and filled with keys created and data of the gedcom objects
 *@return HashTable* - A pointer to where the hashtable is located in memory
 **/
HashTable* mapKeys(const GEDCOMobject* gedcom);

#endif