/**
 * Utilities.
 */
#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Stuff that we shouldn't have to define.
#define loop for(;;)
typedef char * Filepath;
void unimplemented(void);

// Comparisons. A <= B.
bool lexiographicCompare(void * ptrA, void * ptrB);
bool numericCompare(void * dataA, void * dataB); // we can use user-data to store values

// Random.
void initRandomSeed();
int randomIntRange(int low, int high);
void randomWordFixed(size_t length, char *);
void randomWord(char *);

// Cross-platform timing? TODO
timer_t getTime();

#endif // UTIL_H_INCLUDED
