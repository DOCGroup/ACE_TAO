// $Id$

#include "ace/RMCast/RMCast_Fork.h"

#if !defined (__ACE_INLINE__)
# include "ace/RMCast/RMCast_Fork.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Fork, "$Id$")

ACE_RMCast_Fork::~ACE_RMCast_Fork (void)
{
}

int
ACE_RMCast_Fork::open (void)
{
  int r = this->ACE_RMCast_Module::open ();
  if (r != 0)
    return r;
  if (this->secondary () != 0)
    return this->secondary ()->open ();
  return 0;
}

int
ACE_RMCast_Fork::close (void)
{
  int r = this->ACE_RMCast_Module::close ();
  if (r != 0)
    return r;
  if (this->secondary () != 0)
    return this->secondary ()->close ();
  return 0;
}

int
ACE_RMCast_Fork::data (ACE_RMCast::Data &data)
{
  int r = this->ACE_RMCast_Module::data (data);
  if (r != 0)
    return r;
  if (this->secondary () != 0)
    return this->secondary ()->data (data);
  return 0;
}

int
ACE_RMCast_Fork::poll (ACE_RMCast::Poll &poll)
{
  int r = this->ACE_RMCast_Module::poll (poll);
  if (r != 0)
    return r;
  if (this->secondary () != 0)
    return this->secondary ()->poll (poll);
  return 0;
}

int
ACE_RMCast_Fork::ack_join (ACE_RMCast::Ack_Join &ack_join)
{
  int r = this->ACE_RMCast_Module::ack_join (ack_join);
  if (r != 0)
    return r;
  if (this->secondary () != 0)
    return this->secondary ()->ack_join (ack_join);
  return 0;
}

int
ACE_RMCast_Fork::ack_leave (ACE_RMCast::Ack_Leave &ack_leave)
{
  int r = this->ACE_RMCast_Module::ack_leave (ack_leave);
  if (r != 0)
    return r;
  if (this->secondary () != 0)
    return this->secondary ()->ack_leave (ack_leave);
  return 0;
}

int
ACE_RMCast_Fork::ack (ACE_RMCast::Ack &ack)
{
  int r = this->ACE_RMCast_Module::ack (ack);
  if (r != 0)
    return r;
  if (this->secondary () != 0)
    return this->secondary ()->ack (ack);
  return 0;
}

int
ACE_RMCast_Fork::join (ACE_RMCast::Join &join)
{
  int r = this->ACE_RMCast_Module::join (join);
  if (r != 0)
    return r;
  if (this->secondary () != 0)
    return this->secondary ()->join (join);
  return 0;
}

int
ACE_RMCast_Fork::leave (ACE_RMCast::Leave &leave)
{
  int r = this->ACE_RMCast_Module::leave (leave);
  if (r != 0)
    return r;
  if (this->secondary () != 0)
    return this->secondary ()->leave (leave);
  return 0;
}
