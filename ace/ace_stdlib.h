/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_stdlib.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_STDLIB_H
# define ACE_ACE_STDLIB_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_STDLIB_H)
#   include /**/ <stdlib.h>
# endif /* ACE_LACKS_STDLIB_H */

# if defined (ACE_HAS_BROKEN_RANDR)
// The SunOS 5.4.X version of rand_r is inconsistent with the header
// files...
typedef u_int ACE_RANDR_TYPE;
extern "C" int rand_r (ACE_RANDR_TYPE seed);
# else
#   if defined (HPUX_10)
// HP-UX 10.x's stdlib.h (long *) doesn't match that man page (u_int *)
typedef long ACE_RANDR_TYPE;
#   else
typedef u_int ACE_RANDR_TYPE;
#   endif /* HPUX_10 */
# endif /* ACE_HAS_BROKEN_RANDR */

# if defined (ACE_LACKS_MKTEMP)
extern "C" char *mktemp (char *);
# endif /* ACE_LACKS_MKTEMP */

#endif /* ACE_ACE_STDLIB_H */
