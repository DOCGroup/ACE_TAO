// Method_Request.cpp
// $Id$

#include "ace/Utils/Method_Request.h"

ACE_RCSID(ace, Method_Request, "$Id$")

ACE_Method_Request::ACE_Method_Request (u_long prio)
  : priority_ (prio)
{
}

ACE_Method_Request::~ACE_Method_Request (void)
{
}

u_long
ACE_Method_Request::priority (void) const
{
  return this->priority_;
}

void
ACE_Method_Request::priority (u_long prio)
{
  this->priority_ = prio;
}
