// Synch_Options.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Synch_Options.h"

ACE_ALLOC_HOOK_DEFINE(ACE_Synch_Options)

void
ACE_Synch_Options::dump (void) const
{
  ACE_TRACE ("ACE_Synch_Options::dump");
}

// Static initialization.

/* static */ 
ACE_Synch_Options ACE_Synch_Options::defaults;

/* static */ 
ACE_Synch_Options ACE_Synch_Options::synch;

/* static */ 
ACE_Synch_Options ACE_Synch_Options::asynch (ACE_Synch_Options::USE_REACTOR);

ACE_Synch_Options::ACE_Synch_Options (u_long options,
				      const ACE_Time_Value &timeout,
				      const void *arg)
{
  ACE_TRACE ("ACE_Synch_Options::ACE_Synch_Options");
  this->set (options, timeout, arg);
}

void
ACE_Synch_Options::set (u_long options,
			const ACE_Time_Value &timeout,
			const void *arg)
{
  ACE_TRACE ("ACE_Synch_Options::set");
  this->options_ = options;
  this->timeout_ = (ACE_Time_Value &) timeout;
  this->arg_ = arg;
}

int
ACE_Synch_Options::operator[] (u_long option) const
{
  ACE_TRACE ("ACE_Synch_Options::operator[]");
  return (this->options_ & option) != 0;
}

void 
ACE_Synch_Options::operator= (u_long option)
{
  ACE_TRACE ("ACE_Synch_Options::operator=");
  this->options_ |= option;
}

const ACE_Time_Value &
ACE_Synch_Options::timeout (void) const
{
  ACE_TRACE ("ACE_Synch_Options::timeout");
  return this->timeout_;
}

void 
ACE_Synch_Options::timeout (ACE_Time_Value &tv)
{
  ACE_TRACE ("ACE_Synch_Options::timeout");
  this->timeout_ = tv;
}

const ACE_Time_Value *
ACE_Synch_Options::time_value (void) const
{
  ACE_TRACE ("ACE_Synch_Options::time_value");
  return (*this)[USE_TIMEOUT] ? &this->timeout_ : 0;
}

const void *
ACE_Synch_Options::arg (void) const
{
  ACE_TRACE ("ACE_Synch_Options::arg");
  return this->arg_;
}

void
ACE_Synch_Options::arg (const void *a)
{
  ACE_TRACE ("ACE_Synch_Options::arg");
  this->arg_ = a;
}

