// FILE_Connector.cpp
// $Id$

#include "ace/FILE_Connector.h"
#include "ace/Handle_Ops.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/FILE_Connector.i"
#endif

ACE_RCSID(ace, FILE_Connector, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_FILE_Connector)

void
ACE_FILE_Connector::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_FILE_Connector::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
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

  ACE_HANDLE handle = ACE_INVALID_HANDLE;

  // Check to see if caller has requested that we create the filename.
  if (ACE_reinterpret_cast (const ACE_Addr &,
                            ACE_const_cast (ACE_FILE_Addr &,
                                            remote_sap)) == ACE_Addr::sap_any)
    {
      // Create a new temporary file.
#ifdef ACE_LACKS_MKSTEMP
      new_io.addr_ =
        ACE_FILE_Addr (ACE_sap_any_cast (ACE_FILE_Addr &)); // class copy.
#else
      // Use ACE_OS::mkstemp() if it is available since it avoids a
      // race condition, and subsequently a security hole due to that
      // race condition (specifically, a denial-of-service attack).
      //
      // However, using mkstemp() prevents us from doing a timed open
      // since it opens the file for us.  Better to avoid the race
      // condition.
      char filename[] = "ace-file-XXXXXX";

      handle = ACE_OS::mkstemp (filename); // mkstemp() replaces "XXXXXX"

      if (handle == ACE_INVALID_HANDLE
          || new_io.addr_.set (filename) != 0)
        return -1;

      new_io.set_handle (handle);

      return 0;
#endif  /* ACE_LACKS_MKSTEMP */
    }
  else
    new_io.addr_ = remote_sap; // class copy.

  handle = ACE_Handle_Ops::handle_timed_open (timeout,
                                              new_io.addr_.get_path_name (),
                                              flags,
                                              perms);

  new_io.set_handle (handle);
  return handle == ACE_INVALID_HANDLE ? -1 : 0;
}
