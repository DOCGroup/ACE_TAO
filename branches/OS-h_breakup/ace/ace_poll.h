/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_poll.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_POLL_H
# define ACE_ACE_POLL_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_POLL_H)
#   include /**/ <poll.h>
# endif /* !ACE_LACKS_POLL_H */

#endif /* ACE_ACE_POLL_H */
