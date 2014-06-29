#pragma once

#include "perfeventmapentry.h"
#include <experimentDebug.h>

using namespace std;

#define MAX_CONCURRENT_PROFILE_EVENTS 10

class PerfEventMap
{
public:
	PerfEventMap();
	~PerfEventMap();

	bool Contains(const __u64& Key_Config);
	inline int Count() const { return numEntries; }

	bool Add(const __u32& Type, const __u64& Key_Config);	// Expects entry to be constructed
	//void Remove(const __u64& Key_Config);		// Needs to account for case where group leader is removed!
												// (not handled currently)

	PerfEventMapEntry* EntryListStart();
	PerfEventMapEntry* EntryAt(const __u64& Key_Config);

	inline int GetGroupLeaderFD() { return groupLeader_fd; }

private:
	PerfEventMapEntry* events[MAX_CONCURRENT_PROFILE_EVENTS];
	int numEntries;		// numEntries will index into (1 + # of elements currently initialized)
	int groupLeader_fd;	// File Descriptor of the group leader. This allows all events to be enabled/disabled 
						// using one ioctl() call.

	bool Contains(const __u64& Key_Config, int& Index);
	void PrintMap();	// For debugging!
};