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

#ifndef ACE_ASSERT_H
#define ACE_ASSERT_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ <assert.h>

#   if defined (ACE_LACKS_ASSERT_MACRO)
#     define assert(expr)
#   endif

#endif /* ACE_ASSERT_H */
