//
// $Id$
//

#include "RMCast_Module.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Module.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Module, "$Id$")

ACE_RMCast_Module::~ACE_RMCast_Module (void)
{
}

int
ACE_RMCast_Module::next (ACE_RMCast_Module *next)
{
  if (this->next_ != 0 && next != 0)
    return 1;
  this->next_ = next;
  return 0;
}

ACE_RMCast_Module *
ACE_RMCast_Module::next (void) const
{
  return this->next_;
}

int
ACE_RMCast_Module::prev (ACE_RMCast_Module *prev)
{
  if (this->prev_ != 0 && prev != 0)
    return 1;
  this->prev_ = prev;
  return 0;
}

ACE_RMCast_Module *
ACE_RMCast_Module::prev (void) const
{
  return this->prev_;
}

int
ACE_RMCast_Module::open (void)
{
  return 0;
}

int
ACE_RMCast_Module::close (void)
{
  return 0;
}
