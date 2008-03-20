// $Id$

#include "tao/PortableServer/Active_Object_Map_Monitor.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Guard_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Active_Object_Map_Monitor::Active_Object_Map_Monitor (void)
{
}
    
Active_Object_Map_Monitor::Active_Object_Map_Monitor (const char* name)
  : Monitor_Base (name)
{
}

Active_Object_Map_Monitor::~Active_Object_Map_Monitor (void)
{
}

void
Active_Object_Map_Monitor::update (void)
{
}

void
Active_Object_Map_Monitor::receive (double value)
{
  // No reason to call this overload.
}

void
Active_Object_Map_Monitor::receive (size_t value)
{
  ACE_WRITE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
  this->data_.timestamp_ = ACE_OS::gettimeofday ();
  this->data_.value_ = static_cast<double> (value);
}

void
Active_Object_Map_Monitor::clear (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
