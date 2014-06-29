#pragma once

#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include "perfeventmap.h"

namespace Perf
{
	struct read_format
	{
		__u64 nr;
		//u64 time_enabled;
		//u64 time_running;
		struct {
			__u64 value;
			//u64 id;
		} values[MAX_CONCURRENT_PROFILE_EVENTS];
	};

	class PerfManager
	{
	public:
		PerfManager();
		~PerfManager();

		static __u64 GetHwCacheConfigMask(const perf_hw_cache_id& CacheId, const perf_hw_cache_op_id& CacheOpId,
											const perf_hw_cache_op_result_id& CacheOpResultId);

		void AddEventType(const __u32& Type, const __u64& Config);

		void StartProfilingAll();
		void StopProfilingAll();

		void ResetAllCounters();
		bool ResetCounter(const __u64& Config);
		long long GetEventCount(const __u64& Config);

	protected:
		PerfEventMap PerfEvents;
		bool resultsAreHot;		// Specifies if results have been read yet
		read_format cachedResults;
	};

	// Place inline function definitions in header file to avoid linker unresolved external error

	inline void PerfManager::StartProfilingAll()
	{
		if (PerfEvents.Count() > 0)
		{
			int result = ioctl(PerfEvents.GetGroupLeaderFD(), PERF_EVENT_IOC_ENABLE, 1);
			int err = errno;
			LOG_DEBUG(DEBUG_INFO, "TEMP: disable for results!! >> ioctl ENABLE result: " << result << endl)
			LOG_DEBUG(DEBUG_INFO, "errno: " << err << endl)			
		}
		return;
	}

	inline void PerfManager::StopProfilingAll()
	{
		if (PerfEvents.Count() > 0)
		{
			ioctl(PerfEvents.GetGroupLeaderFD(), PERF_EVENT_IOC_DISABLE, 1);

			resultsAreHot = true;			
		}
		return;
	}
}