/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_aaa.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_AAA_H
# define ACE_ACE_AAA_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_AAA_H)
#   include /**/ <aaa.h>
# endif /* !ACE_LACKS_AAA_H */

// Include additional/alternate headers for certain platfor/compiler combinations.
# if defined (ACE_NEEDS_SYS_AAA_H)
#   include /**/ <sys/aaa.h>
# endif /* ACE_NEEDS_SYS_AAA_H) */

#endif /* ACE_ACE_AAA_H */
