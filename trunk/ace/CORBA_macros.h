// -*- C++ -*-

// ============================================================================
/**
 * @file CORBA_macros.h
 *
 * $Id$
 *
 *   Writing code that is portable between platforms with or without
 *   native C++ exceptions is hard.  The following macros offer some
 *   help on this task, mostly oriented to making the ORB code and the
 *   IDL generated code portable.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Aniruddha Gokhale  <gokhale@sahyadri.research.bell-labs.com>
 *  @author Carlos O'Ryan  <coryan@uci.edu>, et al.
 */
// ============================================================================

// Macros for handling CORBA exceptions.

#ifndef ACE_CORBA_MACROS_H
#define ACE_CORBA_MACROS_H

#include "ace/pre.h"

#include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// By default, if the compiler supports native exception handling, assume
// CORBA also support native exception handling. But it can be disabled by
// defining ACE_CORBA_HAS_EXCEPTIONS=0. If the compiler does not support
// exceptions handling, make sure native exception handling is disabled.
# if defined (ACE_CORBA_HAS_EXCEPTIONS)
#   if (ACE_CORBA_HAS_EXCEPTIONS == 0)
#     undef ACE_CORBA_HAS_EXCEPTIONS
#     define ACE_USES_NATIVE_EXCEPTIONS 0
#   endif /* ACE_CORBA_HAS_EXCEPTIONS */
# else /* ! ACE_CORBA_HAS_EXCEPTIONS */
#   define ACE_CORBA_HAS_EXCEPTIONS
# endif /* ACE_CORBA_HAS_EXCEPTIONS */

#define ACE_ENV_POLLUTE_NAMES

#include "ace/Exception_Macros.h"

// ============================================================

// Print out a TAO exception.  This is not CORBA compliant.
# define ACE_PRINT_TAO_EXCEPTION(EX,INFO) \
  EX._tao_print_exception (INFO)

// Print out a CORBA exception.  There is not portable way to
// dump a CORBA exception.  If you are using other ORB implementation,
// redefine the macro to get what you want.
# if !defined ACE_PRINT_EXCEPTION
#   define ACE_PRINT_EXCEPTION(EX,INFO) ACE_PRINT_TAO_EXCEPTION(EX,INFO)
# endif /* ACE_PRINT_EXCEPTION */

#include "ace/post.h"

#endif /* ACE_CORBA_MACROS_H */
