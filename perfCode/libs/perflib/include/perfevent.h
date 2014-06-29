// perfevent.h

#pragma once

#include <linux/perf_event.h>
#include <iostream>

using namespace std;

namespace Perf
{

	typedef perf_hw_cache_id _CACHE_ID;
	typedef perf_hw_cache_op_id _OP_ID;
	typedef perf_hw_cache_op_result_id _RESULT_ID;

	// Cache param constants

	const perf_hw_cache_id L1D_Id = PERF_COUNT_HW_CACHE_L1D;
	const perf_hw_cache_id L1I_Id = PERF_COUNT_HW_CACHE_L1I;
	const perf_hw_cache_id LL_Id = PERF_COUNT_HW_CACHE_LL;
	const perf_hw_cache_id DTLB_Id = PERF_COUNT_HW_CACHE_DTLB;
	const perf_hw_cache_id ITLB_Id = PERF_COUNT_HW_CACHE_ITLB;
	const perf_hw_cache_id BPU_Id = PERF_COUNT_HW_CACHE_BPU;
	const perf_hw_cache_id NODE_Id = PERF_COUNT_HW_CACHE_NODE;

	const perf_hw_cache_op_id Prefetch_Op = PERF_COUNT_HW_CACHE_OP_PREFETCH;
	const perf_hw_cache_op_id Read_Op = PERF_COUNT_HW_CACHE_OP_READ;
	const perf_hw_cache_op_id Write_Op = PERF_COUNT_HW_CACHE_OP_WRITE;

	const perf_hw_cache_op_result_id Access_Result = PERF_COUNT_HW_CACHE_RESULT_ACCESS;
	const perf_hw_cache_op_result_id Miss_Result = PERF_COUNT_HW_CACHE_RESULT_MISS;

	string GetCacheIdString(const _CACHE_ID& Id);
	string GetOpIdString(const _OP_ID& Id);
	string GetResultIdString(const _RESULT_ID& Id);

	struct PerfEvent
	{
		PerfEvent(const __u32& Type, const __u64& Config);
		virtual ~PerfEvent();

		// static string GetCacheIdString(const _CACHE_ID& Id);
		// static string GetOpIdString(const _OP_ID& Id);
		// static string GetResultIdString(const _RESULT_ID& Id);

		__u32 type;
		__u64 config;
	};

	struct PerfCacheEvent : public PerfEvent
	{
		PerfCacheEvent(const _CACHE_ID& CacheId, const _OP_ID& OpType, const _RESULT_ID& ResultType);
		~PerfCacheEvent();
		static __u64 GetHwCacheConfigMask(const _CACHE_ID& CacheId, const _OP_ID& CacheOpId,
											const _RESULT_ID& CacheOpResultId);
	};	
}