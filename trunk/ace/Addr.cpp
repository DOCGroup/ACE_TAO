// Addr.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Addr.h"

// This is a static class variable. 
const ACE_Addr ACE_Addr::sap_any;

#if !defined (__ACE_INLINE__)
#include "ace/Addr.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Addr)

void
ACE_Addr::dump (void) const
{
  ACE_TRACE ("ACE_Addr::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "addr_type_ = %d", this->addr_type_));
  ACE_DEBUG ((LM_DEBUG, "\naddr_size_ = %d", this->addr_size_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Initializes instance variables.

void
ACE_Addr::base_set (int type, int size)
{
  this->addr_type_ = type;
  this->addr_size_ = size;
}

// Initializes instance variables.  Note that 0 is an unspecified
// protocol family type...

ACE_Addr::ACE_Addr (int type, int size)
{
  this->base_set (type, size);
}

