#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "route-records.h"

int main(int argc, char *argv[]) {
    FILE *fileIn;
    RouteRecord *records = NULL;
    int actualRecords = 0;
    int menuOption;
    char key1[4], key2[4];

    if (argc < 2) {
        printf("ERROR: Missing file name\n");
        return EXIT_FAILURE;
    }

    printf("Opening %s...\n", argv[1]);
    fileIn = fopen(argv[1], "r");

    /* 4. Check to see if the file opens. If it does not open, 
       print out ERROR: Could not open file and end the program. */
    if (!fileIn) {
        printf("ERROR: Could not open file\n");
        return EXIT_FAILURE;
    }

    records = createRecords(fileIn);
    actualRecords = fillRecords(records, fileIn);
    printf("Unique routes operated by airlines: %d\n", actualRecords);
    fclose(fileIn);

    while (1) {
        printMenu();
        if (scanf("%d", &menuOption) != 1) {
            printf("Invalid input. Please enter a numeric value.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }

        switch (menuOption) {
            case 1:
                printf("Enter origin: ");
                scanf("%3s", key1);
                printf("Enter destination: ");
                scanf("%3s", key2);
                searchRecords(records, actualRecords, key1, key2, ROUTE);
                break;

            case 2:
                printf("Enter origin: ");
                scanf("%3s", key1);
                searchRecords(records, actualRecords, key1, NULL, ORIGIN);
                break;

            case 3:
                printf("Enter destination: ");
                scanf("%3s", key1);
                searchRecords(records, actualRecords, key1, NULL, DESTINATION);
                break;

            case 4:
                printf("Enter airline: ");
                scanf("%3s", key1);
                searchRecords(records, actualRecords, key1, NULL, AIRLINE);
                break;

            case 5:
                printf("Good-bye!\n");
                free(records);
                return EXIT_SUCCESS;

            default:
                printf("Invalid option. Please select a number between 1 and 5.\n");
                break;
        }
    }

    return 0;
}
