/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    assert.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_ASSERT_H
# define ACE_OS_INCLUDE_ASSERT_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_ASSERT_H)
#   include /**/ <assert.h>
# endif /* !ACE_LACKS_ASSERT_H */

#endif /* ACE_OS_INCLUDE_ASSERT_H */
