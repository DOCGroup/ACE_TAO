/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sched.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SCHED_H
# define ACE_ACE_SCHED_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_SCHED_H)
#   include /**/ <sched.h>
# endif /* !ACE_LACKS_SCHED_H */

#endif /* ACE_ACE_SCHED_H */
