#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "Toufiq-sheehan-assignment7.h"

main() { 
	char choice;
	do {
		Menu(&choice); 
	 	switch (choice) {
	 		case 'a':
	 			addContact(); 
	 			break;
	 		case 'r':
	 			removeContact(); 
	 			break; 
	 		case 's':
	 			searchContacts(); 
	 			break; 
	 		case 'd':
	 			displayContacts(); 
	 			break; 
	 		case 'e':
	 			exit(EXIT_SUCCESS); 
		}
	} while(choice != 'e'); 
} 

void Menu(char *choice) {
	regex_t regex;
	char local;
	int reti;
	reti = regcomp(&regex, "[arsde]", 0);

	for (;;) {
		printf("\n (a) Add a contact\t");
		printf("\n (r) Remove a contact\t");
		printf("\n (s) Search a contact\t");
		printf("\n (d) Display a contact\t");
		printf("\n (e) Exit\n\t");
		printf("\n Enter your choice: ");

		local = getchar();
		clearInputBuffer(); /* clears input buffer */
		
		reti = regexec(&regex, &local, 0, NULL, 0);

		// the input (local) is compared with [arsde]
		if (!reti) {
			break;
		}
		// regexec() failed to match
		else if (reti == REG_NOMATCH){
			printf("\n Your choice %c is not valid. Please choose from the menu.", local);
		}
	}
	*choice = local;
}

// clears input buffer so the Menu doesn't loop with the newline character
void clearInputBuffer() {
	char c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

void addContact() {
	printf("\n[Add Contact]\n");

	contactInfo *contact;
	contact = malloc(sizeof(contactInfo));

 	contact->fName = malloc(sizeof(char) * FNAME_MAX_LENGTH);
 	contact->lName = malloc(sizeof(char) * LNAME_MAX_LENGTH);
 	contact->pNum = malloc(sizeof(char) * PNUM_MAX_LENGTH);
 	contact->address = malloc(sizeof(char) * ADDRESS_MAX_LENGTH);
 	contact->email = malloc(sizeof(char) * EMAIL_MAX_LENGTH);

 	printf("First Name: ");
 	fgets(contact->fName, FNAME_MAX_LENGTH, stdin);
 	if (contact->fName[strlen(contact->fName) - 1] == '\n') {
 		contact->fName[strlen(contact->fName) - 1] = '\0';
 	}

 	printf("Last Name: ");
 	fgets(contact->lName, LNAME_MAX_LENGTH, stdin);
 	if (contact->lName[strlen(contact->lName) - 1] == '\n') {
 		contact->lName[strlen(contact->lName) - 1] = '\0';
 	}

 	printf("Phone Number: ");
 	fgets(contact->pNum, PNUM_MAX_LENGTH, stdin);
 	if (contact->pNum[strlen(contact->pNum) - 1] == '\n') {
 		contact->pNum[strlen(contact->pNum) - 1] = '\0';
 	}
 	int i = checkPhone(contact->pNum);

 	printf("Address: ");
 	fgets(contact->address, ADDRESS_MAX_LENGTH, stdin);
 	if (contact->address[strlen(contact->address) - 1] == '\n') {
 		contact->address[strlen(contact->address) - 1] = '\0';
 	}

 	printf("Email: ");
 	fgets(contact->email, EMAIL_MAX_LENGTH, stdin);
 	if (contact->email[strlen(contact->email) - 1] == '\n') {
 		contact->email[strlen(contact->email) - 1] = '\0';
 	}

 	if (i == 1) {
 		// FILE *fopen(const char *filename, const char *mode) opens the filename pointed to by filename using the given mode.
 		// "r": open, "w": create, "a": append, "w+" created an empty file for both reading and writing
 		FILE *fd;
 		fd = fopen(FILENAME, "a");
 		fprintf(fd, "\n%s %s %s %s\t%s", contact->fName,contact->lName,contact->pNum,contact->address,contact->email);
 		fclose(fd);
 		printf("%s %s has been added to your contacts.", contact->fName,contact->lName);
 	}

 	free(contact->fName);
 	free(contact->lName);
 	free(contact->pNum);
 	free(contact->address);
 	free(contact->email);
 	free(contact);

}

void removeContact() {
	int maxLineLength = FNAME_MAX_LENGTH + LNAME_MAX_LENGTH + PNUM_MAX_LENGTH + ADDRESS_MAX_LENGTH + EMAIL_MAX_LENGTH + 5; 
	char str[maxLineLength]; 
	int result; 
	int count = 0; 
	 
	FILE *originalFile; 
	FILE *tempFile; 
	originalFile = fopen(FILENAME, "r");
	if(originalFile == NULL) { 
		printf("Error! File does not exist!"); 
		main(); 
	} 
	tempFile = fopen(TEMPFILENAME, "w+"); 
	 
	contactInfo *contact; 
	contact = malloc (sizeof(contactInfo)); 
	 
 	contact->fName = malloc(sizeof(char) * FNAME_MAX_LENGTH);
 	contact->lName = malloc(sizeof(char) * LNAME_MAX_LENGTH);

	printf("\n[Remove a Contact]\nFirst Name: "); 
	fgets(contact->fName,FNAME_MAX_LENGTH,stdin); 
	if (contact->fName[strlen(contact->fName) - 1] == '\n') { 
		contact->fName[strlen(contact->fName) - 1] = '\0'; 
	}

	printf("Last Name: "); 
	fgets(contact->lName,LNAME_MAX_LENGTH,stdin); 
	if (contact->lName[strlen(contact->lName) - 1] == '\n') { 
		contact->lName[strlen(contact->lName) - 1] = '\0'; 
	}

	int compareLimit = strlen(contact->fName) + strlen(contact->lName) + 1; 
	char* fullName = malloc(sizeof(char) * compareLimit); 

	// int sprintf(char *str, const char *format, ...) sends formatted output to a string pointed to by str.
	sprintf(fullName,"%s %s", contact->fName, contact->lName);

	while(!feof(originalFile)) { 

		// int fscanf(FILE *stream, const char *format, ...) reads formatted input from a stream.
 		fscanf(originalFile, "\n%[^\n]", str); 

 		// int strncasecmp(const char *string1, const char *string2, size_t count) function
		// compares up to count characters of string1 and string2 without sensitivity to case.
 		result = strncasecmp(fullName, str, compareLimit); 
 		if (result == 0) { 
 			count++;
 			continue;
 		} 
 		// int fprintf(FILE *stream, const char *format, ...) sends formatted output to a stream.
 		fprintf(tempFile,"\n%s",str); 
 	}

 	fclose(originalFile); 
 	fclose(tempFile);

 	if( remove(FILENAME) != 0 ) { 
 		perror( "Error deleting file" ); 
 	}
	/*	int rename(const char *old_filename, const char *new_filename) causes the filename referred to 
			by old_filename to be changed to new_filename. */
 	result= rename( TEMPFILENAME , FILENAME ); 

	if ( result != 0 ) { 
		perror( "Error renaming file" ); 
	} 
	if(count > 0) { 
		printf("%s %s is removed from your contacts.\n", contact->fName, contact->lName); 
	} 
	else { 
		printf("%s %s is not in your contacts.\n", contact->fName, contact->lName); 
	} 

 	free(contact->fName);
 	free(contact->lName);
	free(contact); 
	free(fullName); 
}

void searchContacts() {
	int maxLineLength = FNAME_MAX_LENGTH + LNAME_MAX_LENGTH + PNUM_MAX_LENGTH + ADDRESS_MAX_LENGTH + EMAIL_MAX_LENGTH + 5; 
	char str[maxLineLength]; 
	char fName[FNAME_MAX_LENGTH]; 
	int result; 
 
	FILE *fd; 
	fd = fopen(FILENAME, "r"); 
	if(fd == NULL) { 
		printf("Error! File does not exist!"); 
		main(); 
	} 
 
	contactInfo *contact; 

	contact = malloc (sizeof(contactInfo)); 
 	contact->fName = malloc(sizeof(char) * FNAME_MAX_LENGTH);
 	contact->lName = malloc(sizeof(char) * LNAME_MAX_LENGTH);
 	contact->pNum = malloc(sizeof(char) * PNUM_MAX_LENGTH);
 	contact->address = malloc(sizeof(char) * ADDRESS_MAX_LENGTH);
 	contact->email = malloc(sizeof(char) * EMAIL_MAX_LENGTH);
 
	printf("\n[Search contacts]\nSearch by the contact's First Name: "); 
	fgets(fName, FNAME_MAX_LENGTH, stdin);
	if (fName[strlen(fName) - 1] == '\n') { 
		contact->fName[strlen(fName) - 1] = '\0'; 
	}

	int compareLimit = strlen(fName); 
 
	while(!feof(fd)) {
		fscanf(fd, "%s %s %s %[^\t]%s", contact->fName, contact->lName, contact->pNum, contact->address, contact->email); 
		result = strncasecmp(fName, contact->fName, compareLimit); 
 		
		// I set this to 10 because strncasecmp was returning 10 when making a match for some reason,
		// which is odd because it returns 0 in removeContact()
 		if(result == 10) {
	 		printf("\nFirst Name: %s\nLast Name: %s\nPhone Number: %s\nAddress: %s\nEmail: %s\n", contact->fName, contact->lName, contact->pNum, contact->address, contact->email); 
	 	}
	 	/*else {
	 		printf("%d\n", result); <= This returns 10
	 	}*/
	} 
 
	fclose(fd); 
 
 	free(contact->fName);
 	free(contact->lName);
 	free(contact->pNum);
 	free(contact->address);
 	free(contact->email);
	free(contact); 
} 

void displayContacts() { 
	char choice; 
	do { 
 		for (;;) { 

			printf("\n[Display contacts]");
			printf("\n(1) Display all");
			printf("\n(2) Return to Main Menu\n");

			printf("\nPlease enter your choice: \n");
 
 			choice = getchar(); 
 			clearInputBuffer(); 
 
 			if((choice == '1') || (choice == '2')) { 
 				break; 
 			} 
 			else { 
 				printf("\nYour choice %c is not valid. Please choose from the menu.", choice); 
 			} 
 		}	 
 		if(choice == '1'){ 
 			displayAll(); 
 		} 
	} while(choice != '2'); 
}

 
void displayAll() { 
	int maxLineLength = FNAME_MAX_LENGTH + LNAME_MAX_LENGTH + PNUM_MAX_LENGTH + ADDRESS_MAX_LENGTH + EMAIL_MAX_LENGTH + 5; 
	char str[maxLineLength]; 
 
	FILE *fd; 
	fd = fopen(FILENAME, "r"); 
	if(fd == NULL) { 
		printf("Error! File does not exist!"); 
		main(); 
	} 
 
	contactInfo *contact; 

	contact = malloc (sizeof(contactInfo));
 	contact->fName = malloc(sizeof(char) * FNAME_MAX_LENGTH);
 	contact->lName = malloc(sizeof(char) * LNAME_MAX_LENGTH);
 	contact->pNum = malloc(sizeof(char) * PNUM_MAX_LENGTH);
 	contact->address = malloc(sizeof(char) * ADDRESS_MAX_LENGTH);
 	contact->email = malloc(sizeof(char) * EMAIL_MAX_LENGTH);
 
	while(!feof(fd)) { 
		fscanf(fd, "%s %s %s %[^\t]%s", contact->fName, contact->lName, contact->pNum, contact->address, contact->email); 
		printf("\nFirst Name: %s\nLast Name: %s\nPhone Number: %s\nAddress: %s\nEmail: %s\n", contact->fName, contact->lName, contact->pNum, contact->address, contact->email); 
	} 
 
	fclose(fd); 
 
 	free(contact->fName);
 	free(contact->lName);
 	free(contact->pNum);
 	free(contact->address);
 	free(contact->email);
	free(contact);
}

int checkPhone(char *pNum) {
	regex_t regex;
	char *local;
	local = pNum;
	int reti;

	// Regex checks to see if the phone is in the correct form
	reti = regcomp(&regex, "[0-9][-][0-9]{3}[-][0-9]{4}", REG_EXTENDED);
	reti = regexec(&regex, local, 0, NULL, 0);
	if (!reti) {
		return 1;
	}
	else {
		printf("The phone number %s is not valid.\n", pNum);
		main();
	}
}