/* -*- C++ -*- */
// $Id$

// Shared_Memory_SV.i

#include "ace/Global_Macros.h"

ACE_INLINE int
ACE_Shared_Memory_SV::open (key_t id,
			    int length,
			    int create,
			    int perms,
			    void *addr,
			    int flags)
{
  ACE_TRACE ("ACE_Shared_Memory_SV::open");
  return shared_memory_.open_and_attach (id, length, create,
					 perms, addr, flags);
}

// The "do-nothing" constructor.

ACE_INLINE
ACE_Shared_Memory_SV::ACE_Shared_Memory_SV (void)
{
  ACE_TRACE ("ACE_Shared_Memory_SV::ACE_Shared_Memory_SV");
}
