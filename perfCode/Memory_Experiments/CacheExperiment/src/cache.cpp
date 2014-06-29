//#define _GNU_SOURCE 
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <iomanip>
#include <climits>
#include <sstream>
#include "massiveArray.h" 
#include "migrator.h"
#include "perfmanager.h"
#include "workload.h"
#include "perfevent.h"
#include <logger/logger.h>

using namespace std;

struct ExperimentOptions
{
	MigrationOptions migrationConfigs;
	string outputFilename;
	ExperimentOptions() : migrationConfigs(MigrationOptions())
		, outputFilename("cacheExperimentResults.csv")
	{}
};

/* /////////////////////////////////////////////////////////////////////// */
/* ------------------------ Experiment Parameters ------------------------ */
/* /////////////////////////////////////////////////////////////////////// */

// Note: I used the #define technique below for parameters (for now) because project won't compile if you change
// # of enumerated array items and forget to make sure the MACRO matches # of elements you listed in the array

LoggingMode loggingMode = CsvModeOnly;

//////// SPECIFY DISTINCT CACHE_IDS ////////

#define NUM_DISTINCT_CACHE_IDS 1 		

Perf::_CACHE_ID PEParams_Caches[NUM_DISTINCT_CACHE_IDS] = {
	Perf::L1D_Id
};

//////// SPECIFY DISTINCT CACHE_IDS ////////

#define NUM_DISTINCT_OP_IDS 1

Perf::_OP_ID PEParams_Ops[NUM_DISTINCT_OP_IDS] = {
	Perf::Read_Op
};

//////// SPECIFY DISTINCT CACHE_IDS ////////

#define NUM_DISTINCT_RESULT_IDS 2

Perf::_RESULT_ID PEParams_Results[NUM_DISTINCT_RESULT_IDS] = {		
	Perf::Access_Result,
	Perf::Miss_Result
};

/* /////////////////////////////////////////////////////////////////////// */
/* /////////////////////////////////////////////////////////////////////// */
/* /////////////////////////////////////////////////////////////////////// */

// Generate every possible combination of events with given parameters
#define NUM_EVENTS NUM_DISTINCT_CACHE_IDS * NUM_DISTINCT_OP_IDS * NUM_DISTINCT_RESULT_IDS

Perf::PerfEvent* Events[NUM_EVENTS];

string OptionStrings[NUM_EVENTS] = {""};
string OptionSummaryString = "";

/* ///////////////////////////// Functions //////////////////////////////// */

void InitializePerfEvents()
{
	for (int cacheIdx = 0; cacheIdx < NUM_DISTINCT_CACHE_IDS; cacheIdx++)
	{
		for (int opIdx = 0; opIdx < NUM_DISTINCT_OP_IDS; opIdx++)
		{
			for (int resultIdx = 0; resultIdx < NUM_DISTINCT_RESULT_IDS; resultIdx++)
			{
				// Place-value notation (with variable bases lol)...
				// listed from most significant place to least significant place
				// Analagous to (x2 * 10^2) + (x1 * 10^1) + (x0 * 10^0)
				int index = 
					(cacheIdx * NUM_DISTINCT_OP_IDS) + (opIdx * NUM_DISTINCT_RESULT_IDS) + (resultIdx * 1);

				Events[index] =
					new Perf::PerfCacheEvent(	PEParams_Caches[cacheIdx],
												PEParams_Ops[opIdx],
												PEParams_Results[resultIdx]
											);

				OptionSummaryString += "\t";

				OptionStrings[index] += Perf::GetCacheIdString(PEParams_Caches[cacheIdx]) + " ";
				OptionStrings[index] += Perf::GetOpIdString(PEParams_Ops[opIdx]) + " ";
				OptionStrings[index] += Perf::GetResultIdString(PEParams_Results[resultIdx]);

				OptionSummaryString += OptionStrings[index] + "\n";
			}
		}
	}

	return;
}

void CleanupPerfEvents()
{
	for (int i = 0; i < NUM_EVENTS; i++)
		delete Events[i];
	return;
}

string NumToStr(const int& num)
{
	stringstream ss;
	ss << num;
	return ss.str();
}

string NumToStr(const long& num)
{
	stringstream ss;
	ss << num;
	return ss.str();
}

string NumToStr(const long long& num)
{
	stringstream ss;
	ss << num;
	return ss.str();
}

string NumToStr(const double& num)
{
	stringstream ss;
	ss << setprecision(15) << num;
	//cout << "ss.str(): " << ss.str() << endl;	
	return ss.str();
}

void PrintCurrentCPU(const string& Prefix)
{	
	int cpuNum = sched_getcpu();
	string msg(Prefix + " we are running on cpu #");
	msg += NumToStr(cpuNum);	
	Logger::GetInstance()->LogLine_Run(msg);
	return;
}

void PrintBold(const string& OutputString)
{
	static const char ESC = 27;

	printf("%c[1m", ESC);
	cout << OutputString;
	printf("%c[0m", ESC);

	return;
}

void PrintExperimentStartMessage(const ExperimentOptions& Options)
{
	bool printBold = true;
	Logger::GetInstance()->LogLine_Run(
		"\n------------------------- EXPERIMENT START -------------------------", printBold);
	Logger::GetInstance()->LogLine_Run("Options:", printBold);
	
	static string msg = "\tNumber of Migrations - " + NumToStr(Options.migrationConfigs.numMigrations);
	Logger::GetInstance()->LogLine_Run(msg);

	Logger::GetInstance()->Log_Run("\tOutput filename: ");
	Logger::GetInstance()->LogLine_Run(Options.outputFilename);

	Logger::GetInstance()->Log_Run("\tMode - ");
	switch (Options.migrationConfigs.mode)
	{
	case TRANS_CORE:
		Logger::GetInstance()->LogLine_Run("core-to-core");
		break;
	case TRANS_PROC:
		Logger::GetInstance()->LogLine_Run("processor-to-processor");
		break;
	default:
		Logger::GetInstance()->LogLine_Run("unspecified mode!");
	}

	

	msg = "\tDEBUG_LEVEL - " + NumToStr(DEBUG_LEVEL);
	Logger::GetInstance()->LogLine_Run(msg);

	msg = "\tTotal # of Events - " + NumToStr(NUM_EVENTS);
	Logger::GetInstance()->LogLine_Run(msg);

	Logger::GetInstance()->LogLine_Run("\nMonitored Events:", printBold);
	Logger::GetInstance()->Log_Run(OptionSummaryString, printBold);

	Logger::GetInstance()->LogLine_Run("--------------------------------------------------", printBold);

	PrintCurrentCPU("At experiment start,");

	Logger::GetInstance()->LogLine_Run("");

	return;
}

void PrintExperimentEndMessage()
{
	Logger::GetInstance()->LogLine_Run(
		"------------------------- EXPERIMENT END -------------------------\n", true);
	return;
}

MIGRATE_MODE GetExperimentModeFromArg(char* mode)
{
	if (*mode == 'c')
		return TRANS_CORE;
	else if (*mode == 'p')
		return TRANS_PROC;
	else
		return UNSPECIFIED;
}

string GetFilenameFromArg(char* filename)
{
	string filenameStr = filename;
	if (filenameStr.find('.') == string::npos)
		filenameStr += ".csv";
	return filenameStr;
}

void PrintCacheExperimentManPage(const string& Msg, const string& EnteredCommandName, const string& ProperCommand)
{
	PrintBold("\n ////////// CACHE MEMORY EXPERIMENT //////////\n\n");
	cout << ">>>>> MESSAGE: ";
	cout << Msg << " <<<<<\n\n";
    PrintBold(" ---------- PROPER COMMAND USAGE ----------\n\n");                
    
    cout << ProperCommand;

    PrintBold("\n\n ---------- PARAMETERS ----------\n");
    PrintBold("\nNUM_MIGRATIONS:\n");
    cout << "\tIntegral # of forced migrations requested\n";
    PrintBold("\n[OUTPUT FILENAME]:\n");
    cout << "\tName of file to output. If no extension is specified, .csv will be appended.\n";
    cout << "\t(default = cacheExperimentResults[.csv])\n";
    PrintBold("\n[MODE]:\n");
    cout << "\tc - Specifies to perform a core-to-core migration (default)\n";
    cout << "\tp - Specifies to perform a processor-to-processor migration\n";

    PrintBold("\nExample:\n");
    PrintBold("\t" + EnteredCommandName + " 2 c");
    cout << " - Run experiment with 2 forced core-to-core migrations\n\n";
	return;
}

bool ParseCommandLineArgs(const int& argc, char* argv[], ExperimentOptions& Options)
{
	// Format: [command] NUM_MIGRATIONS [MODE]
	char* command_arg = argv[0];
	char* num_migrations_arg = argv[1];
	char* arg2 = argv[2];
	char* arg3 = argv[3];
	string command = command_arg;
	string properCommand = "\"" + command + " NUM_MIGRATIONS [OUTPUT FILENAME] [MODE]\"";
	
	switch (argc)
	{
	case 1:		// Not enough args specified! Print our "man page"
		PrintCacheExperimentManPage("Not enough arguments!", command, properCommand);
        return false;

    case 2:		// No FILENAME or MODE specified. Use default: core-to-core (already set)    	

    	Options.migrationConfigs.numMigrations = atoi(num_migrations_arg);
    	break;

    case 3:		// NUM_MIGRATIONS and 1 optional parameter specified.
    	Options.migrationConfigs.numMigrations = atoi(num_migrations_arg);
    	Options.migrationConfigs.mode = GetExperimentModeFromArg(arg2);
    	if (Options.migrationConfigs.mode == UNSPECIFIED)
    	{
    		// No mode specified...must be a filename!
    		Options.outputFilename = GetFilenameFromArg(arg2);
    		Options.migrationConfigs.mode = TRANS_CORE;
    	}
    	break;

    default:	// All options present. Read options to set up experiment    	

    	Options.migrationConfigs.numMigrations = atoi(num_migrations_arg);
    	Options.outputFilename = GetFilenameFromArg(arg2);
    	Options.migrationConfigs.mode = GetExperimentModeFromArg(arg3);
    	switch (Options.migrationConfigs.mode)
    	{
		case TRANS_CORE:
		case TRANS_PROC:	// Correctly parsed
			break;
		case UNSPECIFIED:	// Incorrectly parsed			
		default:
			PrintCacheExperimentManPage("Unrecognized arguments (wrong order?)", command, properCommand);
			return false;
    	}
    }

	return true;
}

bool CheckCorrectMigration(const ExperimentOptions& Options)
{
	bool result;
	static int currentCPU = -1;
	static int lastCPU = -1;

	currentCPU = Migrator::GetCurrentProcessorId();

	if (lastCPU != -1)	// Not the first time through...
	{
		// Migration attempt has just occured! Check to make sure we moved...

		if (currentCPU == lastCPU)	// We haven't moved anywhere...return false
			return false;
		
		// Check that we moved to the correct one.. (a core away, or a processor away)
		switch (Options.migrationConfigs.mode)
		{
		case TRANS_CORE:		
			if (((currentCPU + lastCPU) % 2) == 0)	// Even + Even => Even
				result = true;
			else
				result = false;
			break;

		case TRANS_PROC:		
			if (((currentCPU + lastCPU) % 2) == 1)	// Even + Odd => Odd
				result = true;
			else
				result = false;
			break;

		default:
			break;
		}
	}
	else 		
	{		// First time through (return true)
		result = true;
	}

	lastCPU = currentCPU; 	// Save this CPU for next time...

	return result;
}


int main( int argc, char *argv[])
{
	const int AggCoreStatsFileIndex = 1;
	const int AggProcStatsFileIndex = 2;
	// Variables for statistics
	const int MIN_INDEX = 0;
	const int MAX_INDEX = 1;
	const int AVG_INDEX = 2;
	double runStats[NUM_EVENTS][3] = {0};

	// Initialize runStats[][MIN_INDEX] to be largest possible int
	for (int i = 0; i < NUM_EVENTS; i++)	
		runStats[i][MIN_INDEX] = INT_MAX;	

	/////////////////////////////
	int loopCounter = 0;

	ExperimentOptions options;	// Defaults set in default constructor
	Logger* logger = Logger::GetInstance();	
	int unsuccessfulMigrations = 0;

	/* ----- Read command-line arguments and create the experiment ----- */
	if (!ParseCommandLineArgs(argc, argv, options))
		return 0;
	
	// Open main output file
	logger->SetMode(loggingMode, options.outputFilename);
	
	// Open appropriate aggregate stats file
	bool appendMode = true;
	if (options.migrationConfigs.mode == TRANS_CORE)
		logger->SetFileX(AggCoreStatsFileIndex, "core2core_AggregateStats.csv", appendMode);
	if (options.migrationConfigs.mode == TRANS_PROC)
		logger->SetFileX(AggProcStatsFileIndex, "proc2proc_AggregateStats.csv", appendMode);

	InitializePerfEvents();	

	// Construct migrator with experiment options
	Migrator migrator(options.migrationConfigs);
	
	PrintExperimentStartMessage(options);

	Perf::PerfManager manager;
	for (int i = 0; i < NUM_EVENTS; i++)
		manager.AddEventType(Events[i]->type, Events[i]->config);

	/* ------------------------------------------------------------- */

	// Print out what we're measuring
	for (int i = 0; i < NUM_EVENTS; i++)
		{
			if (i == (NUM_EVENTS-1))
				logger->LogLine_Csv(OptionStrings[i]);
			else
				logger->Log_Csv(OptionStrings[i]);
		}

	// Run experiment	
	
	do
	{		
		if (CheckCorrectMigration(options))
		{
			if (loopCounter++ > 0)
				logger->LogLine_Run("migration successful!");
		}
		else
		{
			logger->LogLine_Run("MIGRATION UNSUCCESSFUL!!", true);	// true -> bold text
			unsuccessfulMigrations++;
		}

		manager.GetEventCount(Events[0]->config);
		
		manager.ResetAllCounters();

		// Make sure we start at zero
		
		long zeroCount = manager.GetEventCount(Events[0]->config);
		if (zeroCount != 0)
		{
			logger->Log_Run("\n\tJust reset counters, but the value of first counter is: ");
			logger->LogLine_Run(NumToStr(zeroCount));
		}

		manager.StartProfilingAll();

		//////////// Begin profiling section ////////////
		string msg;	
		
		Workload::DoWork();		
		
		PrintCurrentCPU("\tDoWork() has completed.");
		
		//////////// End profiling section ////////////

		manager.StopProfilingAll();

		for (int i = 0; i < NUM_EVENTS; i++)
		{
			int countValue = manager.GetEventCount(Events[i]->config);
			
			// Log to experiment file
			if (i == (NUM_EVENTS-1))				
				logger->LogLine_Csv(NumToStr(countValue));
			else				
				logger->Log_Csv(NumToStr(countValue));

			// Update stats
			if (loopCounter == 0)
			{
				runStats[i][MAX_INDEX] = countValue;
				runStats[i][MIN_INDEX] = countValue;
				runStats[i][AVG_INDEX] = countValue;
			}
			else
			{
				if (countValue > runStats[i][MAX_INDEX])
					runStats[i][MAX_INDEX] = countValue;
				if (countValue < runStats[i][MIN_INDEX])
					runStats[i][MIN_INDEX] = countValue;
				
				double currAvgWeighted = (runStats[i][AVG_INDEX] * ((double)(loopCounter-1)/(double)loopCounter));
				double currNumWeighted = (double)countValue * ((double)1 / (double)loopCounter);

				runStats[i][AVG_INDEX] = currAvgWeighted + currNumWeighted;
			}
		}		
		
	} while (migrator.Migrate());

	// Show error message if we weren't actually able to migrate where we wanted to
	if (unsuccessfulMigrations > 0)
		PrintBold("ERROR: There were " + NumToStr(unsuccessfulMigrations) + " unsuccessful Migrations!\n");

	int aggOutIndex;

	if (options.migrationConfigs.mode == TRANS_CORE)
		aggOutIndex = AggCoreStatsFileIndex;
	if (options.migrationConfigs.mode == TRANS_PROC)
		aggOutIndex = AggProcStatsFileIndex;

	for (int i = 0; i < NUM_EVENTS; i++)
	{			
		logger->Log_Csv(NumToStr(runStats[i][MIN_INDEX]), aggOutIndex);
		logger->Log_Csv(NumToStr(runStats[i][MAX_INDEX]), aggOutIndex);
		logger->Log_Csv(NumToStr(runStats[i][AVG_INDEX]), aggOutIndex);
	}
	logger->LogLine_Csv("", aggOutIndex);

	PrintExperimentEndMessage();
	LOG_DEBUG(DEBUG_INFO, "here")
	CleanupPerfEvents();
	
	return 0;
}
