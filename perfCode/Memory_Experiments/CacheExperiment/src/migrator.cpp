#include "migrator.h"

Migrator::Migrator(const MigrationOptions& Options) : options(Options)
	, migrationsAttempted(0)
	, nextProcessorId(GetCurrentProcessorId())	// Initialize to current id
	, cpuSet(new cpu_set_t())
{
	Advance_nextProcessorId();		// Calculate id for first migration
}

Migrator::~Migrator()
{
	delete cpuSet;
}

bool Migrator::Migrate()
{
	if (migrationsAttempted >= options.numMigrations)
		return false;

	// Reset our CPU set, then add the target core id
	CPU_ZERO(cpuSet);
	CPU_SET(nextProcessorId, cpuSet);

	// Migrate!
	sched_setaffinity(0, sizeof(cpuSet), cpuSet);

	// Prepare for next migration
	Advance_nextProcessorId();
	migrationsAttempted++;

	//std::cout << "\n >>>>>>>>> Migration #" << migrationsAttempted << " Attempt...";

	return true;
}

/** 		
 * @details This function assumes that the numbering of processors will be the same as on the SERVER machine!!
 * TODO: Change logic to account for different processor numbering schemes (and single-processor systems)
 */
void Migrator::Advance_nextProcessorId()
{
	switch (options.mode)
	{
	case TRANS_CORE:
		// Keep it jumping from core to core on a single processor
		nextProcessorId = (nextProcessorId + 2) % TOTAL_NUM_CORES;
		break;

	case TRANS_PROC:
		// Keep it jumping back and forth between processors
		nextProcessorId = (++nextProcessorId) % TOTAL_NUM_CORES;
		break;

	default:
		std::cout << "Unspecified migration mode! Ending experiment.\n";
		migrationsAttempted = options.numMigrations;	// Experiment will not be able to migrate further
	}
	return;
}