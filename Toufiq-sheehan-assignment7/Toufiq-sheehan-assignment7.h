#define FNAME_MAX_LENGTH 15
#define LNAME_MAX_LENGTH 15
#define PNUM_MAX_LENGTH 20
#define ADDRESS_MAX_LENGTH 70
#define EMAIL_MAX_LENGTH 30

void Menu(char *choice);
void clearInputBuffer();
void addContact();
void removeContact();
void searchContacts(); 
void displayContacts(); 
void displayAll(); 
int checkPhone(char *pNum); 
 
char const *FILENAME = "sheehan_contacts"; 
char const *TEMPFILENAME = "sheehan_temp_contacts"; 
 
typedef struct { 
	char* fName; 
	char* lName; 
	char* pNum; 
	char* address; 
	char* email; 
} contactInfo;