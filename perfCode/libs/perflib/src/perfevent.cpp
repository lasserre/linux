#include "perfevent.h"

namespace Perf {

PerfEvent::PerfEvent(const __u32& Type, const __u64& Config) : type(Type)
	, config(Config)
{}

PerfEvent::~PerfEvent()
{
}

__u64 PerfCacheEvent::GetHwCacheConfigMask(const _CACHE_ID& CacheId, const _OP_ID& CacheOpId,
										const _RESULT_ID& CacheOpResultId)
{
	return (CacheId) | (CacheOpId << 8) | (CacheOpResultId << 16);
}

PerfCacheEvent::PerfCacheEvent(const _CACHE_ID& CacheId, const _OP_ID& OpType, const _RESULT_ID& ResultType) : 
PerfEvent(PERF_TYPE_HW_CACHE, GetHwCacheConfigMask(CacheId, OpType, ResultType))
{}

PerfCacheEvent::~PerfCacheEvent()
{
}

string GetCacheIdString(const _CACHE_ID& Id)
{
	switch (Id)
	{
	case L1D_Id:	
		return "L1D_Id";
	case L1I_Id:
		return "L1I_Id";
	case LL_Id:
		return "LL_Id";
	case DTLB_Id:
		return "DTLB_Id";
	case ITLB_Id:
		return "ITLB_Id";
	case BPU_Id :
		return "BPU_Id";
	case NODE_Id:
		return "NODE_Id";
	default:
		return "Unknown cache Id";
	}
}

string GetOpIdString(const _OP_ID& Id)
{
	switch (Id)
	{
	case Prefetch_Op:
		return "Prefetch_Op";
	case Read_Op:
		return "Read_Op";
	case Write_Op:
		return "Write_Op";
	default:
		return "Unknown op Id";
	}
}

string GetResultIdString(const _RESULT_ID& Id)
{
	switch (Id)
	{
	case Access_Result:
		return "Access_Result";
	case Miss_Result:
		return "Miss_Result";
	default:
		return "Unknown result Id";
	}
}

}