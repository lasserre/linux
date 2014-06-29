#include "perfeventmapentry.h"

static int constructorStackPointer = 0;

PerfEventMapEntry::PerfEventMapEntry() : key_config(0)
	, value(NULL)
	, fd(0)
	, openSuccessful(false)
	, nr_index(-1)
{
	LOG_DEBUG(DEBUG_VERBOSE, ++constructorStackPointer << endl)
}

PerfEventMapEntry::~PerfEventMapEntry()
{
	LOG_DEBUG(DEBUG_VERBOSE, --constructorStackPointer << endl)
	
	if (this->value != NULL)
		delete this->value;			

	if (openSuccessful)
		close(fd);
}

PerfEventMapEntry::PerfEventMapEntry(const __u32& Type, const __u64& Config, const int& GroupLeaderFD /*= -1*/) 
	: key_config(Config)
	, value(new perf_event_attr())
	, fd(0)
	, openSuccessful(false)
	, nr_index(-1)
{
	LOG_DEBUG(DEBUG_VERBOSE, ++constructorStackPointer << endl)
		
	value->type = Type;
	value->size = sizeof(perf_event_attr);
	value->config = Config;
	value->disabled = 1;	// We don't want to start counting until a call to StartProfilingAll()
	value->read_format = PERF_FORMAT_GROUP;

	int err;
	bool isGroupLeader = true;
	
	if (GroupLeaderFD != -1)
	{
		// This should be a child event
		isGroupLeader = false;
		value->disabled = 0;
	}

	OpenPerfFD(isGroupLeader, err, GroupLeaderFD);
}

PerfEventMapEntry::PerfEventMapEntry(const PerfEventMapEntry& Other) : key_config(Other.key_config)
	, value(Other.value)
	, fd(Other.fd)
	, openSuccessful(Other.openSuccessful)
	, nr_index(Other.nr_index)
{
	LOG_DEBUG(DEBUG_VERBOSE, ++constructorStackPointer << endl)
}

PerfEventMapEntry& PerfEventMapEntry::operator=(const PerfEventMapEntry& rhs)
{
	key_config = rhs.key_config;
	value = rhs.value;
	fd = rhs.fd;
	openSuccessful = rhs.openSuccessful;
	nr_index = rhs.nr_index;
	return *this;
}

bool PerfEventMapEntry::OpenPerfFD(const bool& IsGroupLeader, int& errorCode, const int& GroupLeaderFD /*= -1*/)
{
	if (value == NULL)	// Make sure we have assigned values to the perf_event_attr struct!
		return false;

	if (this->fd == 0)		// Only open if hasn't been opened yet
	{
		int pid_to_profile = 0;		// Profile this process
		int cpu_to_profile = -1;	// Any cpu
		int group_fd = -1;			// Start your own group, by default
		unsigned long flags = 0;		// No flags

		// If we're adding to a group, we need to specify the group leader's file descriptor
		if (!IsGroupLeader)
			group_fd = GroupLeaderFD;

		this->fd = syscall(__NR_perf_event_open, value, pid_to_profile, cpu_to_profile, group_fd, flags);

		if (fd == -1)
		{
			errorCode = errno;

			cout << "Error opening perf_fd! errno: " << errorCode << endl;
			printf("Error string: %s\n", strerror(errorCode));
			cout << "E2BIG: " << E2BIG << endl;
			cout << "EINVAL: " << EINVAL << endl;
			cout << "ENOSPC: " << ENOSPC << endl;

			return false;
		}
		else
		{
			LOG_DEBUG(DEBUG_INFO, "Succesfully opened perf_fd with fd of: " << fd << endl)
			openSuccessful = true;
			return true;
		}
	}
	else
		return false;
}