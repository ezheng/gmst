/*
 * util.h
 * Oil Collecting Vehicle Routing Problem
 * Some utilities Definition ...
 */

 // For compiling with Mersenne twister random number generator include MTWISTER 
 // compiling directive

#ifndef UTIL_H
#  define UTIL_H

#include <ctime>

#ifdef __linux__ 
	#include <unistd.h>
	#include <sys/times.h>
#endif

void randomize();

void setSeed(const unsigned int seed);

/* generates an integer i in {0,...,maxValue-1} */
unsigned int intRandom(const unsigned int maxValue);

double doubleRandom(const double maxValue);

double wallClock();

double cpuTime();

#endif /* ifndef UTIL_H */
