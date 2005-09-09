// $Id$

// The following configuration file is designed to work for LynxOS,
// version 4.0.0 and later, using the GNU g++ compiler.

// With this version of LynxOS, support for the poll(2) call is removed,
// although the system headers still define it. Otherwise LynxOS config
// is the same as before.

#include "ace/config-LynxOS.h"

#if defined (ACE_HAS_POLL)
# undef ACE_HAS_POLL
#endif /* ACE_HAS_POLL */
