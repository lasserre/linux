// migrator.h

#pragma once

#include <sched.h>
#include <iostream>

#define DEFAULT_NUMMIGRATIONS -1
#define DEFAULT_MODE TRANS_CORE

#define TOTAL_NUM_CORES 8			// 8 for server machine. Change for other machines!!

enum MIGRATE_MODE
{
	UNSPECIFIED,
	TRANS_CORE,		// Core-to-core mode
	TRANS_PROC		// Processor-to-processor mode
};

struct MigrationOptions
{
	int numMigrations;
	MIGRATE_MODE mode;

	/* ----- Experiment defaults set here ----- */
	MigrationOptions() : numMigrations(DEFAULT_NUMMIGRATIONS)
	, mode(DEFAULT_MODE)
	{}

	MigrationOptions(const MigrationOptions& other) : numMigrations(other.numMigrations)
	, mode(other.mode)
	{}
};

/**
 * @brief The Migrator class contains methods to figure out which processor ID we need to migrate to next, based
 * on our mode and current processor.
 */
class Migrator
{
public:	
	Migrator(const MigrationOptions& Options);
	~Migrator();

	/**
	 * @brief Migrate() attempts to migrate the calling process to the next core by calling sched_setaffinity()
	 * @return true if successful, false if done
	 */
	bool Migrate();

	/**
	 * @brief Gets the id of the processor we are currently running on	 	 
	 */
	static int GetCurrentProcessorId() { return sched_getcpu(); }

	/**
	 * @brief Gets the id of the next processor we will run on, when we call Migrate()	 	 
	 */
	inline int GetNextProcessorId() { return nextProcessorId; }

protected:
	MigrationOptions options;
	int migrationsAttempted;	
	int nextProcessorId;
	cpu_set_t* cpuSet;

	/* ----- Protected methods ----- */
	void Advance_nextProcessorId();
};