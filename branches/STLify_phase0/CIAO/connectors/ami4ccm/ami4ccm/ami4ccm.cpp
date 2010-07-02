// -*- C++ -*-
// $Id$

#include "ami4ccm/ami4ccm.h"
#include "tao/debug.h"

ACE_RCSID (ami4ccm,
           ami4ccm,
           "$Id$")

CCM_AMI::ExceptionHolder_i::ExceptionHolder_i (
  ::Messaging::ExceptionHolder* holder)
    : holder_ (holder)
{
}

void
CCM_AMI::ExceptionHolder_i::raise_exception (void)
{
  if (holder_)
    {
      holder_->raise_exception ();
    }
  else
    {
      throw ::CORBA::UNKNOWN (TAO::VMCID, CORBA::COMPLETED_YES);
    }
}
