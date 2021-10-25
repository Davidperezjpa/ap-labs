#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define REPORT_FILE "report.txt"
#define HASHSIZE 50000

void analizeLog(char *logFile, char *reportLine);

// -------------- HashTable struct definition ---------------
struct nlist {
    struct nlist *next;
    char *name;
    char *defn;
};

// ------------- Report struct definition ---------------
struct reportLine {
    char *logType;
    char *description;
};

// declaring hashtable instance.
static struct nlist *hashtab[HASHSIZE];

// ---------------- HashTable Methods ------------------ 
// hashes string s to a positiveNumber
unsigned hash(char *s){
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++){
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

// Search inside hashtable for ocurrence of string s
struct nlist *lookup(char *s){
    struct nlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np->next){
        if (strcmp(s, np->name) == 0){
            return np;  // Found!
        }
        return NULL;     // Not Found
    }
}

// inserts or replaces new value to hash table
struct nlist *install(char *name, char *defn){
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL){ //Not found, inserting new
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL){
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;

    } else{ // record found, replacing.
        free((void *) np->defn);   // free previous defn
    }
    if ((np->defn = strdup(defn)) == NULL){
        return NULL;
    }
    return np;
}


// Split by category and set it in our reportLine structure.
struct reportLine categorizeLine(char *txtLine){
    
    int endIndex;
    char* logType;
    struct reportLine reportInstance;
    char* description;
    char* startType = strchr(txtLine, ']');
    
    description = strdup(txtLine);
    if (startType == 0){
        logType = "General";
        reportInstance.logType = logType;
        reportInstance.description = description;
        return reportInstance;
    }
    char* endType = strchr(startType, ':');

    if(startType && endType) {
        endIndex = (int)(endType - startType);
        logType = strndup(startType + 2, endIndex - 1);
    } else{
        logType = "General";
    }

    // printf("Description: %s\n", description);
    // printf("LogType: %s\n", logType);

    reportInstance.logType = logType;
    reportInstance.description = description;
    return reportInstance;
}

// Generating new file and ordering log by category.
void createReport(char* report){
    FILE *finalFile = fopen(report, "w+");
    struct nlist *head, *ptr;
    for(int i = 0; i <= HASHSIZE; i++){
        head = hashtab[i];
        if(head){
            fputs(head->name, finalFile);  // Category
            fputs("\n", finalFile);
            for(ptr = head; ptr != NULL; ptr = ptr->next){
                fputs(ptr->defn, finalFile);   // Description
                // printf("%s", ptr->defn);
            }
            fputs("\n", finalFile);
        }else{
            continue;
        }
    }
    fclose(finalFile);
}



void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    // Implement your solution here.

    // Opening File
    int fileResult = open(logFile, O_RDONLY);
    if(fileResult == -1){
        printf("Error opening file.\n");
        exit(-1);
    }
    
    // go through the file, categorize and inserting in hash table.
    char buffer;
    int fileSize = 0;
    int readingNum;

    lseek(fileResult,0,SEEK_SET);
    int charsInLine = 0;
    readingNum = 1;

    while(readingNum > 0){
        readingNum = read(fileResult, &buffer, 1);
        // printf("%c",buffer);
        if (readingNum){
            if(buffer == '\n'){
                charsInLine++;
                
                char* lineStr = (char*) malloc(charsInLine * sizeof(char));
                lseek(fileResult, -charsInLine, SEEK_CUR);
                read(fileResult, lineStr, charsInLine);

                if ((lineStr != NULL) && (lineStr[0] == '\0')){
                    // Empty line, skipping.
                    printf("Empty Line!\n");
                }else{
                    // printf("%s\n", lineStr);
                    struct reportLine reportLog = categorizeLine(lineStr);
                    // printf("logType: %s\n Description: %s", reportLog.logType, reportLog.description);
                    install(reportLog.logType, reportLog.description);
                }
                charsInLine = 0;
            }else{
                charsInLine++;
            }
        }
    }
    close(fileResult);
    
    createReport(report);
    printf("Report is generated at: [%s]\n", report);
}


//  ------------------ MAIN ------------------------
int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage:./dmesg-analizer logfile.txt\n");
        return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}
