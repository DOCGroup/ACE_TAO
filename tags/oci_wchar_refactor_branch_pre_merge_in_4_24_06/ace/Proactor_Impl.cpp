// $Id$

#include "ace/Proactor_Impl.h"

ACE_RCSID (ace,
	   Proactor_Impl,
	   "$Id$")

#if ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))
// This only works on Win32 platforms and on Unix platforms supporting
// aio calls.

ACE_Proactor_Impl::~ACE_Proactor_Impl()
{
}

#endif
