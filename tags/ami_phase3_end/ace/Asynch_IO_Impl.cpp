// $Id$
#define ACE_BUILD_DLL
#include "ace/OS.h"
#include "ace/Asynch_IO_Impl.h"

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS))
// This only works on Win32 platforms and on Unix platforms supporting
// aio calls. 

#if !defined (__ACE_INLINE__)
#include "ace/Asynch_IO_Impl.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WIN32 || ACE_HAS_WINCE */
