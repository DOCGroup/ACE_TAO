// FILE_Connector.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/FILE_Connector.h"

ACE_ALLOC_HOOK_DEFINE(ACE_FILE_Connector)

void
ACE_FILE_Connector::dump (void) const
{
  ACE_TRACE ("ACE_FILE_Connector::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_FILE_Connector::ACE_FILE_Connector (void)
{
  ACE_TRACE ("ACE_FILE_Connector::ACE_FILE_Connector");
}

int
ACE_FILE_Connector::connect (ACE_FILE_IO &new_io,
			     const ACE_FILE_Addr &remote_sap,
			     ACE_Time_Value *timeout,
			     const ACE_Addr &,
			     int,
			     int flags,
			     int perms)
{
  ACE_TRACE ("ACE_FILE_Connector::connect");
  ACE_ASSERT (new_io.get_handle () == ACE_INVALID_HANDLE);

  ACE_HANDLE handle = ACE::handle_timed_open (timeout, 
						 remote_sap.get_path_name (),
						 flags, perms);
  new_io.set_handle (handle);
  new_io.remote_addr_ = remote_sap; // class copy.
  return handle == ACE_INVALID_HANDLE ? -1 : 0;
}

