/* -*- C++ -*- */
// $Id$

// TLI.i

#include "ace/TLI.h"

inline int 
ACE_TLI::look (void) const
{
  ACE_TRACE ("ACE_TLI::look");
  return ACE_OS::t_look (this->get_handle ());
}

inline int 
ACE_TLI::rcvdis (struct t_discon *discon) const
{
  ACE_TRACE ("ACE_TLI::rcvdis");
  return ACE_OS::t_rcvdis (this->get_handle (), discon);
}

inline int 
ACE_TLI::snddis (struct t_call *call) const
{
  ACE_TRACE ("ACE_TLI::snddis");
  return ACE_OS::t_snddis (this->get_handle (), call);
}

inline int 
ACE_TLI::rcvrel (void) const
{
  ACE_TRACE ("ACE_TLI::rcvrel");
  return ACE_OS::t_rcvrel (this->get_handle ());
}

inline int 
ACE_TLI::sndrel (void) const
{
  ACE_TRACE ("ACE_TLI::sndrel");
  return ACE_OS::t_sndrel (this->get_handle ());
}
