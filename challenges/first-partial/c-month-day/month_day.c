#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
    static char daytab[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},    // Normal year
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}     // Leap year
    };
    int leap, currentMonth;

    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    // printf("Is Leap year: %d\n", leap);
    if (leap == 0 && yearday == 366){
        printf("Invalid yearDay, this year is not a leap year.\n");
        exit(-1);
    }

    currentMonth = 1;
    while(yearday > daytab[leap][currentMonth]){
        yearday -= daytab[leap][currentMonth];
        currentMonth ++;
    }
    *pmonth = currentMonth;
    *pday = yearday;
}

void printDate(int pday, int pmonth, int year){
    
    char *monthsStr[] = {"Jan", "Feb", "Mar",
                        "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep",
                        "Oct", "Nov", "Dec"};

    printf("%s %d, %d\n", monthsStr[pmonth-1], pday, year);
}

int isNumber(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}


int main(int argc, char *argv[]) {
    // validations
    if (argc != 3){
        printf("expected 2 arguments, <year> <yearday>\n");
        exit(-1);
    }else{
        for (int i = 1;  i < argc; i++){
            if (isNumber(argv[i]) == 0){
                printf("Invalid arguments. Only positive integers are allowed as arguments.\n");
                exit(-1);
            }
            if(atoi(argv[2]) > 366){
                printf("Invalid argument. Yearday can't be greater than 366, <year> <yearday>.\n");
                exit(-1);
            }
        }
    }
    
    int year = atoi(argv[1]);
    int yearDay = atoi(argv[2]);
    int day = 0;
    int month = 0;
    
    month_day(year, yearDay, &month, &day);
    printDate(day, month, year);

    return 0;
}
