/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_utime.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_UTIME_H
# define ACE_ACE_UTIME_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_UTIME_H)
#   include /**/ <utime.h>
# endif /* !ACE_LACKS_UTIME_H */

#endif /* ACE_ACE_UTIME_H */
