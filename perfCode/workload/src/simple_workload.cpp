// simple_workload.cpp

#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <experimentDebug.h>
//#include <workload.h>	// Included in makefile?

using namespace std;

// Function prototypes
uint64_t factorial(const uint64_t& i);
void DoFactorial(const int& Number);

int main(int argc, char* argv[])
{
	int NumFactsToCalc = 10;	// Default # times to run
	int x = 10;					// Default # to calculate the factorial of

	// -------------------------------------
	// Command format:
	// 		./Workload [NumFactsToCalc] [x]
	// -------------------------------------
	if (argc > 1)
		NumFactsToCalc = atoi(argv[1]);
	if (argc > 2)
		x = atoi(argv[2]);

	// Run DoFactorial() NumFactsToCalc times
	for (int i = 0; i < NumFactsToCalc; i++)
		DoFactorial(x);

	return 0;
}

uint64_t factorial(const uint64_t& i)
{
	if (i < 2)
		// 0! and 1! equal 1
		return 1;
	else
		// Otherwise, x! = x * (x-1)!
		return i * factorial(i - 1);
}

void DoFactorial(const int& Number)
{
    LOG_DEBUG(DEBUG_EXP_OUTPUT, "\tCalculating " << Number << "! ..." << endl)
    uint64_t NumFact = factorial(Number);
    LOG_DEBUG(DEBUG_EXP_OUTPUT, "\t" << Number << "! = " << NumFact << endl)
}