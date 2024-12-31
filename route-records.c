#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "route-records.h"

// Create records and allocate memory for RouteRecord array
RouteRecord* createRecords(FILE* fileIn) {
    int recordCount = 0;
    char line[256];

    // Count lines (excluding header)
    while (fgets(line, sizeof(line), fileIn) != NULL) {
        if (recordCount == 0) { // Skip header
            recordCount++;
            continue;
        }
        recordCount++;
    }

    // Allocate memory for RouteRecord array
    RouteRecord* records = (RouteRecord*)malloc(recordCount * sizeof(RouteRecord));
    if (!records) {
        fprintf(stderr, "ERROR: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize passenger counts to 0
    for (int i = 0; i < recordCount; i++) {
        memset(records[i].passengerCounts, 0, sizeof(records[i].passengerCounts));
    }

    rewind(fileIn);  // Rewind file pointer
    return records;
}

// Fill records from CSV file into RouteRecord array
int fillRecords(RouteRecord* r, FILE* fileIn) {
    char line[256], origin[4], destination[4], airline[4];
    int month, passengers, currentIndex = 0;

    fgets(line, sizeof(line), fileIn);  // Skip header

    while (fgets(line, sizeof(line), fileIn) != NULL) {
        // Debugging: Print the line being read
        printf("Reading line: %s", line);

        // Parse the comma-separated line
        if (sscanf(line, "%d,%3s,%3s,%3s,%d", &month, origin, destination, airline, &passengers) != 5) {
            printf("Error parsing line: %s", line);  // Log parsing error
            continue; // Skip invalid lines
        }

        // Debugging: Print parsed values
        printf("Parsed: Month: %d, Origin: %s, Destination: %s, Airline: %s, Passengers: %d\n",
               month, origin, destination, airline, passengers);

        // Check if route already exists
        int idx = findAirlineRoute(r, currentIndex, origin, destination, airline, 0);
        if (idx == -1) {
            // New record
            strcpy(r[currentIndex].origin, origin);
            strcpy(r[currentIndex].destination, destination);
            strcpy(r[currentIndex].airline, airline);
            memset(r[currentIndex].passengerCounts, 0, sizeof(r[currentIndex].passengerCounts)); // Initialize to 0
            r[currentIndex].passengerCounts[month - 1] = passengers;
            currentIndex++;
        } else {
            // Update existing record
            r[idx].passengerCounts[month - 1] += passengers;
        }
    }

    return currentIndex;  // Return the number of unique routes
}

// Recursive function to find an airline route in the array
int findAirlineRoute(RouteRecord* r, int length, const char* origin, const char* destination, const char* airline, int curIdx) {
    if (curIdx >= length) return -1;

    if (strcmp(r[curIdx].origin, origin) == 0 &&
        strcmp(r[curIdx].destination, destination) == 0 &&
        strcmp(r[curIdx].airline, airline) == 0) {
        return curIdx;
    }

    return findAirlineRoute(r, length, origin, destination, airline, curIdx + 1);
}

// Search records based on keys and SearchType
void searchRecords(RouteRecord* r, int length, const char* key1, const char* key2, SearchType st) {
    int totalPassengers = 0, monthlyTotals[6] = {0}, matches = 0;

    printf("Searching by ");
    if (st == ROUTE) printf("route...\n");
    else if (st == ORIGIN) printf("origin...\n");
    else if (st == DESTINATION) printf("destination...\n");
    else if (st == AIRLINE) printf("airline...\n");

    for (int i = 0; i < length; i++) {
        int match = 0;

        // Check for match based on SearchType
        if ((st == ORIGIN && strcmp(r[i].origin, key1) == 0) ||
            (st == DESTINATION && strcmp(r[i].destination, key1) == 0) ||
            (st == AIRLINE && strcmp(r[i].airline, key1) == 0) ||
            (st == ROUTE && strcmp(r[i].origin, key1) == 0 && strcmp(r[i].destination, key2) == 0)) {
            match = 1;
        }

        if (match) {
            matches++;
            printf("%s (%s-%s)\n", r[i].airline, r[i].origin, r[i].destination);

            // Calculate statistics
            for (int j = 0; j < 6; j++) {
                totalPassengers += r[i].passengerCounts[j];
                monthlyTotals[j] += r[i].passengerCounts[j];
            }
        }
    }

    if (matches == 0) {
        printf("No matches found.\n");
        return;
    }

    printf("%d matches were found.\n", matches);
    printf("Statistics\n");
    printf("Total Passengers: %d\n", totalPassengers);

    for (int i = 0; i < 6; i++) {
        printf("Total Passengers in Month %d: %d\n", i + 1, monthlyTotals[i]);
    }

    printf("Average Passengers per Month: %d\n", totalPassengers / 6);
}



// Print the menu for user interaction
void printMenu() {
    printf("\n\n######### Airline Route Records Database MENU #########\n");
    printf("1. Search by Route\n");
    printf("2. Search by Origin Airport\n");
    printf("3. Search by Destination Airport\n");
    printf("4. Search by Airline\n");
    printf("5. Quit\n");
    printf("Enter your selection: ");
}
