//$Id$

#include "Synch_i.h"

Synch_i::Synch_i (void)
{
  this->synched_ = 0;
}

void
Synch_i::go (CORBA::Long base_time)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
	      "GO\n"));
  this->synched_ = 1;
  this->base_time_.set (base_time, 0);
}

int
Synch_i::synched (void)
{
  return this->synched_;
}

ACE_Time_Value*
Synch_i::base_time (void)
{
  return &this->base_time_;
}
