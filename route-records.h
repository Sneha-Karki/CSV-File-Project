#ifndef ROUTE_RECORDS_H
#define ROUTE_RECORDS_H

#include <stdio.h>

typedef enum SearchType { 
    ROUTE, 
    ORIGIN, 
    DESTINATION, 
    AIRLINE 
} SearchType;

typedef struct {
    char origin[4];        // Origin airport code
    char destination[4];   // Destination airport code
    char airline[4];       // Airline code
    int passengerCounts[6]; // Passenger counts for 6 months
} RouteRecord;

// Function prototypes
RouteRecord* createRecords(FILE* fileIn);
int fillRecords(RouteRecord* r, FILE* fileIn);
int findAirlineRoute(RouteRecord* r, int length, const char* origin, const char* destination, const char* airline, int curIdx);
void searchRecords(RouteRecord* r, int length, const char* key1, const char* key2, SearchType st);
void printMenu();

#endif // ROUTERECORDS_H
