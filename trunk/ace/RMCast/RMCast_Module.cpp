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
ACE_RMCast_Module::open (void)
{
  if (this->next () != 0)
    return this->next ()->open ();
  return 0;
}

int
ACE_RMCast_Module::close (void)
{
  if (this->next () != 0)
    return this->next ()->close ();
  return 0;
}

int
ACE_RMCast_Module::data (ACE_RMCast::Data &data)
{
  if (this->next () != 0)
    return this->next ()->data (data);
  return 0;
}

int
ACE_RMCast_Module::poll (ACE_RMCast::Poll &poll)
{
  if (this->next () != 0)
    return this->next ()->poll (poll);
  return 0;
}

int
ACE_RMCast_Module::ack_join (ACE_RMCast::Ack_Join &ack_join)
{
  if (this->next () != 0)
    return this->next ()->ack_join (ack_join);
  return 0;
}

int
ACE_RMCast_Module::ack_leave (ACE_RMCast::Ack_Leave &ack_leave)
{
  if (this->next () != 0)
    return this->next ()->ack_leave (ack_leave);
  return 0;
}

int
ACE_RMCast_Module::ack (ACE_RMCast::Ack &ack)
{
  if (this->next () != 0)
    return this->next ()->ack (ack);
  return 0;
}

int
ACE_RMCast_Module::join (ACE_RMCast::Join &join)
{
  if (this->next () != 0)
    return this->next ()->join (join);
  return 0;
}

int
ACE_RMCast_Module::leave (ACE_RMCast::Leave &leave)
{
  if (this->next () != 0)
    return this->next ()->leave (leave);
  return 0;
}
