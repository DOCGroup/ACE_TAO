// System_Time.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/System_Time.h"

ACE_System_Time::ACE_System_Time (LPCTSTR poolname)
  : delta_time_ (0)
{
  ACE_TRACE ("ACE_System_Time::ACE_System_Time");
  ACE_NEW (this->shmem_, ALLOCATOR (poolname));
}

ACE_System_Time::~ACE_System_Time (void)
{
  delete this->shmem_;
  ACE_TRACE ("ACE_System_Time::~ACE_System_Time");
}

// Get the local system time.

int
ACE_System_Time::get_local_system_time (ACE_UINT32 &time_out)
{
  ACE_TRACE ("ACE_System_Time::get_local_system_time");
  time_out = ACE_OS::time (0);
  return 0;
}

int
ACE_System_Time::get_local_system_time (ACE_Time_Value &time_out)
{
  ACE_TRACE ("ACE_System_Time::get_local_system_time");
  time_out.sec (ACE_OS::time (0));
  return 0;
}

// Get the system time of the central time server.

int
ACE_System_Time::get_master_system_time (ACE_UINT32 &time_out)
{
  ACE_TRACE ("ACE_System_Time::get_master_system_time");

  if (this->delta_time_ == 0)
    {
      // Try to find it
      void * temp;
      if (this->shmem_->find (ACE_DEFAULT_TIME_SERVER_STR, temp) ==  -1)
	{
	  // No time entry in shared memory (meaning no Clerk exists)
	  // so return the local time of the host.
	  return this->get_local_system_time (time_out);
	}
      else
	// Extract the delta time.
	this->delta_time_ = (long *) temp;
    }

  ACE_UINT32 local_time;

  // If delta_time is positive, it means that the system clock is
  // ahead of our local clock so add delta to the local time to get an
  // approximation of the system time. Else if delta time is negative,
  // it means that our local clock is ahead of the system clock, so
  // return the last local time stored (to avoid time conflicts).
  if (*this->delta_time_ >=0 )
    {
      this->get_local_system_time (local_time);
      time_out = local_time + (ACE_UINT32) *this->delta_time_;
    }
  else
    // Return the last local time. Note that this is stored as the
    // second field in shared memory.
    time_out = *(this->delta_time_ + 1);
  return 0;
}

int
ACE_System_Time::get_master_system_time (ACE_Time_Value &time_out)
{
  ACE_TRACE ("ACE_System_Time::get_master_system_time");
  ACE_UINT32 to;
  if (this->get_master_system_time (to) == -1)
    return -1;
  time_out.sec (to);
  return 0;
}

// Synchronize local system time with the central time server using
// specified mode (currently unimplemented).

int
ACE_System_Time::sync_local_system_time (ACE_System_Time::Sync_Mode)
{
  ACE_TRACE ("ACE_System_Time::sync_local_system_time");
  ACE_NOTSUP_RETURN (-1);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>
#pragma instantiate ACE_Allocator_Adapter<ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

