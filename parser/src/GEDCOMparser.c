#define _GNU_SOURCE

#include "GEDCOMparser.h"
#include "HashMap.h"
#include "GEDCOMutilities.h"
#include "CREATEutilities.h"
#include "DATATYPEutilities.h"
#include "GEDCOMwriter.h"

void deleteField(void* toBeDeleted) {
	if (toBeDeleted == NULL) {
		return;
	}
	Field* fieldToDelete = (Field*) toBeDeleted;
	free(fieldToDelete->tag);
	free(fieldToDelete->value);
	free(fieldToDelete);
}

int compareFields(const void* first,const void* second) {
	if (first == NULL || second == NULL) {
		return 2; // return 2 for invalid input
	}
	// cast const void pointers to field objects
	Field* fieldA = (Field*) first;
	Field* fieldB = (Field*) second;
	if (fieldA == NULL || fieldB == NULL 
		|| fieldA->tag == NULL || fieldB->tag == NULL 
		|| fieldA->value == NULL || fieldB->value == NULL) {
		return 2; // return 2 for invalid input
	}
	// Determine size of compare strings, add extra length for space in string
	int lengthA = strlen((char*)fieldA->tag) + strlen((char*)fieldA->value) +2;
	int lengthB = strlen((char*)fieldB->tag) + strlen((char*)fieldB->value) +2;
	// Allocate new buffer
	char* compareA = malloc(sizeof(char) * lengthA);
	char* compareB = malloc(sizeof(char) * lengthB);
	// concatanate the field objects, format: tag [SPACE] value
	strcpy(compareA, (char*)fieldA->tag);
	strcat(compareA, " "); 
	strcat(compareA, (char*)fieldA->value);
	// string B
	strcpy(compareB, (char*)fieldB->tag);
	strcat(compareB, " ");
	strcat(compareB, (char*)fieldB->value);
	// Perform a lexographical comparasion on concatanated tag and valie fields 
	int result = strcmp(compareA, compareB);
	// release memory allocated for string comparison
	free(compareA);
	free(compareB);
	return result;
}

char* printField(void* toBePrinted) {
	if (toBePrinted == NULL) {
		return NULL;
	}
	// validate field object
	const Field* fieldObj = (Field*) toBePrinted;
	if (fieldObj == NULL || fieldObj->tag == NULL || fieldObj->value == NULL) {
		return NULL;
	}
	// field labels for string readability
	char fieldLabel[] = "Field Record: "; 
	char tagLabel[] = "tag: ";
	char valueLabel[] = ", value: ";
	// Determine array length
	int length = strlen(fieldLabel)+strlen(tagLabel)+strlen(valueLabel)+strlen(fieldObj->tag)+strlen(fieldObj->value)+1;
	// Allocate memory for string
	char* printString = malloc(sizeof(char) * length);
	if (printString == NULL) {
		return NULL;
	}
	// Build the field string
	strcpy(printString, fieldLabel);
	strcat(printString, tagLabel);
	strcat(printString, fieldObj->tag); 
	strcat(printString, valueLabel);
	strcat(printString, fieldObj->value);
	// return print field string 
	return printString;
}

void deleteEvent(void* toBeDeleted) {
	if (toBeDeleted == NULL) {
		return;
	}
	Event* eventToDelete = (Event*) toBeDeleted;
	free(eventToDelete->date);
	free(eventToDelete->place);
	clearList(&eventToDelete->otherFields);
	free(eventToDelete);
}

int compareEvents(const void* first,const void* second) {
	if (first == NULL || second == NULL) {
		return 2; // return 2 for invalid input
	}
	// cast event objects and validate date members
	Event* eventA = (Event*) first;
	Event* eventB = (Event*) second;
	if (eventA == NULL || eventB == NULL || eventA->date == NULL || eventB->date == NULL) {
		return 2;
	}
	// Cannot compare by date, comparing type lexographically
	if (strcmp(eventA->date, "") == 0 || strcmp(eventB->date, "") == 0 || strlen(eventA->date) != 11 || strlen(eventB->date) != 11) {
		// Perform a lexographical comparasion on concatanated tag and valie fields 
		return strcmp(eventA->type, eventB->type);
	} else {
		// Date format: DD MMM YYYY
		int dayA, dayB, yearA, yearB;
		char monthA[3], monthB[3];
		// get string copys of dates
		char* dateStringA = strdup(eventA->date);
		char* dateStringB = strdup(eventB->date);
		// parse first date
		char* key = strtok(dateStringA, " ");
		if (key != NULL) {
			dayA = atoi(key); // atoi returns 0 on invalid input 
			key = strtok (NULL, " ");
		}
		if (key != NULL) {
			strncpy(monthA, key, 3);
			key = strtok (NULL, "\0");
		}
		if (key != NULL) {
			yearA = atoi(key);
		}		
		// parse second date
		key = strtok(dateStringB, " ");
		if (key != NULL) {
			dayB = atoi(key);
			key = strtok (NULL, " ");
		}
		if (key != NULL) {
			strncpy(monthB, key, 3);
			key = strtok (NULL, "\0");
		}
		if (key != NULL) {
			yearB = atoi(key);
		}
		free(dateStringA);
		free(dateStringB);
		static const char monthList[12][3] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
		// Compare: -1 if first earlier than second, 0 if same, 1 if first later than second
		if (yearA == yearB) {
			int i, monthIndexA = 0, monthIndexB = 0;
			for (i = 0; i < 12; i++) {
				if (strcmp(monthA, monthList[i]) == 0) {
					monthIndexA = i;
				}
				if (strcmp(monthB, monthList[i]) == 0) {
					monthIndexB = i;
				}
			}
			if (monthIndexA == monthIndexB) {
				if (dayA == dayB) {
					return 0;
				} else if (dayA > dayB) {
					return 1;
				} else {
					return -1;
				}
			} else if (monthIndexA > monthIndexB) {
				return 1;
			} else {
				return -1;
			}
		} else if (yearA > yearB) {
			return 1;
		} else {
			return -1;
		}
	}
	return 0;
}

char* printEvent(void* toBePrinted) {
	if (toBePrinted == NULL) {
		return NULL;
	}
	 // validate data: must not be null and valid
	const Event* eventObj = (Event*) toBePrinted;
	if (eventObj == NULL || eventObj->date == NULL || eventObj->place == NULL || strlen(eventObj->type) > 5) {
		return NULL;
	}
	// event labels for readability
	char eventLabel[] = "Event Record:"; 
	char typeLabel[] = " type: ";
	// get length of print event string
	int len = strlen(eventLabel) + strlen(typeLabel) + 5;
	char* printString = malloc(sizeof(char) * len +10);
	// Build print string
	strncpy(printString, eventLabel, strlen(eventLabel) +1);
	strncat(printString, typeLabel, strlen(typeLabel) +1);
	strncat(printString, eventObj->type, strlen(eventObj->type) +1);
	// add date and place fields if they exists
	if (strcmp(eventObj->date, "") != 0) {
		char dateLabel[] = ", date: ";
		int newLen = strlen(printString) + strlen(dateLabel) + strlen(eventObj->date) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, dateLabel, strlen(dateLabel) +1);
		strncat(printString, eventObj->date, strlen(eventObj->date) +1);
	}
	if (strcmp(eventObj->place, "") != 0) {
		char placeLabel[] = ", place: ";
		int newLen = strlen(printString) + strlen(placeLabel) + strlen(eventObj->place) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, placeLabel, strlen(placeLabel) +1);
		strncat(printString, eventObj->place, strlen(eventObj->place) +1);
	}
	// add event fields if they exist
	if (getLength(eventObj->otherFields) > 0) {
		char* infoData = toString(eventObj->otherFields);
		if (strcmp(infoData, "") != 0) {
			char infoLabel[] = "\nother fields: ";
			int newLen = strlen(printString) + strlen(infoLabel) + strlen(infoData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, infoLabel, strlen(infoLabel) +1);
			strncat(printString, infoData, strlen(infoData) +1);
		}	
		free(infoData);
	}
	return printString;
}

void deleteIndividual(void* toBeDeleted) {
	if (toBeDeleted == NULL) {
		return;
	}
	Individual* invidualToDelete = (Individual*) toBeDeleted;
	free(invidualToDelete->givenName);
	free(invidualToDelete->surname);
	clearList(&invidualToDelete->otherFields);
	clearList(&invidualToDelete->events);
	clearList(&invidualToDelete->families);
	free(invidualToDelete);
}

int compareIndividuals(const void* first,const void* second) {
	if (first == NULL || second == NULL) {
		return 2; // return 2 for bad input
	}
	Individual* personA = (Individual*) first;
	Individual* personB = (Individual*) second;
	// validate individuals and values must not be null
	if (personA == NULL || personB == NULL 
	|| personA->givenName == NULL || personA->surname == NULL 
	|| personB->givenName == NULL || personB->surname == NULL) {
		return 2;
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
	return result;
}

char* printIndividual(void* toBePrinted) {
	if (toBePrinted == NULL) {
		return NULL; // pointer must be valid
	}
	const Individual* printPerson = (Individual*)toBePrinted;
	// label for individual record 
	char individualLabel[] = "Individual Record: ";
	char* printString = malloc(sizeof(char) * strlen(individualLabel) +10);
	strncpy(printString, individualLabel, strlen(individualLabel) +1);
	// add names if they are not empty
	if (printPerson->givenName != NULL && strcmp(printPerson->givenName, "") != 0) {
		char givenLabel[] = "given: ";
		int newLen = strlen(printString) + strlen(givenLabel) + strlen(printPerson->givenName) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, givenLabel, strlen(givenLabel) +1);
		strncat(printString, printPerson->givenName, strlen(printPerson->givenName) +1);
	} else {
		char noGivenLabel[] = "given name unknown";
		int newLen = strlen(printString) + strlen(noGivenLabel) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, noGivenLabel, strlen(noGivenLabel) +1);
	}
	if (printPerson->surname != NULL && strcmp(printPerson->surname, "") != 0) {
		char surLabel[] = ", sur: ";
		int newLen = strlen(printString) + strlen(surLabel) + strlen(printPerson->surname) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, surLabel, strlen(surLabel) +1);
		strncat(printString, printPerson->surname, strlen(printPerson->surname) +1);
	} else {
		char noSurLabel[] = ", surname unknown";
		int newLen = strlen(printString) + strlen(noSurLabel) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, noSurLabel, strlen(noSurLabel) +1);
	}
	// add fields and events if not empty
	if (getLength(printPerson->otherFields) > 0) {
		char* otherData = toString(printPerson->otherFields);
		if (otherData != NULL && strcmp(otherData, "") != 0) {
			char otherLabel[] = "\npersons fields: ";
			int newLen = strlen(printString) + strlen(otherLabel) + strlen(otherData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, otherLabel, strlen(otherLabel) +1);
			strncat(printString, otherData, strlen(otherData) +1);
		}
		free(otherData);
	}
	if (getLength(printPerson->events) > 0) {
		char* eventsData = toString(printPerson->events);
		if (eventsData != NULL && strcmp(eventsData, "") != 0) {
			char eventsLabel[] = "\npersons events: ";
			int newLen = strlen(printString) + strlen(eventsLabel) + strlen(eventsData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, eventsLabel, strlen(eventsLabel) +1);
			strncat(printString, eventsData, strlen(eventsData) +1);
		}
		free(eventsData);
	}
	// line terminate and return string
	strncat(printString, "\n", strlen("\n") +1);
	return printString;
}

void deleteFamily(void* toBeDeleted) {
	if (toBeDeleted == NULL) {
		return;
	}
	Family* familyToDelete = (Family*) toBeDeleted;
	// clear list children is a dummy function
	clearList(&familyToDelete->children);
	clearList(&familyToDelete->events);
	clearList(&familyToDelete->otherFields);
	free(familyToDelete);
}

int compareFamilies(const void* first,const void* second) {
	if (first == NULL || second == NULL) {
		return 2; // return 2 for invalid input
	}
	// cast family objects
	Family* famA = (Family*) first;
	Family* famB = (Family*) second;
	if (famA == NULL || famB == NULL) {
		return 2;
	}
	// Perform comparison of number of family members 
	int countA = 0, countB = 0;
	if (famA->husband != NULL) {
		countA++;
	}
	if (famA->wife != NULL) {
		countA++;
	}
	countA += getLength(famA->children);
	if (famB->husband != NULL) {
		countB++;
	}
	if (famB->wife != NULL) {
		countB++;
	}
	countB += getLength(famB->children);
	// -1 if a < b, 0 if a == b, 1 if a > b
	if (countA == countB) {
		return 0;
	} else if (countA > countB) {
		return 1;
	} else {
		return -1;
	}
}

char* printFamily(void* toBePrinted) {
	if (toBePrinted == NULL) {
		return NULL; // must pass in valid pointer
	}
	const Family* familyObj = (Family*) toBePrinted;
	// family record label
	char familyLabel[] = "Family Record: \n";
	char* printString = malloc(sizeof(char) * strlen(familyLabel) +10); 
	strncpy(printString, familyLabel, strlen(familyLabel) +1);
	// add husband and wife indi string if not null
	if (familyObj->wife != NULL) {		
		char* wifeData = printIndividualName(familyObj->wife);
		// char* wifeData = printIndividual(familyObj->wife);
		if (wifeData != NULL && strcmp(wifeData, "") != 0) {
			char wifeLabel[] = "--WIFE--";
			int newLen = strlen(printString) + strlen(wifeLabel) + strlen(wifeData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, wifeLabel, strlen(wifeLabel) +1);
			strncat(printString, wifeData, strlen(wifeData) +1);
		}
		free(wifeData);
	}	
	if (familyObj->husband != NULL) {
		char* husbandData = printIndividualName(familyObj->husband);
		// char* husbandData = printIndividual(familyObj->husband);
		if (husbandData != NULL && strcmp(husbandData, "") != 0) {
			char husbandLabel[] = "--HUSBAND--";
			int newLen = strlen(printString) + strlen(husbandLabel) + strlen(husbandData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, husbandLabel, strlen(husbandLabel) +1);
			strncat(printString, husbandData, strlen(husbandData) +1);
		}
		free(husbandData);
	}
	// add children list indi strings
	if (getLength(familyObj->children) > 0) {
		char* childData = toString(familyObj->children);
		if (childData != NULL && strcmp(childData, "") != 0) {
			char childLabel[] = "\n--CHILDREN--";
			int newLen = strlen(printString) + strlen(childLabel) + strlen(childData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, childLabel, strlen(childLabel) +1);
			strncat(printString, childData, strlen(childData) +1);
		}
		free(childData);
	}
	// add field and event strings if not empty
	if (getLength(familyObj->otherFields) > 0) {
		char* otherData = toString(familyObj->otherFields);
		if (otherData != NULL && strcmp(otherData, "") != 0) {
			char otherLabel[] = "\nfamily fields: ";
			int newLen = strlen(printString) + strlen(otherLabel) + strlen(otherData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, otherLabel, strlen(otherLabel) +1);
			strncat(printString, otherData, strlen(otherData) +1);
		}
		free(otherData);
	}
	if (getLength(familyObj->events) > 0) {
		char* eventsData = toString(familyObj->events);
		if (eventsData != NULL && strcmp(eventsData, "") != 0) {
			char eventsLabel[] = "\nfamily events: ";
			int newLen = strlen(printString) + strlen(eventsLabel) + strlen(eventsData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, eventsLabel, strlen(eventsLabel) +1);
			strncat(printString, eventsData, strlen(eventsData) +1);
		}
		free(eventsData);
	}
	// add new line and return string
	strncat(printString, "\n", strlen("\n") +1);
	return printString;
}

void deleteGEDCOM(GEDCOMobject* obj) {
	if (obj == NULL) {
		return;
	}
	GEDCOMobject* gedcomToDelete = (GEDCOMobject*) obj;
	deleteHeader(gedcomToDelete->header);
	deleteSubmitter(gedcomToDelete->submitter);
	clearList(&gedcomToDelete->families);
	clearList(&gedcomToDelete->individuals);
	free(gedcomToDelete);
}

char* printGEDCOM(const GEDCOMobject* obj) {
	if (obj == NULL) {
		return NULL; // gedcom object must not be null
	}
	GEDCOMobject* gedcomObj = (GEDCOMobject*) obj;
	if (gedcomObj == NULL || gedcomObj->header == NULL || gedcomObj->submitter == NULL) {
		return NULL; // must be a valid gedcom
	}
	// labels for Gedcom print string
	char gedcomLabel[] = "-----------------------GEDCOM RECORD-----------------------";
	char seperator[] = "\n-----------------------------------------------------------\n";
	// create and allocate gedcom string
	char* printString = malloc(sizeof(char) * strlen(gedcomLabel) +50);
	strncpy(printString, gedcomLabel, strlen(gedcomLabel) +1);
	// add header string to gedcom
	char* headerData = printHeader((void*) gedcomObj->header);
	if (headerData != NULL && strcmp(headerData, "") != 0) {
		int newLen = strlen(printString) +strlen(seperator) +strlen(headerData) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, seperator, strlen(seperator) +1);
		strncat(printString, headerData, strlen(headerData) +1);
	}
	free(headerData);
	// add list of individual and family strings to gedcom
	if (getLength(gedcomObj->individuals) > 0) {
		char individualLabel[] = "---------------------INDIVIDUAL RECORDS--------------------";
		char* individualData = toString(gedcomObj->individuals);
		if (individualData != NULL && strcmp(individualData, "") != 0) {
			int newLen = strlen(printString) +strlen(seperator) +strlen(individualLabel) + strlen(individualData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, seperator, strlen(seperator) +1);
			strncat(printString, individualLabel, strlen(individualLabel) +1);
			strncat(printString, individualData, strlen(individualData) +1);
		}
		free(individualData);
	}
	if (getLength(gedcomObj->families) > 0) {
		char familyLabel[] = "----------------------FAMILY RECORDS-----------------------";
		char* familyData = toString(gedcomObj->families);
		if (familyData != NULL && strcmp(familyData, "") != 0) {
			int newLen = strlen(printString) +strlen(seperator) +strlen(familyLabel) + strlen(familyData) +10;
			printString = realloc(printString, sizeof(char) * newLen);
			strncat(printString, seperator, strlen(seperator) +1);
			strncat(printString, familyLabel, strlen(familyLabel) +1);
			strncat(printString, familyData, strlen(familyData) +1);
		}
		free(familyData);
	}
	// add submitter string
	char* submitterData = printSubmitter((void*) gedcomObj->submitter);
	if (submitterData != NULL && strcmp(submitterData, "") != 0) {
		int newLen = strlen(printString) + (2*strlen(seperator)) + strlen(submitterData) +10;
		printString = realloc(printString, sizeof(char) * newLen);
		strncat(printString, seperator, strlen(seperator) +1);
		strncat(printString, submitterData, strlen(submitterData) +1);
		strncat(printString, seperator, strlen(seperator) +1);
	}
	free(submitterData);
	return printString;
}

char* printError(GEDCOMerror err) {
	char errMessage[75];
	switch(err.type) {
		case OK:
			strncpy(errMessage, "valid GEDCOM created", strlen("valid GEDCOM created") +1);
			break;
		case INV_FILE:
			strncpy(errMessage, "invalid file", strlen("invalid file") +1);
			break;
		case INV_GEDCOM:
			strncpy(errMessage, "invalid GEDCOM", strlen("invalid GEDCOM") +1);
			break;
		case INV_HEADER:
			strncpy(errMessage, "invalid header", strlen("invalid header") +1);
			break;
		case INV_RECORD:
			strncpy(errMessage, "invalid record", strlen("invalid record") +1);
			break;			
		case OTHER_ERROR:
			strncpy(errMessage, "other error, possible allocation error", strlen("other error, possible allocation error") +1);
			break;
		default: 
			strncpy(errMessage, "other error", strlen("other error") +1);
			break;
	}

	// Write line of the error
	if (err.line != -1) {
		char lineError[15];
		sprintf(lineError, "%d", err.line);
		strncat(errMessage, " (line ", strlen(" (line ") +1);
		strncat(errMessage, lineError, strlen(lineError) +1);
		strncat(errMessage, ")\n", strlen(")\n") +1);
	}

	// allocate string and copy contents into it
	char* printString = malloc(sizeof(char) * strlen(errMessage) +10);
	strcpy(printString, errMessage);

	return printString;
}

Individual* findPerson(const GEDCOMobject* familyRecord, bool (*compare)(const void* first, const void* second), const void* person) {
	if (familyRecord == NULL || compare == NULL || person == NULL) {
		return NULL; // input pointers must not be NULL
	}
	ListIterator itr = createIterator(familyRecord->individuals);
	void* elem; // create iterator for the list
	while ( (elem = nextElement(&itr)) != NULL) {
		// use the custom compare function passed in to compare the individuals
		if (compare((const void*)elem, (const void*)person) == true) {
			// cast the void pointer of the found person to a Individual object and return it
			Individual* personFound = (Individual*)elem;
			return personFound;
		}
	}
	// No person found
	return NULL;
}

List getDescendants(const GEDCOMobject* familyRecord, const Individual* person) {
	// create a list of descendants to be returned
	List descendants;
	if (familyRecord == NULL || person == NULL) { // records must ve valid
		descendants = initializeList(&printIndividualName, &deleteIndividual, &compareIndividuals);
		return descendants; // return empty list for invalid input
	}
	// recursive function to get a given persons children, and their children
	descendants = getChildren(familyRecord->families, person);
	if (getLength(descendants) > 0) {
		// remove the head of the list since it is the search person
		Individual* head = getFromFront(descendants);
		deleteDataFromList(&descendants, (void*) head);
		deleteIndividual((void*) head);
	}
	// return the list of descendants
	return descendants;
}

GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj) {	
	//initialize the error object with OK and -1
	GEDCOMerror error = initializeError();
	*obj = NULL; // incase of error, make sure obj is NULL
	// Error checking input parameters
	if (fileName == NULL || strcmp(fileName, "") == 0) {
		error.type = INV_FILE;
		return error;
	}
	// check for .ged extension
	const char ch = '.';
	char* extension = strrchr(fileName, ch);
	if (extension == NULL || strcmp(extension, ".ged") != 0) {
		error.type = INV_FILE;
		return error;
	}
	FILE* file = fopen(fileName, "r" );
	if (file == NULL) { // failed to open the file
		error.type = INV_FILE;
		return error;
	}
	// create and allocate new gedcom object and ensure malloc is successful
	GEDCOMobject* gedcom = malloc(sizeof(GEDCOMobject));
	if (gedcom == NULL) {
		fclose(file);
		free(gedcom);
		error.type = OTHER_ERROR;
		return error;
	}
	// instantiate gedcom members and initilize lists
	gedcom->header = NULL; 
	gedcom->submitter = NULL; 
	gedcom->individuals = initializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
	gedcom->families = initializeList(&printFamily, &deleteFamily, &compareFamilies);
	// create a hashtable to store nodes of keys and pointers to objects
	HashTable* hashMap = createHashTable();
	if (hashMap == NULL) { // ensure malloc is successful
		fclose(file);
		deleteGEDCOM(gedcom); // release memory and return with error
		error.type = OTHER_ERROR; 
		return error;
	}
	// create temporary objects to add created individuals and families to the gedcom list 
	Individual* tempIndi = NULL;
	Family* tempFam = NULL;

	char line[1000] = {0};
	line[999] = '\0';
	char recordBuffer[5000];
	memset(recordBuffer, 0, 5000);

	bool hasClosingTag = false, hasHeader = false;
	int n = 0, level = -1, lastLevel = -1, lineNumber = 0, recordType;
	char c;

	while ( ((c = fgetc(file)) != EOF) && (error.type == OK) ) {
		line[n++] = (char) c;
		// get level of the current line		
		level = line[0] - '0';		
		if (level == 0 && lineNumber > 0) {
			recordType = getRecordType(recordBuffer);
			switch (recordType) {
				case 0: // header record case
					if (level != 0 || gedcom->header != NULL) {
						error.type = INV_GEDCOM;						
					} else {						
						error = createHeader(recordBuffer, &gedcom->header, hashMap);						
					}
					hasHeader = true;
					break;
				case 1: // individual record case
					error = createIndividual(recordBuffer, &tempIndi, hashMap);
					if (error.type != OK) {
						error.line = lineNumber - error.line;
					} else {
						insertBack(&gedcom->individuals, (void*) tempIndi);
					}
					break;
				case 2: // family record case
					error = createFamily(recordBuffer, &tempFam, hashMap);
					if (error.type != OK) {
						error.line = lineNumber - error.line;
					} else {
						insertBack(&gedcom->families, (void*) tempFam);
					}
					break;
				case 3: // submittion record case
					if (gedcom->submitter != NULL) {
						error.type = INV_RECORD;
						error.line = lineNumber;
					} else {
						error = createSubmitter(recordBuffer, &gedcom->submitter, hashMap);
						if (error.type != OK) {
							error.line = lineNumber - error.line;
						}
					}
					break;
				case 4: 
					hasClosingTag = true;					
					break;
				default: 
					break;
			}
			memset(recordBuffer, ' ', 5000);
			strncpy(recordBuffer, line, strlen(line) +1);
			memset(line, 0, 1000);
			line[999] = '\0';
			n = 0;
		}		
		int len = strlen(line);
		if (len > 254) {
			error.type = INV_RECORD;
			error.line = lineNumber +1;
			break;
		}
		if (len >= n && n > 2) {
			// check for proper line increments 
			if (level - lastLevel > 1 && (level >= 0 && lastLevel >= 0 && level < 5)) {
				error.type = INV_RECORD;
				error.line = lineNumber +1;
			}
			lastLevel = level;
			if (line[n-2] == '\n' || line[n-2] == '\r') {
				if (line[n-1] == '\n' || line[n-1] == '\r') {
					line[n-2] = '\n'; // null terminate line to string
					line[n-1] = '\0'; // null terminate line to string
					strncat(recordBuffer, line, strlen(line) +1);
					memset(line, 0, 1000);
					line[999] = '\0';
					n = 0;
				} else {
					line[n-2] = '\n';
					char tempc = line[n-1];
					line[n-1] = '\0';
					strncat(recordBuffer, line, strlen(line) +1);
					memset(line, 0, 1000);
					line[999] = '\0';
					line[0] = tempc;
					n = 1;
				}
				lineNumber++; // increase num lines
				int trlr = getRecordType(recordBuffer);
				if (trlr == 4) { // check for TRLR on every line
					hasClosingTag = true;
					break;
				}
			}
		}
		// check for line length 
		if (strlen(line) >= 255) {
			error.type = INV_RECORD;
			error.line = lineNumber +1;
			break;
		} else if (hasClosingTag == true) { // check for end of file
			break;
		}
	}
	if (error.type == OK && hasHeader == false) {
		error.line = -1;
		error.type = INV_GEDCOM;
	}
	// check last line for TRLR tag
	if (error.type == OK && hasClosingTag == false) {
		const char needle[5] = "TRLR";
		char *ret = NULL;
		ret = strstr(line, needle);
		if (ret != NULL) {
			hasClosingTag = true;
		}
	}
	// no header or no closing tag case
	if (error.type == OK && (gedcom->header == NULL || gedcom->submitter == NULL || hasClosingTag == false)) {
		error.type = INV_GEDCOM;
	}
	// link the families to the individuals using the hash map
	if (error.type == OK) {
		error = linkIndividualsToFamilies(file, gedcom, hashMap);
	}
	// free all data in the hash table
	destroyHashTable(hashMap);
	// no records case
	if (error.type == OK && (getLength(gedcom->individuals) == 0 && getLength(gedcom->families) == 0)) {
		error.type = INV_GEDCOM;		
	}
	// free gedcom created if error
	if (error.type != OK) {
		if (error.type == INV_GEDCOM) {
			error.line = -1;
		}
		deleteGEDCOM(gedcom); // problem in creating gedcom, free all data
		return error;
	}
	// point the obj to the gedcom created
	*obj = gedcom;
	return error;
}

GEDCOMerror writeGEDCOM(char* fileName, const GEDCOMobject* obj) {
	// initialize return value (type = OK, line = -1)
	GEDCOMerror writeErr;
	writeErr.type = OK;
	writeErr.line = -1;

	// validate input parameters
	if (fileName == NULL || obj == NULL) {
		writeErr.type = WRITE_ERROR;
		return writeErr;
	}
	if (obj->header == NULL || obj->submitter == NULL) {
		writeErr.type = WRITE_ERROR;
		return writeErr;
	}
	// validate at least one record
	if (getLength(obj->individuals) == 0 && getLength(obj->families) == 0) {
		writeErr.type = WRITE_ERROR;
		return writeErr;
	}
	// create family object copy
	GEDCOMobject* familyObject = (GEDCOMobject*) obj;
	// create hashmap to link family keys to individuals
	HashTable* hashMap = mapKeys(familyObject);
	// open file to write too	
	if (hashMap == NULL || familyObject == NULL) {	
		destroyHashTable(hashMap);
		writeErr.type = WRITE_ERROR;
		writeErr.line = -1;
		return writeErr;
	}

	char* indiStr = NULL;
	char* familiesStr = NULL;

	// write to string for the header
	char* headStr = writeHeader(obj->header);
	// create string for invidivual records
	if (getLength(obj->individuals) > 0) {
		indiStr = writeIndividuals(obj->individuals, hashMap);
	}
	// create string for family records
	if (getLength(obj->families) > 0) {
		familiesStr = writeFamilys(obj->families, hashMap);
	}
	// write to string for the submitter
	char* submStr = writeSubmitter(obj->submitter);
	
	FILE* filePtr;

	// check if any objects are invalid when creating strings
	if (headStr == NULL || submStr == NULL) {
		writeErr.type = WRITE_ERROR;
		writeErr.line = -1;
		return writeErr;
	} else {
		filePtr = fopen(fileName, "w");
		if (filePtr == NULL) {
			writeErr.type = WRITE_ERROR;
			writeErr.line = -1;
			return writeErr;
		}
		// write strings to file
		fprintf(filePtr, "%s", headStr);
		if (indiStr != NULL) {
			fprintf(filePtr, "%s", indiStr);
			free(indiStr);
		}
		if (familiesStr != NULL) {
			fprintf(filePtr, "%s", familiesStr);
			free(familiesStr);
		}
		fprintf(filePtr, "%s", submStr);
		fprintf(filePtr, "%s", "0 TRLR\n");

		free(headStr);
		free(submStr);
		// close the file  
		fclose(filePtr);
	}	
	
	// free memory allocated
	destroyHashTable(hashMap);
	return writeErr;
}

ErrorCode validateGEDCOM(const GEDCOMobject* obj) {
	// initialize error code to OK
	ErrorCode err = OK;
	// validate GEDCOM object
	if (obj == NULL) {
		return INV_GEDCOM;
	}
	// validate header and submitter object
	if (obj->header == NULL || obj->submitter == NULL) {
		return INV_GEDCOM;
	}
	// validate header members 
	if (obj->header->source == NULL || strcmp(obj->header->source, "") == 0 || obj->header->submitter == NULL) {
		return INV_HEADER;
	}
	err = validateFieldList(obj->header->otherFields);
	if (err != OK) {
		return err;
	}
	// validate submitter members
	if (obj->submitter->submitterName == NULL || obj->submitter->address == NULL || strlen(obj->submitter->submitterName) == 0) {
		return INV_RECORD;
	}
	// validate submitter fields
	err = validateFieldList(obj->submitter->otherFields);
	if (err != OK) {
		return err;
	}
	// validate gedcom has atleast 1 record
	// if (getLength(obj->individuals) == 0 && getLength(obj->families) == 0) {
	// 	return INV_GEDCOM;
	// }
	// validate individual list
	if (getLength(obj->individuals) > 0) {
		ListIterator personItr = createIterator(obj->individuals);
		void* personElem;
		while( (personElem = nextElement(&personItr) ) != NULL) {
			Individual* tempPerson = (Individual*) personElem;
			// validate individual members
			if (tempPerson == NULL || tempPerson->givenName == NULL || tempPerson->surname == NULL) {
				return INV_RECORD;
			}			
			err = validateFieldList(tempPerson->otherFields); // validate individual fields
			if (err != OK) {
				return err;
			}			
			err = validateEventList(tempPerson->events); // validate individual events
			if (err != OK) {
				return err;
			}
		}
	}
	// validate families list
	if (getLength(obj->families) > 0) {
		ListIterator familyItr = createIterator(obj->families);
		void* familyElem;
		while( (familyElem = nextElement(&familyItr) ) != NULL) {
			Family* tempFamily = (Family*) familyElem;			
			if (tempFamily == NULL) { // validate family members
				return INV_RECORD;
			}			
			err = validateFieldList(tempFamily->otherFields); // validate family fields
			if (err != OK) {
				return err;
			}			
			err = validateEventList(tempFamily->events); // validate family events
			if (err != OK) {
				return err;
			}
		}
	}
	return err;
}

List getDescendantListN(const GEDCOMobject* familyRecord, const Individual* person, unsigned int maxGen) {
	// initialize decendants list
	List nDescendants = initializeList(&printGeneration, &deleteGeneration, &compareGenerations);
	// check for valid arguements
	if (familyRecord == NULL || person == NULL || getLength(familyRecord->families) < 1) { // records must ve valid
		return nDescendants; // return empty list for invalid input
	}
	// adjust maxGen if all decendants required
	if (maxGen == 0 || maxGen > getLength(familyRecord->families)) {
		maxGen = getLength(familyRecord->families);
	}
	int i;
	List* childList = NULL;

	for(i = 0; i < maxGen; i++) {
		if (getLength(nDescendants) < 1) {
			// allocate child list string
			List* childList = malloc(sizeof(List));
			*childList = initializeList(&printIndividualName, &deleteIndividual, &compareIndividuals);

			ListIterator famItr = createIterator(familyRecord->families);
			void* famElem;
			while( (famElem = nextElement(&famItr)) != NULL) {
				Family* tempFam = (Family*) famElem;
				if ((tempFam->wife != NULL && customCompareIndividuals(tempFam->wife, person) == true) 
				|| (tempFam->husband != NULL && customCompareIndividuals(tempFam->husband, person) == true)) {
					if (getLength(tempFam->children) > 0) {
						ListIterator childItr = createIterator(tempFam->children);
						void* childElem;
						while( (childElem = nextElement(&childItr) ) != NULL) {					
							Individual* tempChild = (Individual*) childElem;
							Individual* toCopy;
							copyIndividual(&toCopy, tempChild);							
							if (tempChild->surname == NULL || strcmp(tempChild->surname, "") == 0) {
								insertBack(childList, (void*) toCopy);
							} else {
								insertSorted(childList, (void*) toCopy);
							}
						}
						insertBack(&nDescendants, (void*) childList);
					} else {
						clearList(childList);
						free(childList);
						return nDescendants;
					}
				}
			}
			if (getLength(nDescendants) < 1) {
				clearList(childList);
				free(childList);
				return nDescendants;
			}
		} else {
			List* lastGen = getFromBack(nDescendants);
			if (childList != NULL) {				
				childList = NULL;
			}
			childList = malloc(sizeof(List));
			*childList = initializeList(&printIndividualName, &deleteIndividual, &compareIndividuals);
			ListIterator personItr = createIterator(*lastGen);
			void* personElem;
			while( (personElem = nextElement(&personItr)) != NULL) {
				Individual* tempPerson = (Individual*) personElem;
				ListIterator famItr = createIterator(familyRecord->families);
				void* famElem;
				while( (famElem = nextElement(&famItr)) != NULL) {
					Family* tempFam = (Family*) famElem;
					if ((tempFam->wife != NULL && customCompareIndividuals(tempFam->wife, tempPerson) == true) 
					|| (tempFam->husband != NULL && customCompareIndividuals(tempFam->husband, tempPerson) == true)) {
						if (getLength(tempFam->children) > 0) {
							ListIterator childItr = createIterator(tempFam->children);
							void* childElem;
							while( (childElem = nextElement(&childItr) ) != NULL) {													
								Individual* tempChild = (Individual*) childElem;
								if (alreadyInserted(nDescendants, tempChild) == false) {									
									Individual* toCopy;
									copyIndividual(&toCopy, tempChild);
									if (tempChild->surname == NULL || strcmp(tempChild->surname, "") == 0) {
										insertBack(childList, (void*) toCopy);
									} else {
										insertSorted(childList, (void*) toCopy);
									}
								}
							}
						} else {
							break;
						}
					}
				}
			}
			if (getLength(*childList) < 1) { // check for valid list and not empty
				clearList(childList);
				free(childList);
				return nDescendants;
			}
			if (compareGenerations(childList, lastGen) == 0) { // check for duplicate generations
				clearList(childList);
				free(childList);
				return nDescendants;
			} else { // insert list into list
				insertBack(&nDescendants, (void*) childList);

			}
		}
	}
	return nDescendants;
}

List getAncestorListN(const GEDCOMobject* familyRecord, const Individual* person, int maxGen) {
	// initialize ancestors list
	List nAncestors = initializeList(&printGeneration, &deleteGeneration, &compareGenerations);
	// validate arguements
	if (familyRecord == NULL || person == NULL || maxGen < 0 || getLength(familyRecord->families) < 1) { // records must ve valid
		return nAncestors; // return empty list for invalid input
	}
	// recursive function to get a given persons children, and their children
	if (maxGen == 0 || maxGen > getLength(familyRecord->families)) {
		maxGen = getLength(familyRecord->families);
	}

	int i;
	for(i = 0; i < maxGen; i++) {
		if (i == 0) {
			List* parentList = malloc(sizeof(List));
			*parentList = initializeList(&printIndividualName, &deleteIndividual, &compareIndividuals);
			ListIterator famItr = createIterator(person->families);
			void* famElem;
			while( (famElem = nextElement(&famItr)) != NULL) {
				Family* tempFam = (Family*) famElem;
				if (getLength(tempFam->children) > 0) {
					ListIterator childItr = createIterator(tempFam->children);
					void* childElem;
					while( (childElem = nextElement(&childItr) ) != NULL) {					
						Individual* tempChild = (Individual*) childElem;
						if (customCompareIndividuals(tempChild, person) == true) {
							if (tempFam->wife == NULL && tempFam->husband == NULL) {
								clearList(parentList);
								free(parentList);
								return nAncestors;
							}
							if (tempFam->wife != NULL) {
								if (alreadyInserted(nAncestors, tempFam->wife) == false) {
									Individual* toCopy;
									copyIndividual(&toCopy, tempFam->wife);
									if (toCopy->surname == NULL || strcmp(toCopy->surname, "") == 0) {
										insertBack(parentList, (void*) toCopy);
									} else {
										insertSorted(parentList, (void*) toCopy);
									}
								}								
							}
							if (tempFam->husband != NULL) {
								if (alreadyInserted(nAncestors, tempFam->husband) == false) {
									Individual* toCopy;
									copyIndividual(&toCopy, tempFam->husband);
									if (toCopy->surname == NULL || strcmp(toCopy->surname, "") == 0) {
										insertBack(parentList, (void*) toCopy);
									} else {
										insertSorted(parentList, (void*) toCopy);
									}
								}								
							}
							if (getLength(nAncestors) > 0) {
								List* lastGenTst = getFromBack(nAncestors);
								if (compareGenerations(parentList, lastGenTst) == 0) {
									return nAncestors;
								}
							} else if (getLength(*parentList) > 0) {
								insertBack(&nAncestors, (void*) parentList);
								break;
							}
						}
					}
				}
			}
			if (getLength(nAncestors) < 1) {
				clearList(parentList);
				free(parentList);
				return nAncestors;
			}
		} else {
			List* lastGen = getFromBack(nAncestors);
			List* parentList = malloc(sizeof(List));
			*parentList = initializeList(&printIndividualName, &deleteIndividual, &compareIndividuals);
			ListIterator personItr = createIterator(*lastGen);
			void* personElem;
			while( (personElem = nextElement(&personItr)) != NULL) {
				Individual* tempPerson = (Individual*) personElem;
				ListIterator famItr = createIterator(familyRecord->families);
				void* famElem;
				while( (famElem = nextElement(&famItr)) != NULL) {
					Family* tempFam = (Family*) famElem;
					if (getLength(tempFam->children) > 0) {
						ListIterator childItr = createIterator(tempFam->children);
						void* childElem;
						while( (childElem = nextElement(&childItr) ) != NULL) {	
							Individual* tempChild = (Individual*) childElem;
							if (customCompareIndividuals(tempChild, tempPerson) == true) {
								if (tempFam->wife != NULL) {									
									if (alreadyInserted(nAncestors, tempFam->wife) == false) {
										Individual* toCopy;
										copyIndividual(&toCopy, tempFam->wife);
										if (toCopy->surname == NULL || strcmp(toCopy->surname, "") == 0) {
											insertBack(parentList, (void*) toCopy);
										} else {
											insertSorted(parentList, (void*) toCopy);
										}
									}
								}
								if (tempFam->husband != NULL) {
									if (alreadyInserted(nAncestors, tempFam->husband) == false) {
										Individual* toCopy;
										copyIndividual(&toCopy, tempFam->husband);
										if (toCopy->surname == NULL || strcmp(toCopy->surname, "") == 0) {
											insertBack(parentList, (void*) toCopy);
										} else {
											insertSorted(parentList, (void*) toCopy);
										}
									}									
								}				
							}
						}
					}
				}				
			}
			if (getLength(*parentList) < 1) {
				clearList(parentList);
				free(parentList);
				return nAncestors;
			}
			if (compareGenerations(parentList, lastGen) == 0) {
				clearList(parentList);
				free(parentList);
				return nAncestors;
			} else {
				insertBack(&nAncestors, (void*) parentList);
			}
		}
	}
	return nAncestors;
}

void deleteGeneration(void* toBeDeleted) {
	if (toBeDeleted == NULL) {
		return;
	}	
	List* deleteGenList = (List*) toBeDeleted;
	clearList(deleteGenList);
	free(deleteGenList);
	return;
}

int compareGenerations(const void* first,const void* second) {
	if (first == NULL || second == NULL) {
		return 2;
	}
	// cast generations list
	List* firstGenList = (List*) first;
	List* secondGenList = (List*) second;
	// compare lengths of lists first
	if (getLength(*firstGenList) == getLength(*secondGenList)) {

		char* printStr1 = printGeneration((void*) firstGenList);
		char* printStr2 = printGeneration((void*) secondGenList);
		int result = 0;
		if (strcmp(printStr1, printStr2) == 0) {
			result = 0;
		} else if (strcmp(printStr1, printStr2) > 0) {
			result = 1;
		} else {
			result = -1;
		}
		free(printStr1);
		free(printStr2);
		return result;
	} else if (getLength(*firstGenList) > getLength(*secondGenList) ){
		return 1;
	} else {
		return -1;
	}
}

char* printGeneration(void* toBePrinted) {
	if (toBePrinted == NULL) {
		return NULL;
	}
	// cast list to be printed
	List* printGenList = (List*) toBePrinted;
	// allocate string for the generation of individuals
	char* genListStr = malloc(sizeof(char) * strlen("") +1);
	strncpy(genListStr, "", strlen("") +1);
	// check for valid non empty list
	if (getLength(*printGenList) < 1) {
		return genListStr;
	} else {
		free(genListStr);
		genListStr = NULL;
	}
	// labels to seperate generations
	char seperator[] = "-------Generation-------";
	char seperator2[] = "\n------------------------";
	// get list of individuals in genration
	char* printGenListStr = toString(*printGenList);
	// check for valid string
	if (printGenListStr != NULL) {
		genListStr = malloc(sizeof(char)*(strlen(seperator)+strlen(seperator2) +strlen(printGenListStr))+10);
		strncpy(genListStr, seperator, strlen(seperator) +1);	
		strncat(genListStr, printGenListStr, strlen(printGenListStr) +1);
		strncat(genListStr, seperator2, strlen(seperator2) +1);
	}
	free(printGenListStr);
	return genListStr;
}

char* indToJSON(const Individual* ind) {
	// initialize return string incase bad input
	char* jsonStr = malloc(sizeof(char) * strlen("") +1);
	strncpy(jsonStr, "", strlen("") +1);
	if (ind == NULL) {
		return jsonStr;
	} else {
		free(jsonStr);
		jsonStr = NULL;
	}
	// create json keys	
	char givenLbl[] = "{\"givenName\":\"";
	char surnameLbl[] = "\",\"surname\":\"";
	char endLbl[] = "\"}";
	int length = strlen(givenLbl) +strlen(surnameLbl) +strlen(endLbl) +1;
	if (ind->givenName != NULL) {
		length += strlen(ind->givenName) +1;
	} 
	if (ind->surname != NULL) {
		length += strlen(ind->surname) +1;
	}
	// alllocate json string
	jsonStr = malloc(sizeof(char) * length);
	strncpy(jsonStr, givenLbl, strlen(givenLbl) +1);
	if (ind->givenName != NULL) {
		strncat(jsonStr, ind->givenName, strlen(ind->givenName) +1);
	} 
	strncat(jsonStr, surnameLbl, strlen(surnameLbl) +1);
	if (ind->surname != NULL) {
		strncat(jsonStr, ind->surname, strlen(ind->surname) +1);
	}
	strncat(jsonStr, endLbl, strlen(endLbl) +1);
	return jsonStr;
}

Individual* JSONtoInd(const char* str) {
	if (str == NULL) {
		return NULL;
	}
	char* data = strdup(str);
	if (data == NULL) {
		return NULL;	
	}
	// allocate new individual object and initialize members
	Individual* person = malloc(sizeof(Individual));
	person->givenName = NULL;
	person->surname = NULL;
	person->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
	person->otherFields = initializeList(&printField, &deleteField, &compareFields);
	person->families = initializeList(&printFamily, &deleteWeakReference, &compareFamilies);

	char* key = strtok(data, "{}\":,");
	char* lastKey = NULL;
	bool givenFlag = false, surnameFlag = false;

	while (key != NULL) {
		lastKey = malloc(sizeof(char) * strlen(key) +1);
		strncpy(lastKey, key, strlen(key) +1);
		key = strtok(NULL, "{}\":,");
		if (key != NULL && lastKey != NULL) {			
			if (strcmp(lastKey, "givenName") == 0) {
				if (strcmp(key, "surname") == 0) {
					person->givenName = malloc(sizeof(char) * strlen("") +1);
					strncpy(person->givenName, "", strlen("") +1);
				} else {
					person->givenName = malloc(sizeof(char) * strlen(key) +1);
					strncpy(person->givenName, key, strlen(key) +1);
				}					
				givenFlag = true;
			}
			if (strcmp(lastKey, "surname") == 0) {
				person->surname = malloc(sizeof(char) * strlen(key) +1);
				strncpy(person->surname, key, strlen(key) +1);
				surnameFlag = true;
			}
		}
		free(lastKey);
	}
	// check for valid parsing
	if (givenFlag == false && surnameFlag == false) {
		deleteIndividual((void*) person);
		return NULL;
	} else if (givenFlag == false) {
		person->givenName = malloc(sizeof(char) * strlen("") +1);
		strncpy(person->givenName, "", strlen("") +1);
	} else if (surnameFlag == false) {
		person->surname = malloc(sizeof(char) * strlen("") +1);
		strncpy(person->surname, "", strlen("") +1);
	}
	return person;
}

GEDCOMobject* JSONtoGEDCOM(const char* str) {
	if (str == NULL) {
		return NULL;
	}
	char* data = strdup(str);
	if (data == NULL) {
		return NULL;
	}
	// create and initialize gedcom object
	GEDCOMobject* gedcom = malloc(sizeof(GEDCOMobject));
	gedcom->header = malloc(sizeof(Header)); 
	gedcom->header->otherFields = initializeList(&printField, &deleteField, &compareFields);
	gedcom->submitter = malloc(sizeof(Submitter) + strlen("") +1);
	gedcom->submitter->otherFields = initializeList(&printField, &deleteField, &compareFields);
	gedcom->individuals = initializeList(&printIndividual, &deleteIndividual, &compareIndividuals);
	gedcom->families = initializeList(&printFamily, &deleteFamily, &compareFamilies);

	char* key = strtok(data, "{}\":,");
	char* lastKey = NULL;
	bool sourceFlag = false, gedcFlag = false, encodingFlag = false, subnNameFlag = false;

	while (key != NULL) {

		lastKey = malloc(sizeof(char) * strlen(key) +1);
		strncpy(lastKey, key, strlen(key) +1);
		key = strtok(NULL, "{}\":,");
		if (lastKey != NULL && key != NULL) {
			if (strcmp(lastKey, "source") == 0) {
				if (strlen(key) < 249) {
					strncpy(gedcom->header->source, key, strlen(key) +1);
					sourceFlag = true;
				}
			} else if (strcmp(lastKey, "gedcVersion") == 0) {
				gedcom->header->gedcVersion = atof(key);
				gedcFlag = true;
			} else if (strcmp(lastKey, "encoding") == 0) {
				encodingFlag = true;
				if (strcmp(key, "ANSEL") == 0) {
					gedcom->header->encoding = ANSEL;
				} else if ((strcmp(key, "UTF8") == 0) || (strcmp(key, "UTF-8") == 0)) {
					gedcom->header->encoding = UTF8;
				} else if (strcmp(key, "UNICODE") == 0) {
					gedcom->header->encoding = UNICODE;
				} else if (strcmp(key, "ASCII") == 0) {
					gedcom->header->encoding = ASCII;
				} else {
					encodingFlag = false;
				}
			} else if (strcmp(lastKey, "subName") == 0) {
				if (strlen(key) < 61) {
					strncpy(gedcom->submitter->submitterName, key, strlen(key) +1);
					subnNameFlag = true;
				}
			} else if (strcmp(lastKey, "subAddress") == 0) {
				gedcom->submitter = realloc(gedcom->submitter, sizeof(Submitter) + strlen(key) +10);
				strncpy(gedcom->submitter->address, key, strlen(key) +1);
			}
			free(lastKey);
		}
		
	}
	free(data);
	if (sourceFlag == false || gedcFlag == false || encodingFlag == false || subnNameFlag == false) {
		deleteGEDCOM(gedcom);
		return NULL;
	}
	gedcom->header->submitter = gedcom->submitter;
	return gedcom;
}

void addIndividual(GEDCOMobject* obj, const Individual* toBeAdded) {
	// validate arguements
	if (obj == NULL || toBeAdded == NULL) {
		return;
	}
	// insert individual into sorted list
	insertSorted(&obj->individuals, (void*) toBeAdded);
	return;
}

char* iListToJSON(List iList) {
	// create return string and initialize incase bad arguements
	char* iListStr = malloc(sizeof(char) * strlen("") +1);
	// strncpy(iListStr, "", strlen("") +1);
	// // check lengths of iList
	// if (getLength(iList) < 0) {
	// 	return iListStr;
	// } else 
	if (getLength(iList) < 1) {
		iListStr = realloc(iListStr, sizeof(char) * strlen("[]") +1);
		strncpy(iListStr, "[]", strlen("[]") +1);
		return iListStr;
	} else {
		free(iListStr);
		iListStr = NULL;
	}

	ListIterator personItr = createIterator(iList);
	void* personElem;
	while( (personElem = nextElement(&personItr)) != NULL) {
		// cast to individual and get json of that individual
		Individual* person = (Individual*) personElem;
		char* personJson = indToJSON(person);

		if (personJson != NULL) {
			if (iListStr == NULL) {
				iListStr = malloc(sizeof(char) * (strlen("[]") + strlen(personJson)) +10);	
				strncpy(iListStr, "[", strlen("[") +1);
				strncat(iListStr, personJson, strlen(personJson) +1);
			} else {
				iListStr = realloc(iListStr, sizeof(char) * (strlen(iListStr) +strlen(personJson)) +10);			
				strncat(iListStr, ",", strlen(",") +1);
				strncat(iListStr, personJson, strlen(personJson) +1);
			}
		}
		free(personJson);
	}
	strncat(iListStr, "]", strlen("]") +1);

	return iListStr;
}

char* gListToJSON(List gList) {
	// initialize string and allocate incase bad list
	char* gListStr = malloc(sizeof(char) * strlen("") +1);
	strncpy(gListStr, "", strlen("") +1);
	
	// if (getLength(gList) < 0) {
	// 	return gListStr;
	// } else 
	if (getLength(gList) < 1) {
		gListStr = realloc(gListStr, sizeof(char) * strlen("[]") +1);
		strncpy(gListStr, "[]", strlen("[]") +1);
		return gListStr;
	} else {
		free(gListStr);
		gListStr = NULL;
	}

	ListIterator generationItr = createIterator(gList);
	void* listElem;
	while( (listElem = nextElement(&generationItr)) != NULL) {
		// cast list and get json representation
		List* list = (List*) listElem;
		char* iListStr = iListToJSON(*list);

		if (iListStr != NULL) {
			if (gListStr == NULL) {
				gListStr = malloc(sizeof(char) * (strlen("[[]]") +strlen(iListStr)) +1);
				strncpy(gListStr, "[", strlen("[") +1);
				strncat(gListStr, iListStr, strlen(iListStr) +1);
			} else {
				gListStr = realloc(gListStr, sizeof(char)*(strlen(",[]")+strlen(gListStr)+strlen(iListStr))+1);			
				strncat(gListStr, ",", strlen(",") +1);
				strncat(gListStr, iListStr, strlen(iListStr) +1);
			}
		}
		free(iListStr);
	}
	strncat(gListStr, "]", strlen("]") +1);

	return gListStr;
}
