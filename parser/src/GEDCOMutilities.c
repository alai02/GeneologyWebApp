#include "GEDCOMutilities.h"
#include "DATATYPEutilities.h"
#include "CREATEutilities.h"
#include "HashMap.h"
#include "OBJECTutilities.h"
#include "GEDCOMwriter.h"

void extractType(const char* buffer, char** type) {
	// initialize empty string
	*type = malloc(sizeof(char) * strlen("") +1);
	strncpy(*type, "", sizeof(char) * strlen("") +1);
	// validate arguement buffer
	if (buffer == NULL) {
		return;
	}
	char* line = strdup(buffer);
	if (line == NULL) {
		return; // malloc error
	}
	// delim line on white space
	char* key = strtok(line, " ");
	if (key != NULL) {
		key = strtok (NULL, " \n");
	}
	// reallocate memory and copy the string
	if (key != NULL) {
		*type = realloc(*type, sizeof(char) * strlen(key) +1);
		strncpy(*type, key, sizeof(char) * strlen(key) +1);
	}
	free(line); // free memory of line
}

void extractData(const char* buffer, char** data) {
	// initialize empty string
	*data = malloc(sizeof(char) * strlen("") +1);
	strncpy(*data, "", sizeof(char) * strlen("") +1);
	// validate buffer
	if (buffer == NULL) {
		return;
	}
	char* line = strdup(buffer);
	if (line == NULL) {
		return; // malloc error
	}
	// delim line into keys
	char* key = strtok(line, " ");
	if (key != NULL) {
		key = strtok (NULL, " ");
	}	
	if (key != NULL) {
		key = strtok (NULL, "\n");
	}
	if (key != NULL) {
		*data = realloc(*data, sizeof(char) * strlen(key) +1);
		strncpy(*data, key, sizeof(char) * strlen(key) +1);
	}
	free(line);
}

void extractSecondLineData(const char* buffer, char** data) {
	// initialize data incase of bad data
	*data = malloc(sizeof(char) * strlen("") +1);
	strncpy(*data, "", sizeof(char) * strlen("") +1);
	// validate buffer
	if (buffer == NULL) {
		return;
	}
	char* line = strdup(buffer);
	if (line == NULL) {
		return; // malloc error
	}
	// tokenize line
	char* key = strtok(line, "\n");
	if (key != NULL) {
		key = strtok(NULL, "\0"); // get rest of data in buffer
	}
	if (key != NULL) {
		*data = realloc(*data, sizeof(char) * strlen(key) +1);
		strncpy(*data, key, sizeof(char) * strlen(key) +1);
	}
	free(line);
}

void extractMultiLineData(const char* buffer, char**data) {
	// initialize data to empty string
	*data = malloc(sizeof(char) * strlen("") +1);
	strncpy(*data, "", sizeof(char)*strlen("") +1);
	// validate buffer
	if (buffer == NULL) {
		return;
	}
	char* line = strdup(buffer);
	if (line == NULL) {
		return;
	}
	// tokenize line	
	char* key = strtok(line, " ");
	if (key != NULL) {
		key = strtok(NULL, " "); // tokenize second space
	}
	if (key != NULL) {
		key = strtok(NULL, "\0"); // tokenize rest of data
	}
	if (key != NULL) {
		*data = realloc(*data, sizeof(char) * strlen(key) +1);
		strncpy(*data, key, sizeof(char) * strlen(key) +1);
	}
	free(line);
}

float extractFloatValue(const char* buffer) {
	float version = 0.0;
	if (buffer == NULL) {
		return version;
	}
	// create copy of buffer
	char* data = strdup(buffer);
	if (data == NULL) {
		return version;
	}
	// tokenize data
	char* key = strtok(data, " ");
	if (key != NULL) {
		key = strtok(NULL, " ");
	}
	if (key != NULL) {
		key = strtok(NULL, " \n");
	}
	if (key != NULL) {
		key = strtok(NULL, " ");
	}
	if (key != NULL) {
		version = atof(key); //get float value of version
	}
	free(data);
	return version;
}

CharSet extractEncodingValue(const char* buffer) {
	CharSet encoding = ASCII;
	if (buffer == NULL) {
		return encoding;
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return encoding;
	}
	char* key = strtok(data, " ");
	if (key != NULL) {
		key = strtok (NULL, " ");
	}
	if (key != NULL) {
		key = strtok (NULL, "\n");
	}
	if (key != NULL) {
		if (strcmp(key, "ANSEL") == 0) {
			encoding = ANSEL;
		// assuming both cases are correct
		} else if ((strcmp(key, "UTF8") == 0) || (strcmp(key, "UTF-8") == 0)) {
			encoding = UTF8;
		} else if (strcmp(key, "UNICODE") == 0) {
			encoding = UNICODE;
		} else if (strcmp(key, "ASCII") == 0) {
			encoding = ASCII;
		}
	}
	free(data);
	return encoding;
}

void extractFirstName(const char* buffer, char** givenName) {
	// initialize to empty string
	*givenName = malloc(sizeof(char) * strlen("") +1);
	strncpy(*givenName, "", sizeof(char) * strlen("") +1);
	// validate arguement
	if (buffer == NULL) {
		return;
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return;
	}
	char* key = strtok(data, " ");
	if (key != NULL) {
		key = strtok (NULL, " ");
	}
	if (key != NULL) {
		key = strtok (NULL, " /"); // get the first name token
	}
	if (key != NULL && strlen(key) > 1) { // ensure name is atleast 1 character
		*givenName = realloc(*givenName, sizeof(char) * strlen(key) +1);
		strncpy(*givenName, key, sizeof(char) * strlen(key) +1);
	}
	free(data);
}

void extractSurname(const char* buffer, char** surname) {
	*surname = malloc(sizeof(char) * strlen("") +1);
	strncpy(*surname, "", sizeof(char) * strlen("") +1);
	if (buffer == NULL) {
		return;
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return; // malloc error
	}
	// get last name between slashes
	char* key = strtok(data, "/");
	if (key != NULL) {
		key = strtok(NULL, "/");
	}
	if (key != NULL && strlen(key) > 1 && key[0] != '\n') {
		*surname = realloc(*surname, sizeof(char) * strlen(key) +1);
		strncpy(*surname, key, sizeof(char) * strlen(key) +1);
	}
	free(data);
}

void handleContinuedLines(const char* buffer, char** dataResult) {
	if (buffer == NULL) {
		return; // input buffer must be valid
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return; // malloc error
	}
	// declare local variables
	int i, j = 0, numArrays = 0, length, lineType; 
	char* multiLineData = NULL, * tempData = NULL;
	char* array[50]; // assume no more than 50 lines are passed in
	char line[255];
	memset(line, 0, 255);
	line[254] = '\0';
	// initialize char pointers to NULL
	*dataResult = NULL;
	for (i = 0; i < 50; i++) {
		array[i] = NULL;
	}
	// copy data into arrays
	for (i = 0; i <= strlen(data); i++) {
		if (data[i] == '\n') {
			array[numArrays] = malloc(sizeof(char) * strlen(line) +1);
			strncpy(array[numArrays++], line, strlen(line) +1);
			memset(line, 0, 255);
			line[254] = '\0';
			j = 0;
		} else {
			line[j++] = data[i];
		}
	}
	array[numArrays] = malloc(sizeof(char) * strlen(line) +1);
	strncpy(array[numArrays], line, strlen(line) +1);
	// loop through arrays starting at the end and concatinate continued lines
	for (i = numArrays; i >= 0; i--) {
		lineType = getMultiLineDataType(array[i]);
		if (lineType == 1) { // cont type
			// extract data from continued line
			extractMultiLineData(array[i], &tempData);
			length = strlen(array[i-1]) + sizeof(char) + strlen(tempData) + 10;
			multiLineData = malloc(sizeof(char) * length);
			// copy into temporary buffer
			strncpy(multiLineData, array[i-1], strlen(array[i-1]) + 1);
			strncat(multiLineData, "\n", sizeof(char));
			strncat(multiLineData, tempData, strlen(tempData) + 1);
			// reallocate line to longer length
			array[i-1] = realloc(array[i-1], sizeof(char) * strlen(multiLineData) +10);
			strncpy(array[i-1], multiLineData, strlen(multiLineData) +1);
			// free memory
			free(array[i]);
			array[i] = NULL;
			free(multiLineData);
			free(tempData);

		} else if (lineType == 2) { // conc type
			// extract data from continued line
			extractMultiLineData(array[i], &tempData);
			length = strlen(array[i-1]) + strlen(tempData) + 10;
			multiLineData = malloc(sizeof(char) * length);
			// concatinate data into a buffer
			strncpy(multiLineData, array[i-1], strlen(array[i-1]) + 1);
			strncat(multiLineData, tempData, strlen(tempData) + 1);
			// copy into temporary buffer
			array[i-1] = realloc(array[i-1], sizeof(char) * strlen(multiLineData) +10);
			strncpy(array[i-1], multiLineData, strlen(multiLineData) +1);
			// free memory
			free(array[i]);
			array[i] = NULL;
			free(multiLineData);
			free(tempData);
		}
	}
	// case where event record has a field with a continued line
	if (numArrays == 0 && strlen(array[0]) > 5) {
		length = strlen(array[0]) + sizeof(char) + 10;
		*dataResult = malloc(sizeof(char) * length);
		strncpy(*dataResult, array[0], strlen(array[0]) +1);
		strncat(*dataResult, "\n", sizeof(char));
	} else {
		length = 0; // get length of buffer to return
		for (i = 0; i < numArrays; i++) {
			if (array[i] != NULL) {
				length += strlen(array[i]) +1;
			}
		}
		if (array[0] != NULL) { // loop through data and create buffer
			*dataResult = malloc(sizeof(char) * length +10);
			strncpy(*dataResult, array[0], strlen(array[0]) +1); // strcpy first element
			strncat(*dataResult, "\n", sizeof(char));
			for (i = 1; i < numArrays; i++) {
				if (array[i] != NULL) { // concatinate arrays into one dataResult pointer
					strncat(*dataResult, array[i], strlen(array[i]) +1);
					strncat(*dataResult, "\n", sizeof(char));
				}
			}
		} else { // invalid data. cannot parse, first line must be valid and proper type 
			*dataResult = NULL;
		}
	}
	// free data buffer and pointer array
	free(data);
	for (i = 0; i < 50; i++) {
		if (array[i] != NULL) {
			free(array[i]);
		}
	}
}

/* Collection of functions for the DATATYPEutilities header */

int getMultiLineDataType(const char* buffer){
	int dataType = -1;
	if (buffer == NULL) {
		return dataType; // invalid input pointer
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return dataType; // malloc error
	}
	// continued line tags
	static const char* continuedTypes[] = {"CONT", "CONC"};
	char* key = strtok(data, " ");
	if (key != NULL) {
		key = strtok(NULL, " ");
	}
	if (key != NULL) {
		if (strcmp(key, continuedTypes[0]) == 0) {
			dataType = 1; //cont = last + \n + current
		} else if (strcmp(key, continuedTypes[1]) == 0) {
			dataType = 2; //conc = last + current
		} else {
			dataType = 0; //is a field or other value
		}
	}
	free(data);
	return dataType;
}

int getEventDataType(const char* buffer) {
	int dataType = -1;
	if (buffer == NULL) {
		return dataType;
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return dataType; //malloc error
	}
	static const char* eventMembers[] = {"DATE", "PLAC"};
	static const char* eventList[] = {"MARR", "BIRT", "DEAT", "CHR", "ENGA", "MARB", "MARC", "MARR", "MARL", "MARS", "RESI", "EVEN", "BIRT", "CHR", "DEAT", "BURI", "CREM", "ADOP", "BAPM", "BARM", "BASM", "BLES", "CHRA", "CONF", "FCOM", "ORDN", "NATU", "EMIG", "IMMI", "CENS", "PROB", "WILL", "GRAD", "RETI", "ANUL", "CENS", "DIV", "DIVF"};
	int eventListLen = sizeof(eventList) / sizeof(eventList[0]);
	char* key = strtok(data, " ");
	if (key != NULL) {
		key = strtok(NULL, " \n");
	}
	if (key != NULL) {
		int i;
		for (i = 0; i < eventListLen; i++) {
			if (strcmp(key, eventList[i]) == 0) {
				dataType = 0; //list of possible events
			}
		}
		if (strcmp(key, eventMembers[0]) == 0) {
			dataType = 1; // is date member
		} else if (strcmp(key, eventMembers[1]) == 0) {
			dataType = 2; // is place member
		}
		if (dataType == -1) {
			dataType = 3; // is a field member
		}
	}
	free(data);
	return dataType;
}

int getHeaderDataType(const char* buffer) {
	int dataType = -1;
	if (buffer == NULL) {
		return dataType;
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return dataType;
	}
	static const char* headerTypeList[] = {"HEAD", "SOUR", "GEDC", "CHAR", "SUBM"};
	char* key = strtok (data, " ");
	if (key != NULL) {
		key = strtok(NULL, " \n");
	}
	if (key != NULL) {
		if (strcmp(key, headerTypeList[0]) == 0) {
			dataType = 0;
		} else if (strcmp(key, headerTypeList[1]) == 0) {
			dataType = 1;
		} else if (strcmp(key, headerTypeList[2]) == 0) {
			dataType = 2;
		} else if (strcmp(key, headerTypeList[3]) == 0) {
			dataType = 3;
		} else if (strcmp(key, headerTypeList[4]) == 0) {
			dataType = 4;
		} else {
			dataType = 5;
		}
	}
	free(data);
	return dataType;
}

int getSubmitterDataType(const char* buffer) {
	int dataType = -1;
	if (buffer == NULL) {
		return dataType;
	}
	char* data = strdup(buffer);	
	if (data == NULL) {
		return dataType; // malloc error
	}
	static const char* submitterFieldList[] = { "SUBM", "NAME", "ADDR" };
	char* key = strtok(data, " ");
	if (key != NULL) {
		key = strtok(NULL, " ");
	}
	if (key != NULL) {
		if (strcmp(key, submitterFieldList[1]) == 0) {
			dataType = 1;
		} else if (strcmp(key, submitterFieldList[2]) == 0) {
			dataType = 2;
		}
		key = strtok(NULL, "\n");
	}
	if (key != NULL && dataType == -1) {
		if (strcmp(key, submitterFieldList[0]) == 0) {
			dataType = 0;
		} else {
			dataType = 3;
		}
	}
	free(data);
	return dataType;
}

int getIndiDataType(const char* buffer) {
	int dataType = -1;
	if (buffer == NULL) {
		return dataType;
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return dataType; // malloc error
	}
	// lists for possible tags in an individual record
	static const char* indiTypeList[] = {"INDI", "NAME", "FAMC", "FAMS"};
	static const char* indiEventList[] = {"BIRT", "DEAT", "CHR", "BURI", "CREM", "ADOP", "BAPM"};
	int indiEventListLen = sizeof(indiEventList)/sizeof(indiEventList[0]);	
	char* key = strtok(data, " ");
	if (key != NULL) {
		key = strtok(NULL, " \n");
	}
	if (key != NULL) {
		if (strcmp(key, indiTypeList[1]) == 0) {
			dataType = 1;
		} else if (strcmp(key, indiTypeList[2]) == 0) {
			dataType = 2;
		} else if (strcmp(key, indiTypeList[3]) == 0) {
			dataType = 3;
		}
		int i;
		for (i = 0; i < indiEventListLen; i++) {
			if (strcmp(key, indiEventList[i]) == 0) {
				dataType = 4;
			}
		}
		key = strtok(NULL, " \n");
	}
	if (key != NULL && dataType == -1) {
		if (strcmp(key, indiTypeList[0]) == 0) {
			dataType = 0;
		} else {
			dataType = 5;
		}
	}
	free(data);
	return dataType;
}

int getFamDataType(const char* buffer) {
	int dataType = -1;
	if (buffer == NULL) {
		return dataType; // invalid input pointer
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return dataType; // malloc error
	}
	// list of possible tags in a family object
	static const char* famTypeList[] = {"FAM", "WIFE", "HUSB", "CHIL", "FAMC"};
	static const char* famEventList[] = {"ANUL" "CENS", "DIV", "DIVF", "ENGA", "MARR", "MARB", "MARC" ,"MARL", "MARS", "EVEN"};
	int famEventListLen = sizeof(famEventList)/sizeof(famEventList[0]);
	char* key = strtok(data, " ");
	if (key != NULL) {
		key = strtok(NULL, " \n");
	}
	if (key != NULL) {
		if (strcmp(key, famTypeList[1]) == 0) {
			dataType = 1;
		} else if (strcmp(key, famTypeList[2]) == 0) {
			dataType = 2;
		} else if (strcmp(key, famTypeList[3]) == 0) {
			dataType = 3;
		}
		int i;
		for (i = 0; i < famEventListLen; i++) {
			if (strcmp(key, famEventList[i]) == 0) {
				dataType = 5;
			}
		}
		key = strtok(NULL, " \n");
	}
	if (key != NULL && dataType == -1) {
		if (strcmp(key, famTypeList[0]) == 0) {
			dataType = 0;
		} else {
			dataType = 4;
		}
	}
	free(data);
	return dataType;
}

int getRecordType(const char* buffer) {
	int dataType = -1;
	if (buffer == NULL) {
		return dataType;
	}
	char* data = strdup(buffer);
	if (data == NULL) {
		return dataType; // malloc error
	}
	static const char* recordTypeList[] = {"HEAD", "INDI", "FAM", "SUBM", "TRLR"};
	char* key = strtok (data, " ");
	if (key != NULL) {
		key = strtok (NULL, " \n");
	}
	if (key != NULL) {
		if (strcmp(key, recordTypeList[0]) == 0) {
			dataType = 0;
		} else if (strcmp(key, recordTypeList[4]) == 0) {
			dataType = 4;
		}
		key = strtok (NULL, " \n");
	}
	if (key != NULL && dataType == -1) {
		if (strcmp(key, recordTypeList[1]) == 0) {
			dataType = 1;
		} else if (strcmp(key, recordTypeList[2]) == 0) {
			dataType = 2;
		} else if (strcmp(key, recordTypeList[3]) == 0) {
			dataType = 3;
		} else {
			dataType = 5;
		}
	}
	free(data);
	return dataType;
}

/* Collection of functions for the HashMap header */

HashTable* createHashTable() {
	// allocate memory for hash table 
	HashTable* hashMap = malloc(sizeof(HashTable));
	hashMap->size = 0; // initialize to zero
	hashMap->table = malloc(sizeof(HtNode) * SIZE); // allocate hashtable with size 500
	int i; 
	for (i = 0; i < SIZE; i++) {
		hashMap->table[i] = NULL; // initilize each table element to null
	}
	return hashMap;
}

HtNode* createHtNode(char* key, void* data) {	
	HtNode* node = malloc(sizeof(HtNode)); // allocate memory for node in hashtable
	node->key = malloc(sizeof(char) * strlen(key) +1); // allocate memory with length of key
	strncpy(node->key, key, strlen(key) +1);  // copy the key value
	node->data = data; // set the void pointer data
	return node;
}

int hashFunction(char* key) {
	// hash function to create unique key index
	int i, len = strlen(key);
	unsigned long int  hash = 0; // use unsigned int for hash math
	for (i = 0; i < len; i++) {
		hash = (hash * 31) + key[i]; // use each key character to ensure uniqueness
	}
	hash = hash % 100; // get remainder of hash / 100
	// create seperation for individual and familiy keys
	if (key[1] == 'I') { 
		hash += 250;
	} else if (key[1] == 'F') {
		hash += 101;
	}
	return hash;
}

void insertHtNode(HashTable* hashMap, char* key, void* data) {
	HtNode* toAdd = createHtNode(key, data);
	int hashIndex = hashFunction(key); // get the hash value
	hashMap->table[hashIndex] = toAdd; // place the node at the hash index
	hashMap->size++; // increase size of map
}

void* getHtData(HashTable* hashMap, char* key) {	
	int hashIndex = hashFunction(key); // get the hash value for the key
	if (hashMap->table[hashIndex] == NULL) { // no data at key
		return NULL;
	}
	return hashMap->table[hashIndex]->data;
}

char* getHtKey(HashTable* hashMap, void* data) {
	int i;
	for (i = 0; i < SIZE; i++) {
		if (hashMap->table[i] != NULL) {
			if (data == hashMap->table[i]->data) {
				return hashMap->table[i]->key;
			}
		}
	}
	return NULL;
}

void destroyHashTable(HashTable* hashMap) {
	int i;
	for (i = 0; i < SIZE; i++) { // free each node in the table
		if (hashMap->table[i]) {
			free(hashMap->table[i]->key);
		}
		free(hashMap->table[i]);
	}	
	free(hashMap->table); // destroy the hash table
	free(hashMap);
}
	
HashTable* mapKeys(const GEDCOMobject* gedcom) {
	// create new hashtable
	HashTable* hashMap = createHashTable();
	if (hashMap == NULL) {
		return NULL;
	}
	int i = 1;
	ListIterator personItr = createIterator(gedcom->individuals);
	void* personElem;
	while( (personElem = nextElement(&personItr)) != NULL) {
		char* xref = writeXREF(i, 'I');	
		if (xref != NULL) {
			insertHtNode(hashMap, xref, personElem);
			i++;
		}		
		free(xref);
	}

	i = 1;
	ListIterator familyItr = createIterator(gedcom->families);
	void* familyElem;
	// create and insert family keys
	while( (familyElem = nextElement(&familyItr)) != NULL) {
		char* xref = writeXREF(i, 'F');	
		if (xref != NULL) {
			insertHtNode(hashMap, xref, familyElem);
			i++;
		}		
		free(xref);
	}
	return hashMap;
}

/* Collection of functions for the CREATEutilities header */

int createField(const char* buffer, List* list) {
	// invalid parameters pointers
	if (buffer == NULL || list == NULL) {
		return -1;
	}
	char* data = NULL;
	handleContinuedLines(buffer, &data);
	if (data == NULL) {		
		free(data);
		return -1; // invlalid format for field
	}
	// create new field object
	Field* newField = malloc(sizeof(Field));
	newField->tag = NULL;
	newField->value = NULL;
	// function allocates memory and sets the values
	extractType(data, &newField->tag);
	extractData(data, &newField->value);
	free(data);
	// ensure field obj was created successfully
	if (newField->tag != NULL && newField->value != NULL) {
		insertBack(list, (void*) newField);
	} else {
		deleteField((void*) newField);
		return -1;
	}
	return 0;
}

int createEvent(const char* buffer, List* list) {
	if (buffer == NULL || list == NULL) {
		return -1;
	}
	char* data;
	handleContinuedLines(buffer, &data);
	if (data == NULL) {
		free(data);
		return -1;
	}
	// initialize a new event
	Event* newEvent = malloc(sizeof(Event));
	newEvent->date = NULL;
	newEvent->place = NULL;
	newEvent->otherFields = initializeList(&printField, &deleteField, &compareFields);
	List* listPtr = &newEvent->otherFields;
	bool typeSet = false;
	int i, j = 0, eventType, listError;
	char line[255];
	char* tempData = NULL;	

	for (i = 0; i < strlen(data); i++) {
		if (data[i] == '\n') {
			line[j] = '\0';
			eventType = getEventDataType(line);
			switch(eventType) {
				case 0: // type case
					tempData = NULL;
					extractType(line, &tempData);
					if (strlen(tempData) < 6) {
						strcpy(newEvent->type, tempData);
						typeSet = true;
					} else { // throw error, type must be <= 5						
						free(data);
						deleteEvent((void*) newEvent);						
						return -1;
					}
					free(tempData);
					break;
				case 1: // date type
					if (newEvent->date == NULL) {
						extractData(line, &newEvent->date);
					}
					break;
				case 2: // place type
					if (newEvent->place == NULL) {
						extractData(line, &newEvent->place);
					}
					break;
				case 3: // other fields type
					listError = createField(line, listPtr);
					if (listError != 0) {
						free(data);
						deleteEvent((void*) newEvent);						
						return -1;
					}
					break;
				default: // invalid case
					free(data);
					deleteEvent((void*) newEvent);						
					return -1;
					break;
			}
			memset(line, 0, 255);
			j = 0;
		} else {
			line[j++] = data[i];
		}
	}
	// if no date or place values, set to empty strings
	if (newEvent->date == NULL) {
		newEvent->date = malloc(sizeof(char) * strlen("") +1);
		strncpy(newEvent->date, "", sizeof(char) * strlen("") +1);
	}
	if (newEvent->place == NULL) {
		newEvent->place = malloc(sizeof(char) * strlen("") +1);
		strncpy(newEvent->place, "", sizeof(char) * strlen("") +1);
	}
	free(data);
	if (typeSet == true) {		
		insertBack(list, (void*) newEvent); // add to list
	} else {
		deleteEvent((void*) newEvent);
		return -1;
	}
    return 0;
}

GEDCOMerror createHeader(const char* buffer, Header** headerObj, HashTable* hashMap) {
	// initilize header error and headerObj to NULL incase of failure
	GEDCOMerror headerErr = initializeError();
	*headerObj = NULL;
	if (buffer == NULL || hashMap == NULL) {
		headerErr.type = OTHER_ERROR; // input parameters must be valid
		return headerErr;
	}
	// allocate memory for header
	Header* header = malloc(sizeof(Header));
	char* dataBuffer = strdup(buffer); // create a copy of the buffer
	if (dataBuffer == NULL || header == NULL) { // ensure malloc did not fail
		free(dataBuffer);
		free(header);
		headerErr.line = 0;
		headerErr.type = OTHER_ERROR;
		return headerErr;
	}
	// initialize header members
	header->submitter = NULL;
	header->otherFields = initializeList(&printField, &deleteField, &compareFields);	
	char* submitterKey = NULL, * tempData = NULL;
	// local variables for flags, indexes and data
	bool headerFlag = false, sourceFlag = false, gedcFlag = false, charsetFlag = false, submitterFlag = false;
	int i, j = 0, lineNumber = 0, lastLevel = 0, level, dataType, listError;
	char data[2000];
	memset(data, 0, 2000);
	// check non-zero header number
	if (dataBuffer[0] - '0' != 0) {
		headerErr.line = lineNumber;
		headerErr.type = INV_HEADER;
	}
	// loop through data in header record
	for (i = 0; i < strlen(dataBuffer); i++) {
		if (dataBuffer[i] == '\n') {
			lineNumber++;
			level = dataBuffer[i+1] - '0';
			data[j++] = dataBuffer[i];
			data[j] = '\0';
			if (level <= lastLevel || lastLevel == 0 || i+1 >= strlen(dataBuffer)) {
				dataType = getHeaderDataType(data);
				switch (dataType) {
					case 0: // header ref case
						if (lastLevel != 0 || headerFlag == true) { // multiple header refrences, return header error
							headerErr.line = -1;
							headerErr.type = INV_GEDCOM;							
						} else {							
							headerFlag = true;
						}
						break;
					case 1: // source case
						if (sourceFlag == false) {
							extractData(data, &tempData);
							if (strlen(tempData) < 249) { // source must be < 249 chars
								strcpy(header->source, tempData);
								sourceFlag = true;
							} else {
								headerErr.line = lineNumber;
								headerErr.type = INV_HEADER;
							}
							free(tempData);
						}
						// case where data is stored in source record
						extractSecondLineData(data, &tempData);
						if (tempData != NULL && strcmp(tempData, "") != 0) {
							listError = createField(tempData, &header->otherFields);
							if (listError != 0) {
								headerErr.line = lineNumber;
								headerErr.type = INV_HEADER;
							}
						}
						free(tempData);
						break;
					case 2: // gedc version case
						if (gedcFlag == false) { // ensure manditory members set
							header->gedcVersion = extractFloatValue(data);
							if (header->gedcVersion != 0.0) {
								gedcFlag = true;
							}
						}
						break;
					case 3: // charset case
						if (charsetFlag == false) { // ensure manditory members set
							header->encoding = extractEncodingValue(data);
							charsetFlag = true;
						}
						break;
					case 4: // submitter case
						if (submitterFlag == false) { // ensure manditory members set
							extractData(data, &submitterKey);
							submitterFlag = true;
						}
						break;
					case 5: // other field case						
						listError = createField(data, &header->otherFields);
						if (listError != 0) {
							headerErr.line = lineNumber;
							headerErr.type = INV_HEADER;
						}
						break;
					default: // unknown case
						headerErr.type = INV_HEADER;
						headerErr.line = lineNumber;
						break;
				}
				memset(data, 0, 2000);
				j = 0;
			}
			lastLevel = level;
		} else {
			data[j++] = dataBuffer[i];
		}
		// check for errors
		if (headerErr.type != OK) {
			break;
		}
	}
	// ensure valid members set
	if (headerFlag == false || sourceFlag == false || gedcFlag == false || charsetFlag == false || submitterKey == NULL) {
		headerErr.line = lineNumber;		
		headerErr.type = INV_HEADER;		
	}
	free(dataBuffer);
	// ensure no erros have occured
	if (headerErr.type == OK) {
		// Add submitter key to hashmap as key and set the value to header
		insertHtNode(hashMap, submitterKey, (void*) header);
		free(submitterKey);
	} else {
		free(submitterKey);
		deleteHeader((void*) header);
		return headerErr;
	}
	// point header pointer to header created
	*headerObj = header;
	return headerErr;
}

GEDCOMerror createSubmitter(const char* buffer, Submitter** submitterObj, HashTable* hashMap) {
	// initialize error and submitterObj to NULL incase of failure
	GEDCOMerror submitterErr = initializeError();
	*submitterObj = NULL;
	// input pointers must be valid
	if (buffer == NULL || hashMap == NULL) {
		submitterErr.type = OTHER_ERROR;
		return submitterErr;
	}
	// allocate memory for submitter object and data buffer
	Submitter* submitter = malloc(sizeof(Submitter) + sizeof(char) * strlen("") +1);
	char* dataBuffer = NULL;
	// clean up buffer and handle data for continued lines
	handleContinuedLines(buffer, &dataBuffer);
	if (dataBuffer == NULL || submitter == NULL) {
		free(dataBuffer);
		free(submitter);	
		submitterErr.line = 0;
		submitterErr.type = OTHER_ERROR;
		return submitterErr;
	}
	// initialize members for submitter
	strncpy(submitter->address, "", strlen("") +1);
	submitter->otherFields = initializeList(&printField, &deleteField, &compareFields);
	List* listPtr = &submitter->otherFields;
	// create temporary variables for members
	char* name = NULL, * address = NULL, * submitterKey = NULL;	
	// create local variables, data buffers, 
	bool nameFlag = false, addressFlag = false, keyFlag = false;
	int i, j = 0, lineNumber = 0, lastLevel = 0, level, dataType, listError;
	char data[1000];
	memset(data, 0, 1000);

	for (i = 0; i < strlen(dataBuffer); i++) {
		if (dataBuffer[i] == '\n') {
			lineNumber++;
			level = dataBuffer[i+1] - '0';
			data[j++] = dataBuffer[i];
			data[j] = '\0';
			if (level <= lastLevel || lastLevel == 0 || i+1 >= strlen(dataBuffer)) {
				dataType = getSubmitterDataType(data);
				switch (dataType) {
					case 0:
						if (keyFlag == false) {					
							extractType(data, &submitterKey);
							keyFlag = true;
						}
						break;
					case 1:
						if (nameFlag == false) {
							extractData(data, &name);
							if (name != NULL && strlen(name) < 61) {
								strncpy(submitter->submitterName, name, sizeof(char) * strlen(name) +1);
								nameFlag = true;
							} else {
								submitterErr.line = lineNumber;
								submitterErr.type = INV_RECORD;
							}
						}
						break;
					case 2:
						if (addressFlag == false) {
							extractData(data, &address);
							if (address != NULL && strcmp(address, "") != 0) {
								addressFlag = true;
							}
						}
						break;
					case 3:
						listError = createField(data, listPtr);
						if (listError != 0) {
							submitterErr.type = INV_RECORD;
							submitterErr.line = lineNumber;
						}
						break;
					default:
						submitterErr.type = INV_RECORD;
						submitterErr.line = lineNumber;
						break;
				}
				lastLevel = level;
				memset(data, 0, 1000);
				j = 0;
			}
		} else {
			data[j++] = dataBuffer[i];
		}
		// check for errors
		if (submitterErr.type != OK) {
			break;
		}
	}
	if (submitterKey == NULL || nameFlag == false) {
		submitterErr.line = 0;
		submitterErr.type = INV_RECORD;
	}
	// realloc submitter object to include size of address
	if (submitterErr.type == OK && addressFlag == true) {
		int newLen = sizeof(Submitter) + sizeof(char) * strlen(address) +10;
		submitter = realloc(submitter, newLen);
		strncpy(submitter->address, address, strlen(address) +1);
	}
	if (submitterErr.type == OK && keyFlag == true) {
		// Use the submitter key to search the hashmap and get the value of the header and set it to the submitter object
		void* headerData = getHtData(hashMap, submitterKey);
		// the submitter key must match the header reference to the submitter key
		if (submitterErr.type == OK && headerData != NULL) {
			// set the headers submitter pointer to the data
			Header* tempHeader = (Header*) headerData;
			tempHeader->submitter = submitter;
		} else {
			submitterErr.line = -1;
			submitterErr.type = INV_GEDCOM;
		}
	}
	free(dataBuffer);
	free(name);
	free(address);
	free(submitterKey);
	if (submitterErr.type != OK) {
		deleteSubmitter((void*) submitter);
		return submitterErr;
	}
	// Point submitterObj to submitter pointer which holds the data
	*submitterObj = submitter;
	return submitterErr;
}

GEDCOMerror createIndividual(const char* buffer, Individual** individualObj, HashTable* hashMap) {
	// initialize error and individualObj to NULL incase of failure
	GEDCOMerror personErr = initializeError(); 
	*individualObj = NULL;
	// check for valid input parameters
	if (buffer == NULL || hashMap == NULL) {
		personErr.type = OTHER_ERROR;
		return personErr;
	}	
	// allocate memory for person and data buffer
	Individual* individual = malloc(sizeof(Individual));
	char* dataBuffer = strdup(buffer);
	// check for malloc failure
	if (dataBuffer == NULL || individual == NULL) {
		free(dataBuffer);
		free(individual);
		personErr.type = OTHER_ERROR;		
		return personErr;
	}
	// initialize individual members
	individual->givenName = NULL;
	individual->surname = NULL;
	individual->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
	individual->otherFields = initializeList(&printField, &deleteField, &compareFields);
	individual->families = initializeList(&printFamily, &deleteWeakReference, &compareFamilies);

	// create local variables
	char* individualKey = NULL;	
	bool personKeyFlag = false;
	int i, j = 0, lineNumber = 0, lastLevel = 0, level, dataType, listError;
	char data[2000];
	memset(data, 0, 2000);
	
	for (i = 0; i < strlen(dataBuffer); i++) {
		if (dataBuffer[i] == '\n') {
			lineNumber++;
			level = dataBuffer[i+1] - '0';
			data[j++] = dataBuffer[i];
			data[j] = '\0';
			if (level <= lastLevel || lastLevel == 0 || i+1 >= strlen(dataBuffer)) {
				dataType = getIndiDataType(data);
				switch (dataType) {
					case 0: // Indi key case
						if (personKeyFlag == false) {
							extractType(data, &individualKey);
							personKeyFlag = true;
						}
						break;
					case 1: // name case (may be multi level)
						if (individual->givenName == NULL) {	
							extractFirstName(data, &individual->givenName);
						}
						if (individual->surname == NULL) {
							extractSurname(data, &individual->surname);
						}
						if (strcmp(individual->givenName, individual->surname) == 0) {
							free(individual->givenName);
							individual->givenName = NULL;
							individual->givenName = malloc(sizeof(char) * strlen("") +1);
							strncpy(individual->givenName, "", sizeof(char) * strlen("") +1);
						}
						break;
					case 2: // fam child case
						break;
					case 3: // fam spouse case
						break;
					case 4: // event case
						listError = createEvent(data, &individual->events);
						if (listError != 0) {
							personErr.line = lineNumber;
							personErr.type = INV_RECORD;
						}
						break;
					case 5: // other field case												
						listError = createField(data, &individual->otherFields);
						if (listError != 0) {
							personErr.line = lineNumber;
							personErr.type = INV_RECORD;
						}
						break;
					default:
						personErr.line = lineNumber;
						personErr.type = INV_RECORD;
						break;
				}
				lastLevel = level;
				memset(data, 0, 2000);
				j = 0;
			}
		} else {
			data[j++] = dataBuffer[i];
		}
		// check for errors
		if (personErr.type != OK) {
			break;
		}
	}
	// free memory
	free(dataBuffer);
	// check if individual has xref id
	if (personKeyFlag == false) {
		personErr.line = 0;
		personErr.type = INV_RECORD;
	}
	// if valid record, add to hashmap
	if (personErr.type == OK) {
		insertHtNode(hashMap, individualKey, (void*) individual);
		free(individualKey);
	} else {
		free(individualKey);
		deleteIndividual((void*) individual);
		return personErr;
	}
	// set individualObj pointer to individual pointer
	*individualObj = individual; 
	return personErr;
}

GEDCOMerror createFamily(const char* buffer, Family** familyObj, HashTable* hashMap) {
	// initialize return value and set familyObj to NULL incase of failure
	GEDCOMerror familyErr = initializeError();
	*familyObj = NULL;
	// ensure input parameters are valid
	if (buffer == NULL || hashMap == NULL) {
		familyErr.type = OTHER_ERROR;
		return familyErr;
	}	
	// allocate memory for family object and data buffer
	Family* family = malloc(sizeof(Family));
	char* dataBuffer = strdup(buffer);
	// validate malloc success
	if (dataBuffer == NULL || family == NULL) {
		free(dataBuffer);
		free(family);
		familyErr.line = 0;
		familyErr.type = OTHER_ERROR;		
		return familyErr;
	}
	// initialize family members
	family->wife = NULL;
	family->husband = NULL;
	family->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
	family->otherFields = initializeList(&printField, &deleteField, &compareFields);
	family->children = initializeList(&printIndividualName, &deleteWeakReference, &compareIndividuals);
	// pointers for family reference keys
	char* familyKey = NULL;
	// , * wifeKey = NULL, * husbandKey = NULL, * childKeys[20] = { NULL };
	// local variables, data buffers, and flags
	bool keyFlag = false;
	// , wifeKeyFlag = false, husbandKeyFlag = false;
	// childKeyCount = 0, 
	int i, j = 0, lineNumber = 0, lastLevel = 0, level, dataType, listError;
	char data[3000];
	memset(data, 0, 3000);

	for (i = 0; i < strlen(dataBuffer); i++) {
		if (dataBuffer[i] == '\n') {
			lineNumber++;
			level = dataBuffer[i+1] - '0';
			data[j++] = dataBuffer[i];
			data[j] = '\0';
			if (level <= lastLevel || lastLevel == 0 || i+1 >= strlen(dataBuffer)) {
				dataType = getFamDataType(data);
				switch (dataType) {
					case 0: // Family XREF ID						
						if (keyFlag == false) {
							extractType(data, &familyKey);
							keyFlag = true;
						}
						break;
					case 1: // wife XREF ID case
						break;
					case 2: // husband XREF ID case
						break;
					case 3: // child XREF ID case
						break;
					case 4: //other field case
						listError = createField(data, &family->otherFields);
						if (listError != 0) {
							familyErr.line = lineNumber;
							familyErr.type = INV_RECORD;
						}
						break;
					case 5: // events case						
						listError = createEvent(data, &family->events);
						if (listError != 0) {
							familyErr.line = lineNumber;
							familyErr.type = INV_RECORD;
						}
						break;
					default:
						familyErr.line = lineNumber;
						familyErr.type = INV_RECORD;
						break;
				}
				lastLevel = level;
				memset(data, 0, 3000);
				j = 0;
			}
		} else {
			data[j++] = dataBuffer[i];
		}
		if (familyErr.type != OK) {
			break;
		}
	}
	if (familyKey == NULL) {				
		familyErr.line = 0;
		familyErr.type = INV_RECORD;
	}
	free(dataBuffer);
	if (familyErr.type == OK) {
		insertHtNode(hashMap, familyKey, (void*) family);
		free(familyKey);
	} else {
		free(familyKey);
		deleteFamily((void*) family);
		return familyErr;
	}
	*familyObj = family;
	return familyErr;
}

GEDCOMerror linkIndividualsToFamilies(FILE* file, GEDCOMobject* familyRecord, HashTable* hashMap) {
	// initialize error to -1 and OK
	GEDCOMerror linkErr = initializeError();
	if (file == NULL) { // failed to re-open the file
		linkErr.type = INV_FILE;
		return linkErr;
	}
	rewind(file);

	char familyLine[255] = {0};
	memset(familyLine, 0, 255);
	
	char line[500] = {0};
	memset(line, 0, 500);

	char recordBuffer[1000];
	memset(recordBuffer, 0, 1000);
	
	char c;
	Family* family = NULL;
	char* famMemberKey = NULL, * famKey = NULL;
	void* familyPtr, * wifePtr, * husbandPtr, * childPtr;
	int i, j = 0, recordType, personType = 0, n = 0, level = -1, lineNumber = 0;
	
	while ( ((c = fgetc(file)) != EOF) && (linkErr.type == OK) ) {
		line[n++] = (char) c;
		level = line[0] - '0'; // get level of the current line
		if (level == 0 && lineNumber > 0) {		
			recordType = getRecordType(recordBuffer);
			switch (recordType) {
				case 2: // family record case		
					extractType(recordBuffer, &famKey);					
					familyPtr = getHtData(hashMap, famKey);
					family = (Family*) familyPtr;
					for(i = 0; i < strlen(recordBuffer); i++) {
						if (recordBuffer[i] == '\n' && i > 5) {
							personType = getFamDataType(familyLine);
							extractData(familyLine, &famMemberKey);
							switch (personType) {
								case 1:	// wife ID
									wifePtr = getHtData(hashMap, famMemberKey);
									if (wifePtr != NULL) {
										family->wife = wifePtr;
										insertBack(&family->wife->families, (void*) family);	
									}									
									break;
								case 2: // husband ID
									husbandPtr = getHtData(hashMap, famMemberKey);
									if (husbandPtr != NULL) {
										family->husband = husbandPtr;
										insertBack(&family->husband->families, (void*) family);
									}							
									break;
								case 3: // child ID									
									childPtr = getHtData(hashMap, famMemberKey);
									if (childPtr != NULL) {
										Individual* child = (Individual*) childPtr;									
										insertBack(&family->children, (void*) child);
										insertBack(&child->families, (void*) family);
									}			
									break;
								default: 
									break; 
							}
							free(famMemberKey);					
							memset(familyLine, 0, 255);
							familyLine[254] = '\0';
							j = 0;
						}
						familyLine[j++] = recordBuffer[i];
					}
					free(famKey);
					break;
				default:
					break;
			}
			memset(recordBuffer, ' ', 1000);
			strncpy(recordBuffer, line, strlen(line) +1);
			memset(line, 0, 500);
			line[499] = '\0';
			n = 0;
		}
		int len = strlen(line);
		if (len >= n && n > 2) { 
			if (line[n-2] == '\n' || line[n-2] == '\r') {
				if (line[n-1] == '\n' || line[n-1] == '\r') {
					line[n-2] = '\n'; // null terminate line to string
					line[n-1] = '\0'; // null terminate line to string
					strncat(recordBuffer, line, strlen(line) +1);
					memset(line, 0, 500);
					line[499] = '\0';
					n = 0;
				} else {
					line[n-2] = '\n';
					char tempc = line[n-1];
					line[n-1] = '\0';
					strncat(recordBuffer, line, strlen(line) +1);
					memset(line, 0, 500);
					line[499] = '\0';
					line[0] = tempc;
					n = 1;
				}
			}
			lineNumber++;
		}
	}
	fclose(file);
	return linkErr;
}

/* Collection of functions for the OBJECTutilities header */

GEDCOMerror initializeError() {
	GEDCOMerror err; // initializes an error object to no error
	err.line = -1; // initialize no error line -1
	err.type = OK; // initialize error type to OK
	return err;
}

List getChildren(List families, const Individual* person) {
	// create a list of descandants that we will store the individual copies in
	List children = initializeList(&printIndividualName, &deleteIndividual, &compareIndividuals);	
	if (getLength(families) == 0 || person == NULL) {
		return children; // families list must be > 0 and Individual pointer must be valid
	}
	ListIterator famItr = createIterator(families);
	void* famElem; // create an iterator for the families list
	// Loop through the families list until it is empty
	while ( (famElem = nextElement(&famItr)) != NULL) { 
		// cast the void pointer to family type to access members
		Family* tempFam = (Family*) famElem; 
		if (tempFam->wife != NULL && compareIndividuals(tempFam->wife, person) == 0) {
			if (getLength(tempFam->children) > 0) {
				ListIterator childItr = createIterator(tempFam->children);
				void* childElem;
				while ( (childElem = nextElement(&childItr) ) != NULL) {
					Individual* tempChild = (Individual*) childElem;
					if (getLength(tempChild->families) > 1) {
						List grandchildList = initializeList(&printIndividualName, &deleteIndividual, &compareIndividuals);
						grandchildList = getChildren(tempChild->families, tempChild);
						ListIterator grandChildItr = createIterator(grandchildList);
						void* grandchildElem;
						while ( (grandchildElem = nextElement(&grandChildItr)) != NULL) {
							Individual* tempGrandchild = (Individual*) grandchildElem;
							Individual* toAdd;
							copyIndividual(&toAdd, tempGrandchild);
							insertBack(&children, (void*) toAdd);				
						}
						clearList(&grandchildList);
					} else {
						Individual* toCopy;
						copyIndividual(&toCopy, tempChild);
						insertBack(&children, (void*) toCopy);
					}
				}
			}
			Individual* toCopy;
			copyIndividual(&toCopy, tempFam->wife);
			insertFront(&children, (void*) toCopy);		
			return children; // return the current list
		} else if (tempFam->husband != NULL && compareIndividuals(tempFam->husband, person) == 0) {
			if (getLength(tempFam->children) > 0) {
				ListIterator childItr = createIterator(tempFam->children);
				void* childElem;
				while ( (childElem = nextElement(&childItr) ) != NULL) {
					Individual* tempChild = (Individual*) childElem;
					if (getLength(tempChild->families) > 1) {
						List grandchildList = initializeList(&printIndividualName, &deleteIndividual, &compareIndividuals);
						grandchildList = getChildren(tempChild->families, tempChild);
						ListIterator grandChildItr = createIterator(grandchildList);
						void* grandchildElem;
						while ( (grandchildElem = nextElement(&grandChildItr)) != NULL) {
							Individual* tempGrandchild = (Individual*) grandchildElem;
							Individual* toAdd;
							copyIndividual(&toAdd, tempGrandchild);
							insertBack(&children, (void*) toAdd);
						}
						clearList(&grandchildList);
					} else {
						Individual* toCopy;
						copyIndividual(&toCopy, tempChild);
						insertBack(&children, (void*) toCopy);
					}
				}
			}
			Individual* toAdd;
			copyIndividual(&toAdd, tempFam->husband);
			insertFront(&children, (void*) toAdd);
			return children;
		}
	}
	// not spouse of any families, so no children, return list of children
	return children;
}

bool customCompareIndividuals(const void* first, const void* second) {

	if (first == NULL || second == NULL) {
		return false;
	}

	Individual* personA = (Individual*) first;
	Individual* personB = (Individual*) second;
	// validate individuals and values must not be null
	if (personA == NULL || personB == NULL 
	|| personA->givenName == NULL || personA->surname == NULL 
	|| personB->givenName == NULL || personB->surname == NULL) {
		return false;
	}
	// Determine size of compare strings
	int lengthA = strlen(personA->givenName) + strlen(personA->surname) + 10;
	int lengthB = strlen(personB->givenName) + strlen(personB->surname) + 10;
	// Allocate new buffers
	char* compareA = malloc(sizeof(char) * lengthA);
	char* compareB = malloc(sizeof(char) * lengthB);
	// concatanate the name strings surname,givenname
	strncpy(compareA, personA->surname, strlen(personA->surname) +1);
	strncat(compareA, ",", strlen(",") +1); 
	strncat(compareA, personA->givenName, strlen(personA->givenName) +1);

	strncpy(compareB, personB->surname, strlen(personB->surname) +1);
	strncat(compareB, ",", strlen(",") +1);
	strncat(compareB, personB->givenName, strlen(personB->givenName) +1);
	// Perform lexographical comparison on givenName + , + surname 
	int result = strcmp(compareA, compareB); 
	// release memory allocated
	free(compareA);
	free(compareB);

	if (result == 0) {
		char* printStr1 = printIndividual((void*) personA);
		char* printStr2 = printIndividual((void*) personB);
		bool result = false;
		if (strcmp(printStr1, printStr2) == 0) {
			result = true;
		}	
		free(printStr1);
		free(printStr2);
		return result;
	}
	return false;
}

bool customCompareName(const void *first, const void *second){
	
	if (first == NULL || second == NULL) {
		return false;
	}
	
	Individual* personA = (Individual*)first;
	Individual* personB = (Individual*)second;

	if (personB->givenName == NULL || personB->surname == NULL) {
		return false;
	}
	if (personA->givenName == NULL || personA->surname == NULL) {
		return false;
	}

	// Determine new size
	int lengthA = strlen(personA->givenName) + strlen(personA->surname) + 2;
	int lengthB = strlen(personB->givenName) + strlen(personB->surname) + 2;

	// Allocate new buffer
	char* compareA = malloc(sizeof(char) * lengthA);
	char* compareB = malloc(sizeof(char) * lengthB);

	// concatanate the field objects
	strcpy(compareA, personA->givenName);
	strcat(compareA, ","); 
	strcat(compareA, personA->surname);

	strcpy(compareB, personB->givenName);
	strcat(compareB, ",");
	strcat(compareB, personB->surname);

	// Perform lexographical comparison on givenName + , + surname 
	int result = strcmp(compareA, compareB); 

	// release memory allocated
	free(compareA);
	free(compareB);

	if (result == 0) {
		return true;
	} else {
		return false;
	}
}

bool alreadyInserted(const List genList, const Individual* toInsert) {
	// check for valid args
	if (getLength(genList) < 1 || toInsert == NULL) {
		return false;
	}
	// loop through lists of lists
	ListIterator genListItr = createIterator(genList);
	void* genListElem;
	while( (genListElem = nextElement(&genListItr) ) != NULL) {
		List* indList = (List*) genListElem;
		ListIterator tempItr = createIterator(*indList);
		void* tempElem;
		while( (tempElem = nextElement(&tempItr) ) != NULL) {
			Individual* tempInd = (Individual*) tempElem;
			if (tempInd->givenName != NULL && toInsert->givenName != NULL) {
				if (customCompareIndividuals((void*) tempInd, (void*) toInsert)) {
					return true;
				}
			}
		}		
	}
	return false;
}

ErrorCode validateFieldList(const List fieldList) {
	// validate field list has been initialized
	if (getLength(fieldList) < 0) {
		return OK;
	}
	if (getLength(fieldList) > 0) {
		ListIterator fieldItr = createIterator(fieldList);
		void* fieldElem;
		while( (fieldElem = nextElement(&fieldItr) ) != NULL) {
			Field* tempField = (Field*) fieldElem;
			// validate is list containing field objects
			if (tempField == NULL) {
				return INV_RECORD;
			}
			// validate field members
			if (tempField->tag == NULL || tempField->value == NULL) {
				return INV_RECORD;
			}
			// validate field value length
			if (strcmp(tempField->tag, "") == 0 || strlen(tempField->value) > 200) {
				return INV_RECORD;
			}
		}
	}
	return OK;
}

ErrorCode validateEventList(const List eventList) {
	// validate event list has been initialized
	if (getLength(eventList) < 0) {
		return OK;
	}
	ErrorCode eventErr = OK;
	if (getLength(eventList) > 0) {
		ListIterator eventItr = createIterator(eventList);
		void* eventElem;
		while( (eventElem = nextElement(&eventItr) ) != NULL) {
			Event* tempEvent = (Event*) eventElem;
			// validate is list containing Event objects
			if (tempEvent == NULL) {
				eventErr = INV_RECORD;
				return eventErr;
			}
			// validate event members
			if (strlen(tempEvent->type) < 2 || tempEvent->date == NULL || tempEvent->place == NULL) {
				eventErr = INV_RECORD;
			}
			// validate event field list
			eventErr = validateFieldList(tempEvent->otherFields);
			if (eventErr != OK) {
				return eventErr;
			}
		}
	}
	return eventErr;
}

void copyIndividual(Individual** destinationPerson, Individual* sourcePerson) {
	// allocate memory for new individual object
	Individual* copyPerson = malloc(sizeof(Individual));
	copyPerson->givenName = malloc(sizeof(char) * strlen(sourcePerson->givenName) +1);
	copyPerson->surname = malloc(sizeof(char) * strlen(sourcePerson->surname) +1);
	//copy values
	strncpy(copyPerson->givenName, sourcePerson->givenName, strlen(sourcePerson->givenName) +1);
	strncpy(copyPerson->surname, sourcePerson->surname, strlen(sourcePerson->surname) +1);
	// copy lists
	copyPerson->otherFields = initializeList(&printField, &deleteWeakReference, &compareFields);	
	if (getLength(sourcePerson->otherFields) > 0) {
		ListIterator listItr = createIterator(sourcePerson->otherFields);
		void* fieldElem; // create iterator for the list of fields
		while ( (fieldElem = nextElement(&listItr)) != NULL) {
			void* copy = fieldElem;
			insertBack(&copyPerson->otherFields, copy);
		}
	}
	copyPerson->events = initializeList(&printEvent, &deleteWeakReference, &compareEvents);	
	if (getLength(sourcePerson->events) > 0) {
		ListIterator listItr = createIterator(sourcePerson->events);
		void* eventElem; // create iterator for the list of fields
		while ( (eventElem = nextElement(&listItr)) != NULL) {
			void* copy = eventElem;
			insertBack(&copyPerson->events, copy);
		}
	}
	copyPerson->families = initializeList(&printFamily, &deleteWeakReference, &compareFamilies);
	if (getLength(sourcePerson->families) > 0) {
		ListIterator listItr = createIterator(sourcePerson->families);
		void* famElem; // create iterator for the list of fields
		while ( (famElem = nextElement(&listItr)) != NULL) {
			void* copy = famElem;
			insertBack(&copyPerson->families, copy);
		}
	}
	*destinationPerson = copyPerson;
}

char* printIndividualName(void* toBePrinted) {
	if (toBePrinted == NULL) {
		return NULL; // pointer must be valid
	}
	const Individual* individualObj = (Individual*) toBePrinted;
	if (individualObj == NULL) {
		return NULL; // pointer must be of type individual
	}	
	char personLbl[] = "Person: "; // labels for Individual obj
	// allocate memory for string
	char* printString = malloc(sizeof(char) * strlen(personLbl) +10);
	strncpy(printString, personLbl, strlen(personLbl) +1);
	// add first name to string
	if (individualObj->givenName != NULL && strcmp(individualObj->givenName, "") != 0) {
		char givenLabel[] = "given: ";
		int newLen = strlen(printString) + strlen(givenLabel) + strlen(individualObj->givenName) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, givenLabel, strlen(givenLabel) +1);
		strncat(printString, individualObj->givenName, strlen(individualObj->givenName) +1);
	} else {
		char noGivenLabel[] = "given name unknown";
		int newLen = strlen(printString) + strlen(noGivenLabel) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, noGivenLabel, strlen(noGivenLabel) +1);
	}
	// add last name to string
	if (individualObj->surname != NULL && strcmp(individualObj->surname, "") != 0) {
		char surLabel[] = ", sur: ";
		int newLen = strlen(printString) + strlen(surLabel) + strlen(individualObj->surname) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, surLabel, strlen(surLabel) +1);
		strncat(printString, individualObj->surname, strlen(individualObj->surname) +1);
	} else {
		char noSurLabel[] = ", surname unknown";
		int newLen = strlen(printString) + strlen(noSurLabel) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, noSurLabel, strlen(noSurLabel) +1);
	}
	return printString;
}

void deleteWeakReference(void* toBeDeleted) {
	if (toBeDeleted == NULL) {
		return; // solution to double free problem
	}
}

void deleteHeader(void* toBeDeleted) {
	if (toBeDeleted == NULL) {
		return;
	}
	Header* headerToDelete = (Header*) toBeDeleted;
	clearList(&headerToDelete->otherFields);
	free(headerToDelete);
}

void deleteSubmitter(void* toBeDeleted) {
	if (toBeDeleted == NULL) {
		return;
	}
	Submitter* submitterToDelete = (Submitter*) toBeDeleted;
	clearList(&submitterToDelete->otherFields);
	free(submitterToDelete);
}

char* printHeader(void* toBePrinted) {
	if (toBePrinted == NULL) {
		return NULL;
	}
	const Header* headerObj = (Header*) toBePrinted;
	if (headerObj == NULL) {
		return NULL; // malloc error
	}
	// labels for header members
	char headerLabel[] = "-----------------------HEADER RECORD-----------------------\n";
	char sourceLabel[] = "source: ";
	char gedcLabel[] = ", GEDC version: ";
	char encodingLabel[] = ", encoding: ";
	// get string representation of encoding charset
	static char* encodingsList[] = {"ANSEL", "UTF8", "UNICODE", "ASCII"};
	char* encodingString = encodingsList[headerObj->encoding];
	// convert gedcVersion to string
	char gedcVersionString[10];
	sprintf(gedcVersionString, "%0.1f", headerObj->gedcVersion);
	// Get length of buffer needed for string
	int length =  strlen(headerLabel) +strlen(sourceLabel) +strlen(gedcLabel) +strlen(encodingLabel) 
				+ strlen(headerObj->source) +strlen(gedcVersionString) +strlen(encodingString) +10;
	// Allocate string with size to hold all required data
	char* printString = malloc(sizeof(char) * length);
	// Build the header string
	strncpy(printString, headerLabel, strlen(headerLabel) +1);
	strncat(printString, sourceLabel, strlen(sourceLabel) +1);
	strncat(printString, headerObj->source, strlen(headerObj->source) +1);
	strncat(printString, gedcLabel, strlen(gedcLabel) +1);
	strncat(printString, gedcVersionString, strlen(gedcVersionString) +1);
	strncat(printString, encodingLabel, strlen(encodingLabel) +1);
	strncat(printString, encodingString, strlen(encodingString) +1);
	// add other fields if not empty
	if (getLength(headerObj->otherFields) > 0) {
		char* otherData = toString(headerObj->otherFields);
		if (otherData != NULL && strcmp(otherData, "") != 0) {
			char otherLabel[] = "\nother header fields: ";
			int newLen = strlen(printString) + strlen(otherLabel) + strlen(otherData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, otherLabel, strlen(otherLabel) +1);
			strncat(printString, otherData, strlen(otherData) +1);
		}
		free(otherData);
	}
	return printString;
}

char* printSubmitter(void* toBePrinted) {
	if (toBePrinted == NULL) {
		return NULL;
	}
	const Submitter* submitterObj = (Submitter*)toBePrinted;
	if (submitterObj == NULL) {
		return NULL;
	}
	// submitter labels
	char submitterLabel[] = "---------------------SUBMITTER RECORD----------------------\n";
	char nameLabel[] = "submitters name: ";
	// get length and allocate subm string
	int length = strlen(submitterLabel) + strlen(nameLabel) + strlen(submitterObj->submitterName) +10;
	char* printString = malloc(sizeof(char) * length);
	// build string
	strncpy(printString, submitterLabel, strlen(submitterLabel) +1);
	strncat(printString, nameLabel, strlen(nameLabel) +1);
	strncat(printString, submitterObj->submitterName, strlen(submitterObj->submitterName) +1);
	// add address if not empty 
	if (submitterObj->address != NULL && strcmp(submitterObj->address, "") != 0 ) {
		char addressLabel[] = ", address: ";
		int newLen = strlen(printString) + strlen(addressLabel) + strlen(submitterObj->address) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, addressLabel, strlen(addressLabel) +1);
		strncat(printString, submitterObj->address, strlen(submitterObj->address) +1);
	}
	// add other fields if not empty
	if (getLength(submitterObj->otherFields) > 0) {
		char* otherData = toString(submitterObj->otherFields);
		if (otherData != NULL && strcmp(otherData, "") != 0) {
			char otherLabel[] = "\nother fields: ";
			int newLen = strlen(printString) + strlen(otherLabel) + strlen(otherData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, otherLabel, strlen(otherLabel) +1);
			strncat(printString, otherData, strlen(otherData) +1);
		}
		free(otherData);
	}
	return printString;
}

char* personToJSON(const Individual* ind) {
	char* jsonStr = malloc(sizeof(char) * strlen("") +1);
	strncpy(jsonStr, "", strlen("") +1);
	if (ind == NULL) {
		return jsonStr;
	} else {
		free(jsonStr);
		jsonStr = NULL;
	}

	char givenLbl[] = "{\"givenName\":\"";
	char surnameLbl[] = "\",\"surname\":\"";
	char sexLbl[] = "\",\"sex\":\"";
	char famSizeLbl[] = "\",\"famSize\":\"";
	char endLbl[] = "\"}";

	int length = strlen(givenLbl) +strlen(surnameLbl) +strlen(sexLbl) 
				+strlen(famSizeLbl) +strlen(endLbl) +50;

	if (ind->givenName != NULL) {
		length += strlen(ind->givenName) +1;
	}
	if (ind->surname != NULL) {
		length += strlen(ind->surname) +1;
	}

	jsonStr = malloc(sizeof(char) * length);

	strncpy(jsonStr, givenLbl, strlen(givenLbl) +1);
	if (ind->givenName != NULL) {
		strncat(jsonStr, ind->givenName, strlen(ind->givenName) +1);
	}
	strncat(jsonStr, surnameLbl, strlen(surnameLbl) +1);
	if (ind->surname != NULL) {
		strncat(jsonStr, ind->surname, strlen(ind->surname) +1);
	}

	strncat(jsonStr, sexLbl, strlen(sexLbl) +1);
	bool hasSexField = false;
	if (getLength(ind->otherFields) > 0) {		
		ListIterator fieldItr = createIterator(ind->otherFields);
		void* fieldElem;
		while (( fieldElem = nextElement(&fieldItr)) != NULL) {
			Field* fieldObj = (Field*) fieldElem;
			if (strcmp(fieldObj->tag, "SEX") == 0) {
				hasSexField = true;
				strncat(jsonStr, fieldObj->value, strlen(fieldObj->value) +1);
			}
		}
	}
	if (hasSexField == false) {
		strncat(jsonStr, "U", strlen("U") +1);
	}

	char numFam[2];
	strncat(jsonStr, famSizeLbl, strlen(famSizeLbl) +1);
	if (getLength(ind->families) > 0) {
		ListIterator famItr = createIterator(ind->families);
		void* elem;
		bool found = false;
		while((elem = nextElement(&famItr)) != NULL) {
			Family* famObj = (Family*) elem;
			if ((famObj->wife != NULL && compareIndividuals(famObj->wife, ind) == 0) 
			|| (famObj->husband != NULL && compareIndividuals(famObj->husband, ind) == 0)) {
				if (getLength(famObj->children) > 0) {
					sprintf(numFam, "%d", getLength(famObj->children));
					found = true;
					break;
				}				
			}
		}
		if (found == false) {
			sprintf(numFam, "%d", 0);	
		}
	} else {
		sprintf(numFam, "%d", 0);
	}
	strncat(jsonStr, numFam, strlen(numFam) +1);

	strncat(jsonStr, endLbl, strlen(endLbl) +1);
	return jsonStr;
}

char* GEDCOMtoJSON(char* fileName, GEDCOMobject* gedcom) {

	char* jsonStr = malloc(sizeof(char) * strlen("") +1);
	strncpy(jsonStr, "", strlen("") +1);
	
	if (gedcom == NULL) {
		return jsonStr;
	} else {
		free(jsonStr);
		jsonStr = NULL;
	}
	// create json keys	
	char fileNameLbl[] = "{\"fileName\":\"";
	char sourceLbl[] = "\",\"source\":\"";
	char gedcLbl[] = "\",\"gedcVersion\":\"";
	char encodingLbl[] = "\",\"encoding\":\"";
	char subNameLbl[] = "\",\"subName\":\"";
	char subAddrLbl[] = "\",\"subAddr\":\"";
	char numIndLbl[] = "\",\"numInd\":\"";
	char numFamLbl[] = "\",\"numFam\":\"";
	char endLbl[] = "\"}";

	int length = strlen(fileNameLbl) +strlen(sourceLbl) +strlen(gedcLbl) 
				+strlen(encodingLbl) +strlen(subNameLbl) +strlen(subAddrLbl)
				+strlen(numIndLbl) +strlen(numFamLbl) +strlen(endLbl) +100;

	// alllocate json string
	jsonStr = malloc(sizeof(char) * length);

	strncpy(jsonStr, fileNameLbl, strlen(fileNameLbl) +1);
	strncat(jsonStr, fileName, strlen(fileName) +1);

	strncat(jsonStr, sourceLbl, strlen(sourceLbl) +1);
	strncat(jsonStr, gedcom->header->source, strlen(gedcom->header->source) +1);

	strncat(jsonStr, gedcLbl, strlen(gedcLbl) +1);
	char gedcNum[5];
	sprintf(gedcNum, "%0.1f", gedcom->header->gedcVersion);
	strncat(jsonStr, gedcNum, strlen(gedcNum) +1);

	// get string representation of encoding charset
	static char* encodingsList[] = {"ANSEL", "UTF8", "UNICODE", "ASCII"};
	char* encStr = encodingsList[gedcom->header->encoding];

	strncat(jsonStr, encodingLbl, strlen(encodingLbl) +1);
	strncat(jsonStr, encStr, strlen(encStr) +1);

	strncat(jsonStr, subNameLbl, strlen(subNameLbl) +1);
	strncat(jsonStr, gedcom->submitter->submitterName, strlen(gedcom->submitter->submitterName) +1);

	strncat(jsonStr, subAddrLbl, strlen(subAddrLbl) +1);
	strncat(jsonStr, gedcom->submitter->address, strlen(gedcom->submitter->address) +1);

	strncat(jsonStr, numIndLbl, strlen(numIndLbl) +1);
	char numInd[3];
	if (getLength(gedcom->individuals) > 0) {
		sprintf(numInd, "%d", getLength(gedcom->individuals));
	} else {
		sprintf(numInd, "%d", 0);
	}
	strncat(jsonStr, numInd, strlen(numInd) +1);

	strncat(jsonStr, numFamLbl, strlen(numFamLbl) +1);
	char numFam[3];
	if (getLength(gedcom->families) > 0) {
		sprintf(numFam, "%d", getLength(gedcom->families));
	} else {
		sprintf(numFam, "%d", 0);
	}
	strncat(jsonStr, numFam, strlen(numFam) +1);

	strncat(jsonStr, endLbl, strlen(endLbl) +1);
	return jsonStr;
}

void saveGEDCOM(char* fileName, char* jsonGEDCOM) {

	if (fileName == NULL || jsonGEDCOM == NULL) {
		return;
	}
	if (strcmp(fileName, "") == 0 || strcmp(jsonGEDCOM, "") == 0 || strcmp(jsonGEDCOM, "{}") == 0) {
		return;
	}

	GEDCOMobject* newGEDCOM = JSONtoGEDCOM(jsonGEDCOM);
	if (newGEDCOM == NULL) {
		return;
	}
	
	ErrorCode validGED = validateGEDCOM(newGEDCOM);
	if (validGED != OK) {
		deleteGEDCOM(newGEDCOM);
		return;
	}
	writeGEDCOM(fileName, newGEDCOM);
	
	if (newGEDCOM != NULL) {
		deleteGEDCOM(newGEDCOM);
	}
	return;
}

bool saveIndividual(char* fileName, char* jsonInd) {

	if (fileName == NULL || jsonInd == NULL) {
		return false;
	}

	Individual* newInd = JSONtoInd(jsonInd);
	if (newInd == NULL) {
		return false;
	}

	GEDCOMobject* gedcomObj = NULL;
	GEDCOMerror createErr = createGEDCOM(fileName, &gedcomObj);
	if (createErr.type != OK) {
		return false;
	}
	
	addIndividual(gedcomObj, newInd);
	
	GEDCOMerror writeErr = writeGEDCOM(fileName, gedcomObj);
	if (gedcomObj != NULL) {
		deleteGEDCOM(gedcomObj);
	}

	if (writeErr.type == OK) {
		return true;
	} else {
		return false;
	}
}

char* getGenerations(char* fileName, char* jsonInd, bool getDesc, int numGen) {

	char* jsonStr = malloc(sizeof(char) * strlen("{}") +1);
	strncpy(jsonStr, "{}", strlen("{}") +1);
	if (fileName == NULL || jsonInd == NULL) {
		return jsonStr;
	}
	if (strcmp(fileName, "") == 0 || strcmp(jsonInd, "") == 0 ) {
		return jsonStr;
	}

	Individual* refInd = JSONtoInd(jsonInd);
	if (refInd == NULL) {
		return jsonStr;
	}

	GEDCOMobject* gedcomObj = NULL;
	GEDCOMerror createErr = createGEDCOM(fileName, &gedcomObj);
	if (createErr.type != OK) {
		if (gedcomObj != NULL) {
			deleteGEDCOM(gedcomObj);
		}
		if (refInd != NULL) {
			deleteIndividual((void*) refInd);
		}
		return jsonStr;
	}
	if (getLength(gedcomObj->individuals) < 1 || getLength(gedcomObj->families) < 1) {
		if (gedcomObj != NULL) {	
			deleteGEDCOM(gedcomObj);
		}
		if (refInd != NULL) {
			deleteIndividual((void*) refInd);
		}
		return jsonStr;
	}

	Individual* searchPerson = findPerson(gedcomObj, &customCompareName, refInd);
	deleteIndividual((void*) refInd);

	if (searchPerson == NULL) {
		if (gedcomObj != NULL) {
			deleteGEDCOM(gedcomObj);
		}
		return jsonStr;
	}

	List generations;
	if (getDesc == true) {
		generations = getDescendantListN(gedcomObj, searchPerson, numGen);
	} else {
		generations = getAncestorListN(gedcomObj, searchPerson, numGen);
	}

	if (getLength(generations) > 0) {
		free(jsonStr);
		jsonStr = gListToJSON(generations);
		clearList(&generations);
		deleteGEDCOM(gedcomObj);
	}
	return jsonStr;
}

char* getFile(char* fileName) {
	
	char* jsonStr = malloc(sizeof(char) * strlen("{}") +1);
	strncpy(jsonStr, "{}", strlen("{}") +1);
	if (fileName == NULL || strcmp(fileName, "") == 0) {
		return jsonStr;
	}

	GEDCOMobject* gedcomObj = NULL;
	GEDCOMerror createErr = createGEDCOM(fileName, &gedcomObj);
	if (createErr.type == OK) {
		free(jsonStr);
		jsonStr = NULL;
		jsonStr = GEDCOMtoJSON(fileName, gedcomObj);
		deleteGEDCOM(gedcomObj);
	}
	return jsonStr;
}

char* getIndividuals(char* fileName) {

	char* jsonFamStr = malloc(sizeof(char) * strlen("{}") +1);
	strncpy(jsonFamStr, "{}", strlen("{}") +1);
	if (fileName == NULL) {
		return jsonFamStr;
	}

	GEDCOMobject* gedcomObj = NULL;
	GEDCOMerror createErr = createGEDCOM(fileName, &gedcomObj);

	if (createErr.type == OK && getLength(gedcomObj->individuals) > 0) {
		
		free(jsonFamStr);
		jsonFamStr = NULL;

		ListIterator indItr = createIterator(gedcomObj->individuals);
		void* indElem;
		while ( ( indElem = nextElement(&indItr)) != NULL ) {
			Individual* indObj = (Individual*) indElem;
			char* jsonIndStr = NULL;
			if (createErr.type == OK) {
				jsonIndStr = personToJSON(indObj);
			}
			if (jsonIndStr != NULL) {
				if (jsonFamStr == NULL) {
					jsonFamStr = malloc(sizeof(char) * (strlen(jsonIndStr) +10));
					strncpy(jsonFamStr, "[", strlen("[") +1);
					strncat(jsonFamStr, jsonIndStr, strlen(jsonIndStr) +1);
				} else {
					jsonFamStr = realloc(jsonFamStr, sizeof(char)*(strlen(jsonFamStr)+strlen(jsonIndStr)+10));
					strncat(jsonFamStr, ",", strlen(",") +1);
					strncat(jsonFamStr, jsonIndStr, strlen(jsonIndStr) +1);
				}
				free(jsonIndStr);
			}
		}
	} else {
		return jsonFamStr; 
	}
	
	strncat(jsonFamStr, "]", strlen("]") +1);
	deleteGEDCOM(gedcomObj);
	return jsonFamStr;
}

/* Collection of functions for the GEDCOMwriter header */

char* writeXREF(int index, char type) {
	// create parts of key
	char tagDelim[] = "@";
	char indexStr[3];
	char typeStr[2];
	sprintf(indexStr, "%d", index);
	sprintf(typeStr, "%c", type);
	// allocate len of string
	int xrefLen = (2* strlen(tagDelim)) +strlen(typeStr) +strlen(indexStr) +1;
	char* xrefStr = malloc(sizeof(char) * xrefLen);
	// build string
	strncpy(xrefStr, tagDelim, strlen(tagDelim) +1);
	strncat(xrefStr, typeStr, strlen(typeStr) +1);
	strncat(xrefStr, indexStr, strlen(indexStr) +1);
	strncat(xrefStr, tagDelim, strlen(tagDelim) +1);
	// return malloced strings
	return xrefStr;
}

char* writeName(const Individual* person) {
	if (person == NULL) {
		return NULL;
	}
	// name tags for gedcom record
	char nameLbl[] = "1 NAME ";
	char givenLbl[] = "2 GIVN ";
	char surLbl[] = "2 SURN ";
	// allocate with length of string
	int length = strlen(nameLbl)+strlen(givenLbl)+strlen(surLbl)+(2*(strlen(person->givenName)+strlen(person->surname)))+10;
	char* nameStr = malloc(sizeof(char) * length);
	// build the string
	strncpy(nameStr, nameLbl, strlen(nameLbl) +1);
	strncat(nameStr, person->givenName, strlen(person->givenName) +1);
	strncat(nameStr, " /", strlen(" /") +1);
	strncat(nameStr, person->surname, strlen(person->surname) +1);
	strncat(nameStr, "/\n", strlen("/\n") +1);
	// add first name
	strncat(nameStr, givenLbl, strlen(givenLbl) +1);
	strncat(nameStr, person->givenName, strlen(person->givenName) +1);
	strncat(nameStr, "\n", strlen("\n") +1);
	// add last name
	strncat(nameStr, surLbl, strlen(surLbl) +1);
	strncat(nameStr, person->surname, strlen(person->surname) +1);
	strncat(nameStr, "\n", strlen("\n") +1);
	// return malloced string
	return nameStr;
}

char* writeContValue(const char* buffer) {
	if (buffer == NULL) {
		return NULL;
	}
	// create copy of the line
	char* line = strdup(buffer);
	if (line == NULL) {
		return NULL;
	}
	int level = atoi(line);
	level++;
	char snum[2];
	sprintf(snum, "%d", level);
	// cont tag label
	char contLbl[] = " CONT ";
	const char lineDelim[] = "\n";

	char* writeStr = NULL;
	char* ret = NULL;
	ret = strstr(line, lineDelim);
	if (ret != NULL) {
		char* key = strtok(line, lineDelim);
		writeStr = malloc(sizeof(char) * (strlen(key) + strlen(contLbl) + strlen(ret)) +10);
		// build new string
		strncpy(writeStr, key, strlen(key) +1);
		strncat(writeStr, lineDelim, strlen(lineDelim) +1);
		strncat(writeStr, snum, strlen(snum) +1);
		strncat(writeStr, contLbl, strlen(contLbl) +1);
		strncat(writeStr, ret, strlen(ret) +1);
		strncat(writeStr, lineDelim, strlen(lineDelim) +1);
		free(line);
		return writeStr;
	} else {
		free(line);
		return NULL;
	}
}

char* writeFields(const List fieldsList) {
	if (getLength(fieldsList) < 1) {
		return NULL;
	}

	char levelLbl[] = "1 ";
	char spaceDelim[] = " ";
	char lineDelim[] = "\n";

	int length = strlen(levelLbl) +strlen(spaceDelim) +strlen(lineDelim) +(5* sizeof(char)) +1;
	char* writeTemplate = malloc(sizeof(char) * length);
	strncpy(writeTemplate, levelLbl, strlen(levelLbl) +1);

	char* writeList = NULL;
	// loop list of fields and add each field to write list
	ListIterator fieldItr = createIterator(fieldsList);
	void* fieldElem;
	while( (fieldElem = nextElement(&fieldItr)) != NULL) {
		// cast field string and write values and add to string
		Field* field = (Field*) fieldElem;		
		if (field != NULL && field->tag != NULL && field->value != NULL) {
			char* writeField = malloc(sizeof(char) * (length +strlen(field->tag) +strlen(field->value)) +1);
			strncpy(writeField, writeTemplate, strlen(writeTemplate) +1);
			if (strcmp(field->tag, "") != 0) {
				strncat(writeField, field->tag, strlen(field->tag) +1);
				strncat(writeField, spaceDelim, strlen(spaceDelim) +1);
				strncat(writeField, field->value, strlen(field->value) +1);
				strncat(writeField, lineDelim, strlen(lineDelim)+1);
				if (writeList == NULL) {
					writeList = malloc(sizeof(char) * strlen(writeField) +1);
					strncpy(writeList, writeField, strlen(writeField) +1);
				} else {
					writeList = realloc(writeList, sizeof(char) * (strlen(writeField) + strlen(writeList)) +1);
					strncat(writeList, writeField, strlen(writeField) +1);
				}
			}
			free(writeField);
		}
	}
	free(writeTemplate);
	return writeList;
}

char* writeEvents(const List eventList) {
	if (getLength(eventList) < 1) {
		return NULL;
	}
	// tags for event type
	char eventLbl[] = "1 ";
	char dateLbl[] = "\n2 DATE ";
	char placeLbl[] = "\n2 PLAC ";

	int length = strlen(eventLbl)+(5*sizeof(char))+strlen(dateLbl)+strlen(placeLbl)+1;
	char* writeTemplate = malloc(sizeof(char) * length);
	strncpy(writeTemplate, eventLbl, strlen(eventLbl) +1);

	char* writeList = NULL;
		
	ListIterator eventItr = createIterator(eventList);
	void* eventElem;
	while( (eventElem = nextElement(&eventItr)) != NULL) {
		Event* event = (Event*) eventElem;
		char* writeEvent = malloc(sizeof(char) * length);
		strncpy(writeEvent, writeTemplate, strlen(writeTemplate) +1);
		strncat(writeEvent, event->type, strlen(event->type) +1);
		if (event->date != NULL && strcmp(event->date, "") != 0) {
			writeEvent = realloc(writeEvent, sizeof(char) * (length + strlen(event->date)) +10);
			strncat(writeEvent, dateLbl, strlen(dateLbl) +1);
			strncat(writeEvent, event->date, strlen(event->date) +1);
		}
		if (event->place != NULL && strcmp(event->place, "") != 0) {
			char* place = writeContValue(event->place);
			if (place != NULL) {
				writeEvent = realloc(writeEvent, sizeof(char) * (length +strlen(placeLbl) +strlen(place)) +10);
				strncat(writeEvent, placeLbl, strlen(placeLbl) +1);
				strncat(writeEvent, place, strlen(place) +1);
			} else {
				writeEvent = realloc(writeEvent, sizeof(char) * (length +strlen(placeLbl) +strlen(event->place)) +10);
				strncat(writeEvent, placeLbl, strlen(placeLbl) +1);
				strncat(writeEvent, event->place, strlen(event->place) +1);
			}
			free(place);
		}
		strncat(writeEvent, "\n", strlen("\n"));
		if (writeList == NULL) {
			writeList = malloc(sizeof(char) * strlen(writeEvent) +1);
			strncpy(writeList, writeEvent, strlen(writeEvent) +1);
		} else {
			writeList = realloc(writeList, sizeof(char) * (strlen(writeEvent) + strlen(writeList)) +1);
			strncat(writeList, writeEvent, strlen(writeEvent) +1);
		}
		free(writeEvent);
	}
	free(writeTemplate);
	return writeList;
}

char* writeHeader(const void* headerObj) {
	if (headerObj == NULL) {
		return NULL;
	}
	Header* header = (Header*) headerObj;
	if (header == NULL || strlen(header->source) < 1 || header->submitter == NULL) {
		return NULL;
	}
	// convert gedcVersion to string
	char gedcStr[5];
	sprintf(gedcStr, "%0.1f", header->gedcVersion);
	// get string representation of encoding charset
	static char* encodingsList[] = {"ANSEL", "UTF8", "UNICODE", "ASCII"};
	char* charStr = encodingsList[header->encoding];
	// validate encoding and submitter XREF
	if (charStr == NULL) {
		return NULL;
	}
	// tags for header recrd
	char headSrcLbl[] = "0 HEAD\n1 SOUR ";
	char gedcLbl[] = "\n1 GEDC\n2 VERS ";
	char formCharLbl[] = "\n2 FORM LINEAGE-LINKED\n1 CHAR ";
	char submLbl[] = "\n1 SUBM @U1@\n";
	// create and allocate header string
	int length = strlen(headSrcLbl) +strlen(gedcLbl) +strlen(formCharLbl)
				+strlen(submLbl) +strlen(gedcStr) +strlen(header->source) +10;
	char* writeHeader = malloc(sizeof(char) * length);
	if (writeHeader == NULL) {
		return NULL;
	}
	// build header string
	strncpy(writeHeader, headSrcLbl, strlen(headSrcLbl) +1);
	strncat(writeHeader, header->source, strlen(header->source) +1);
	// add gedc tag
	strncat(writeHeader, gedcLbl, strlen(gedcLbl) +1);
	strncat(writeHeader, gedcStr, strlen(gedcStr) +1);
	// add form and char tag
	strncat(writeHeader, formCharLbl, strlen(formCharLbl) +1);
	strncat(writeHeader, charStr, strlen(charStr) +1);
	// add subm ref
	strncat(writeHeader, submLbl, strlen(submLbl) +1);
	// return malloced string
	return writeHeader;
}

char* writeSubmitter(const void* submitterObj) {
	if (submitterObj == NULL) {
		return NULL;
	}
	// cast to submitter
	Submitter* submitter = (Submitter*) submitterObj;
	if (submitter == NULL || submitter->submitterName == NULL) {
		return NULL;
	}
	// submitter tag
	char submLbl[] = "0 @U1@ SUBM\n1 NAME ";
	// get length and allocate string
	int length = strlen(submLbl) +strlen(submitter->submitterName) +10;
	char* writeSubmitter = malloc(sizeof(char) * length);
	// build string
	strncpy(writeSubmitter, submLbl, strlen(submLbl) +1);
	strncat(writeSubmitter, submitter->submitterName, strlen(submitter->submitterName) +1);
	if (submitter->address != NULL && strcmp(submitter->address, "") != 0) {
		char addrLbl[] = "\n1 ADDR ";
		int newLen = length +strlen(addrLbl) + strlen(submitter->address) +1;
		writeSubmitter = realloc(writeSubmitter, sizeof(char) * newLen);
		strncat(writeSubmitter, addrLbl, strlen(addrLbl) +1);
		strncat(writeSubmitter, submitter->address, strlen(submitter->address) +1);
	}
	strncat(writeSubmitter, "\n", strlen("\n") +1);
	return writeSubmitter;
}

char* writeIndividuals(const List personList, HashTable* hashMap) {
	if (getLength(personList) < 1 || hashMap == NULL) {
		return NULL;
	}
	char* writeList = NULL;
		
	ListIterator personItr = createIterator(personList);
	void* personElem;
	while( (personElem = nextElement(&personItr)) != NULL) {
		// cast to individual
		Individual* person = (Individual*) personElem;
		char* writePerson = NULL;
		// get xref of indi from map
		char* xref = getHtKey(hashMap, personElem);
		if (xref == NULL) {
			free(writeList);
			return NULL;
		}
		char* key = malloc(sizeof(char) * (strlen(xref) +strlen("0 ") +strlen(" INDI\n") ) +2);
		strncpy(key, "0 ", strlen("0 ") +1);
		strncat(key, xref, strlen(xref) +1);
		strncat(key, " INDI\n", strlen(" INDI\n") +1);
		char* name = writeName(person);
		// add tags
		if (xref != NULL && name != NULL) {
			writePerson = malloc(sizeof(char) * (strlen(key) + strlen(name)) +1);
			strncpy(writePerson, key, strlen(key) +1);
			strncat(writePerson, name, strlen(name) +1);
			free(key);
			if (getLength(person->otherFields) > 0) {
				char* fields = writeFields(person->otherFields);
				if (fields != NULL) {
					writePerson = realloc(writePerson, sizeof(char) * (strlen(fields) + strlen(writePerson)) +1);
					strncat(writePerson, fields, strlen(fields) +1);
				}
				free(fields);
			}			
			// add event tags if not empty
			if (getLength(person->events) > 0) {
				char* events = writeEvents(person->events);
				if (events != NULL) {
					writePerson = realloc(writePerson, sizeof(char) * (strlen(events) + strlen(writePerson)) +1);
					strncat(writePerson, events, strlen(events) +1);
				}
				free(events);
			}
			if (getLength(person->families) > 0) {
				ListIterator familyItr = createIterator(person->families);
				void* familyElem;
				while( (familyElem = nextElement(&familyItr)) != NULL) {
					Family* family = (Family*) familyElem;
					if (family->husband != NULL && family->wife != NULL) {
						if (compareIndividuals(family->husband, person) == 0 || compareIndividuals(family->wife, person) == 0) {
							char* spouseXREF = getHtKey(hashMap, family);
							char* spouseKey = malloc(sizeof(char) * (strlen(spouseXREF) + strlen("1 FAMS ") + strlen(" \n")) +2);
							strncpy(spouseKey, "1 FAMS ", strlen("1 FAMS ") +1);
							strncat(spouseKey, spouseXREF, strlen(spouseXREF) +1);
							strncat(spouseKey, " \n", strlen(" \n") +1);
							writePerson = realloc(writePerson, sizeof(char) * (strlen(spouseKey) + strlen(writePerson)) +1);
							strncat(writePerson, spouseKey, strlen(spouseKey) +1);
							free(spouseKey);
						} else {
							char* childXREF = getHtKey(hashMap, family);
							char* childKey = malloc(sizeof(char) * (strlen(childXREF) + strlen("1 FAMC ") + strlen(" \n")) +2);
							strncpy(childKey, "1 FAMC ", strlen("1 FAMC ") +1);
							strncat(childKey, childXREF, strlen(childXREF) +1);
							strncat(childKey, " \n", strlen(" \n") +1);
							writePerson = realloc(writePerson, sizeof(char) * (strlen(childKey) + strlen(writePerson)) +1);
							strncat(writePerson, childKey, strlen(childKey) +1);
							free(childKey);
						}
					}
				}
			}
			if (writeList == NULL) {
				writeList = malloc(sizeof(char) * strlen(writePerson) +1);
				strncpy(writeList, writePerson, strlen(writePerson) +1);
			} else {
				writeList = realloc(writeList, sizeof(char) * (strlen(writePerson) + strlen(writeList)) +1);
				strncat(writeList, writePerson, strlen(writePerson) +1);
			}
			free(name);
			free(writePerson);
		}
	}
	return writeList;
}

char* writeFamilys(const List familyList, HashTable* hashMap) {
	if (getLength(familyList) < 1 || hashMap == NULL) {
		return NULL;
	}
	char* writeList = NULL;
	// loop through list of families
	ListIterator familyItr = createIterator(familyList);
	void* familyElem;
	while( (familyElem = nextElement(&familyItr)) != NULL) {
		// cast void pointer to family object
		Family* family = (Family*) familyElem;

		char* famXREF = getHtKey(hashMap, familyElem);
		char* famKey = malloc(sizeof(char) * (strlen(famXREF) + strlen("FAM") + strlen("0 \n")) +10);
		strncpy(famKey, "0 ", strlen("0 ") +1);
		strncat(famKey, famXREF, strlen(famXREF) +1);
		strncat(famKey, " FAM\n", strlen(" FAM\n") +1);
		if (famXREF != NULL) {
			char* writeFamily = malloc(sizeof(char) * strlen(famKey) +1);
			strncpy(writeFamily, famKey, strlen(famKey) +1);
			free(famKey);	
			if (family->husband != NULL) {
				char* husbandXREF = getHtKey(hashMap, family->husband);
				char* husbandKey = malloc(sizeof(char) * (strlen(husbandXREF) + strlen("1 HUSB") + strlen("\n")) +10);
				strncpy(husbandKey, "1 HUSB ", strlen("1 HUSB ") +1);
				strncat(husbandKey, husbandXREF, strlen(husbandXREF) +1);
				strncat(husbandKey, "\n", strlen("\n") +1);
				writeFamily = realloc(writeFamily, sizeof(char) * (strlen(husbandKey) + strlen(writeFamily)) +1);
				strncat(writeFamily, husbandKey, strlen(husbandKey) +1);
				free(husbandKey);
			}
			if (family->wife != NULL) {
				char* wifeXREF = getHtKey(hashMap, family->wife);
				char* wifeKey = malloc(sizeof(char) * (strlen(wifeXREF) + strlen("1 WIFE") + strlen("\n")) +10);
				strncpy(wifeKey, "1 WIFE ", strlen("1 WIFE ") +1);
				strncat(wifeKey, wifeXREF, strlen(wifeXREF) +1);
				strncat(wifeKey, "\n", strlen("\n") +1);
				writeFamily = realloc(writeFamily, sizeof(char) * (strlen(wifeKey) + strlen(writeFamily)) +1);
				strncat(writeFamily, wifeKey, strlen(wifeKey) +1);
				free(wifeKey);
			}
			if (getLength(family->events) > 0) {
				char* events = writeEvents(family->events);
				if (events != NULL) {
					writeFamily = realloc(writeFamily, sizeof(char) * (strlen(events) + strlen(writeFamily)) +1);
					strncat(writeFamily, events, strlen(events) +1);
				}
				free(events);
			}
			if (getLength(family->children) > 0) {
				ListIterator childItr = createIterator(family->children);
				void* childElem;
				int i = 0;
				while( (childElem = nextElement(&childItr)) != NULL) {
					char snum[2];
					sprintf(snum, "%d", i);
					char* childXREF = getHtKey(hashMap, childElem);
					char* childKey = malloc(sizeof(char) * (strlen(childXREF) + strlen("CHIL") + strlen(snum) +strlen("\n")) +10);
					strncpy(childKey, "1 CHIL ", strlen("1 CHIL ") +1);
					strncat(childKey, childXREF, strlen(childXREF) +1);
					strncat(childKey, "\n", strlen("\n") +1);
					writeFamily = realloc(writeFamily, sizeof(char) * (strlen(childKey) + strlen(writeFamily)) +1);
					strncat(writeFamily, childKey, strlen(childKey) +1);
					free(childKey);
					i++;
				}
			}
			if (writeList == NULL) {
				writeList = malloc(sizeof(char) * strlen(writeFamily) +1);
				strncpy(writeList, writeFamily, strlen(writeFamily) +1);
			} else {
				writeList = realloc(writeList, sizeof(char) * (strlen(writeFamily) + strlen(writeList)) +1);
				strncat(writeList, writeFamily, strlen(writeFamily) +1);
			}
			free(writeFamily);
		}		
	}
	return writeList;
}