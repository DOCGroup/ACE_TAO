/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_assert.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_AIO_H
# define ACE_ACE_AIO_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_AIO_H) 
#   include /**/ <aio.h>
# endif /* ACE_LACKS_AIO_H */

# if !defined (_SC_AIO_MAX)
#   define _SC_AIO_MAX 1
# endif /* _SC_AIO_MAX */

# endif /* ACE_ACE_AIO_H */
