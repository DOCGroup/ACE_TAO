// DEV_Addr.cpp
// $Id$

#include "ace/DEV_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/DEV_Addr.i"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Msg.h"
#include "ace/OS_String.h"
#include "ace/ace_stdio.h" // for MAXNAMELEN
#include "ace/Global_Macros.h"

ACE_RCSID(ace, DEV_Addr, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_DEV_Addr)

void
ACE_DEV_Addr::dump (void) const
{
  ACE_TRACE ("ACE_DEV_Addr::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("devname_ = %s"), this->devname_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Do nothing constructor. 

ACE_DEV_Addr::ACE_DEV_Addr (void)
  : ACE_Addr (AF_DEV, sizeof this->devname_)
{
  ACE_TRACE ("ACE_DEV_Addr::ACE_DEV_Addr");

  (void) ACE_OS_String::memset ((void *) &this->devname_, 
                                0, sizeof this->devname_);
}

int
ACE_DEV_Addr::set (const ACE_DEV_Addr &sa)
{
  this->base_set (sa.get_type (), sa.get_size ());

  if (sa.get_type () == AF_ANY)
    (void) ACE_OS_String::memset ((void *) &this->devname_,
                                  0,
                                  sizeof this->devname_);
  else
    (void) ACE_OS_String::memcpy ((void *) &this->devname_, 
                                  (void *) &sa.devname_, 
                                  sa.get_size ());
  return 0;
}

// Copy constructor. 

ACE_DEV_Addr::ACE_DEV_Addr (const ACE_DEV_Addr &sa)
  : ACE_Addr (AF_DEV, sizeof this->devname_)
{
  ACE_TRACE ("ACE_DEV_Addr::ACE_DEV_Addr");

  this->set (sa);
}

ACE_DEV_Addr::ACE_DEV_Addr (const ACE_TCHAR *devname)
  : ACE_Addr (AF_DEV, sizeof this->devname_)
{
  ACE_TRACE ("ACE_DEV_Addr::ACE_DEV_Addr");

  this->set (devname);
}

ACE_DEV_Addr &
ACE_DEV_Addr::operator= (const ACE_DEV_Addr &sa)
{
  ACE_TRACE ("ACE_DEV_Addr::operator=");

  if (this != &sa)
    (void) ACE_OS_String::memcpy ((void *) &this->devname_, 
                                  (void *) &sa.devname_, 
                                  sa.get_size ());
  return *this;
}

