#pragma once

#include <linux/perf_event.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <asm/unistd.h>
#include <iostream>
#include <experimentDebug.h>

using namespace std;

struct PerfEventMapEntry
{
	__u64 key_config;		// Use the config variable as the key into the map
	perf_event_attr* value;
	int fd;
	bool openSuccessful;
	int nr_index;		// Used to index into the values[nr] array to get results for this entry

	PerfEventMapEntry();
	~PerfEventMapEntry();
	PerfEventMapEntry(const __u32& Type, const __u64& Config, const int& GroupLeaderFD = -1);

	// To copy, simply copy the pointer and value (MUST BE "new-ed" ALREADY!!)
	PerfEventMapEntry(const PerfEventMapEntry& Other);
	PerfEventMapEntry& operator=(const PerfEventMapEntry& rhs);

	bool OpenPerfFD(const bool& IsGroupLeader, int& errorCode, const int& GroupLeaderFD = -1);
};