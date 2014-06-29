#include "perfeventmap.h"

PerfEventMap::PerfEventMap() : numEntries(0)
	, groupLeader_fd(-1)
{}

PerfEventMap::~PerfEventMap()
{	
	for (int i = 0; i < numEntries; i++)		// Any remaining items memory is deleted here
	 	delete events[i];
}

// Public wrapper (w/o index required)
bool PerfEventMap::Contains(const __u64& Key_Config)
{
	int index;

	if (Contains(Key_Config, index))
		return true;
	else
		return false;
}

bool PerfEventMap::Contains(const __u64& Key_Config, int& Index)
{		
	LOG_DEBUG(DEBUG_VERBOSE, "In " << __FUNCTION__ << ", printing map contents..." << endl)	
	LOG_DEBUG(DEBUG_VERBOSE, "Looking for Key_Config: " << Key_Config << endl)
		
	EXECUTE_IF_DEBUG(DEBUG_VERBOSE, PrintMap())

	for (int i = 0; i < numEntries; i++)
	{
		if (events[i]->key_config == Key_Config)
		{
			Index = i;
			return true;
		}
	}

	return false;
}

bool PerfEventMap::Add(const __u32& Type, const __u64& Key_Config)
{
	if (this->Contains(Key_Config))
		return false;

	if (this->numEntries < MAX_CONCURRENT_PROFILE_EVENTS)
	{
		// groupLeader_fd defaults to -1, so even if we haven't set it yet, we're good!
		events[numEntries] = new PerfEventMapEntry(Type, Key_Config, groupLeader_fd);

		if (this->numEntries == 0)
		{
			// Just added the first item (group leader)! 
			// Store the group leader's file descriptor so all subsequent events are in same group			

			this->groupLeader_fd = events[numEntries]->fd;
		}

		events[numEntries]->nr_index = numEntries++;
				
		LOG_DEBUG(DEBUG_VERBOSE, "Key_Config: " << Key_Config << endl)
		return true;
	}
	else
		return false;
}

// void PerfEventMap::Remove(const __u64& Key_Config)
// {
// 	int index;

// 	if (this->Contains(Key_Config, index))
// 	{
// 		delete events[index];			// Memory deleted on Remove()
// 		numEntries--;
// 	}
// 	return;
// }

PerfEventMapEntry* PerfEventMap::EntryListStart()
{	
	return events[0];
}

PerfEventMapEntry* PerfEventMap::EntryAt(const __u64& Key_Config)
{
	int index;
	if (this->Contains(Key_Config, index))
		return events[index];
	else
		return NULL;
}

void PerfEventMap::PrintMap()
{
	cout << "----------->>> Printing map of size: " << numEntries;
	cout << " <<<-----------" << endl;
	for (int i = 0; i < numEntries; i++)
	{
		cout << "Entry #" << i << ":" << endl;
		cout << "\tkey_config: " << events[i]->key_config << endl;
		cout << "\tfd: " << events[i]->fd << endl;
		cout << "\tvalue: " << events[i]->value << endl;
		cout << "\topenSuccessful: " << events[i]->openSuccessful << endl;
	}
	cout << "-----------------------------------------------------------" << endl;
	return;
}