/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    new.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_NEW_H
# define ACE_OS_INCLUDE_NEW_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_NEW_H)
#   if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#     include /**/ <new>
#   else
#     include /**/ <new.h>
#   endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
# endif /* ! ACE_LACKS_NEW_H */

#endif /* ACE_OS_INCLUDE_NEW_H */
