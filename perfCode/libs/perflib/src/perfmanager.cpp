#include "perfmanager.h"

using namespace Perf;

PerfManager::PerfManager() : PerfEvents(PerfEventMap())
	, resultsAreHot(false)
	, cachedResults(read_format())
{
}

PerfManager::~PerfManager()
{
}

__u64 PerfManager::GetHwCacheConfigMask(const perf_hw_cache_id& CacheId, const perf_hw_cache_op_id& CacheOpId,
										const perf_hw_cache_op_result_id& CacheOpResultId)
{	
	return (CacheId) | (CacheOpId << 8) | (CacheOpResultId << 16);
}

void PerfManager::AddEventType(const __u32& Type, const __u64& Config)
{	
	if (PerfEvents.Add(Type, Config))	
		{ LOG_DEBUG(DEBUG_VERBOSE, "newEntry added!\nnumEntries: " << PerfEvents.Count() << endl) }
	else
		cout << "Unable to add new entry to map in " << __FUNCTION__ << endl;
	
	return;
}

bool PerfManager::ResetCounter(const __u64& Config)
{
	PerfEventMapEntry* entry = PerfEvents.EntryAt(Config);
	if (entry != NULL)
	{		
		int result = ioctl(entry->fd, PERF_EVENT_IOC_RESET, 0);
		int err = errno;
		LOG_DEBUG(DEBUG_INFO, "TEMP: disable for results!! >> ioctl RESET result: " << result << endl)
		LOG_DEBUG(DEBUG_INFO, "errno: " << err << endl)
		return true;
	}
	else
	{
		cout << "PerfEvents.EntryAt(Config) returned NULL!\n";
		return false;	
	}
}

void PerfManager::ResetAllCounters()
{
	if (PerfEvents.Count() > 0)
	{		
		ioctl(PerfEvents.GetGroupLeaderFD(), PERF_EVENT_IOC_RESET, 1);
		resultsAreHot = true;
	}
	return;
}

long long PerfManager::GetEventCount(const __u64& Config)
{
	long long eventCount = -1;

	if (PerfEvents.Contains(Config))
	{
		int configIndex = PerfEvents.EntryAt(Config)->nr_index;

		if (resultsAreHot)
		{
			// Haven't cached results yet. Read them all in now.
			if (read(PerfEvents.GetGroupLeaderFD(), &cachedResults, sizeof(read_format)) == -1)
			{
				cout << "ERROR READING EVENT GROUP RESULTS!!!" << endl;
				return 0;
			}							

			resultsAreHot = false;
		}

		if (configIndex < cachedResults.nr)		
			eventCount = cachedResults.values[configIndex].value;		
	}

	return eventCount;
}