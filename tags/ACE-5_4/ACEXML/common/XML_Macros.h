// -*- C++ -*-

// ============================================================================
/**
 * @file XML_Macros.h
 *
 * $Id$
 *
 *   Writing code that is portable between platforms with or without
 *   native C++ exceptions is hard.  The following macros offer some
 *   help on this task.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Aniruddha Gokhale  <gokhale@sahyadri.research.bell-labs.com>
 *  @author Carlos O'Ryan  <coryan@uci.edu>
 *  @author Krishnakumar B <kitty@cs.wustl.edu>, et al.
 */
// ============================================================================

// Macros for handling exceptions.

#ifndef _ACEXML_MACROS_H
#define _ACEXML_MACROS_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Exception_Macros.h"

// The following macros assume that an environment variable is passed
// in/out of each function that can throw an exception. The type of the
// environment variable is defined by ACEXML_ENV_TYPE.

#if !defined (ACEXML_ENV_TYPE)
# define ACEXML_ENV_TYPE  ACEXML_Env
#endif /* ACEXML_ENV_TYPE */

// The name of the variable defined by ACEXML_TRY_ENV. Below is the name
// that we use by default. If you wish to change it you can redefine
// ACEXML_TRY_ENV to change the default name. Also ACEXML_ADOPT_ENV allows the
// use of non-standard name within a scope.

#if !defined (ACEXML_TRY_ENV)
# define ACEXML_TRY_ENV  _ACEXML_Env_variable
#endif /* ACEXML_TRY_ENV */

// The base type of Exception from which all the other exception types are
// derived. By default, it is set to ACEXML_Exception.

#if !defined (ACEXML_EXCEPTION_TYPE)
# define ACEXML_EXCEPTION_TYPE  ACEXML_Exception
#endif /* ACEXML_EXCEPTION_TYPE */

// This is the exception caught by ACEXML_CATCHANY.
#if !defined (ACEXML_ANY_EXCEPTION)
# define ACEXML_ANY_EXCEPTION ex
#endif /* ACEXML_ANY_EXCEPTION */

// Declare a new environment variable on the stack. The type of the
// environment variable is determined by ACEXML_ENV_TYPE.
#define ACEXML_DECLARE_NEW_ENV \
     ACEXML_ENV_TYPE ACEXML_TRY_ENV

#if defined (ACE_USES_NATIVE_EXCEPTIONS)
// -----------------------------------------------------------------

#define ACEXML_ADOPT_ENV (ENV)

// No need to check. Native exceptions handle the control flow
// automatically when an exception occurs.
# define ACEXML_CHECK

// Used when the function requires a return value.
# define ACEXML_CHECK_RETURN(RETV)

// ACEXML_THROW_R_INT should not be used by the user.
# define ACEXML_THROW_R_INT(EXCEPTION) \
      throw EXCEPTION

// Throwing an exception is easy. These two macros should _NOT_ be
// used within try blocks.
# define ACEXML_THROW(EXCEPTION) \
      throw EXCEPTION

// Throwing an exception when the function reqires a return value.
# if defined (WIN32) || defined (__HP_aCC)
#   define ACEXML_THROW_RETURN(EXCEPTION, RETV) \
      do \
        { \
          throw EXCEPTION; \
          return RETV; \
        } while (0)
# else /* WIN32 */
#   define ACEXML_THROW_RETURN(EXCEPTION,RETV) \
        throw EXCEPTION
# endif /* WIN32 */

// For compilers with native exceptions, we can simply use try to try. ;-)
// do {} while (0) is required to avoid compilation warnings.
# define ACEXML_TRY \
   do \
     { \
       try \
         {
# define ACEXML_TRY_NEW_ENV \
   do \
     { \
       ACEXML_ENV_TYPE ACEXML_TRY_ENV; \
       try \
         {
# define ACEXML_TRY_EX(LABEL) \
   do \
     { \
       try \
         {

// No need to check for exceptions within try block for compilers with
// native exceptions.
# define ACEXML_TRY_CHECK
# define ACEXML_TRY_CHECK_EX(LABEL)

// Likewise, throwing exceptions within try blocks is easy.
# define ACEXML_TRY_THROW(EXCEPTION) throw EXCEPTION
# define ACEXML_TRY_THROW_EX(EXCEPTION,LABEL) throw EXCEPTION

// Same thing for catch.
# define ACEXML_CATCH(EXCEPTION,VAR) \
         } \
       catch (EXCEPTION & VAR) \
         { \
           ACE_UNUSED_ARG (VAR);

# define ACEXML_CATCHANY \
       ACEXML_CATCH(ACEXML_EXCEPTION_TYPE, ACEXML_ANY_EXCEPTION)

# define ACEXML_CATCHALL \
         } \
       catch (...) \
         {

// Rethrowing the exception from catch blocks.
# define ACEXML_RE_THROW throw
# define ACEXML_RE_THROW_EX(LABEL) throw

// Close the catch block.
# define ACEXML_ENDTRY \
         } \
     } while (0)

#else /* ! ACEXML_USES_NATIVE_EXCEPTIONS */
// -----------------------------------------------------------------

// When handling compilers without native exceptions, things get a bit
// hairy. Exceptions are simulated using ACEXML_ENV_TYPE. The trick here is to
// make sure the flow-of-control can simulate the case when native
// exceptions occur...

# define ACEXML_ADOPT_ENV(ENV) ACEXML_ENV_TYPE &ACEXML_TRY_ENV = ENV

// Follow every statement that could throw exceptions with ACEXML_CHECK or
// ACEXML_CHECK_RETURN. These two macros should _NOT_ be used within try
// blocks. Use ACEXML_TRY_CHECK or ACEXML_TRY_CHECK_EX instead.
# define ACEXML_CHECK \
    if (ACEXML_TRY_ENV . exception () != 0) \
      return
// When function requires a return value
# define ACEXML_CHECK_RETURN(RETV) \
    if (ACEXML_TRY_ENV . exception () != 0) \
      return RETV

// ACEXML_THROW_R_INT should not be used by the user.
# define ACEXML_THROW_R_INT(EXCEPTION) ACEXML_TRY_ENV.exception (new EXCEPTION)

// Throwing exceptions will inevitably cause a return from the current
// function. These two macros should _NOT_ be used within try blocks. Use
// ACEXML_TRY_THROW or ACEXML_TRY_THROW_EX instead.
# define ACEXML_THROW(EXCEPTION) \
    do \
      { \
        ACEXML_TRY_ENV.exception (new EXCEPTION); \
        return; \
      } while (0)

# define ACEXML_THROW_RETURN(EXCEPTION,RETV) \
    do \
      { \
        ACEXML_TRY_ENV.exception (new EXCEPTION); \
        return RETV; \
      } while (0)

// ACEXML_TRY sets up flags to control program flow. ACEXML_TRY_FLAG acts
// like a one-shot flip-flop. When an exception occurs (detected using
// ACEXML_TRY_CHECK,) ACEXML_TRY_FLAG will be reset and the control goes
// back into ACEXML_TRY_LABEL. Since ACEXML_TRY_FLAG is reset, the try
// block won't get executed again and the control proceeds to the following
// catch blocks. ACEXML_EXCEPTION_NOT_CAUGHT flag is used to prevent
// catching an exception twice. This macro assumes there's already an
// ACEXML_ENV_TYPE variable ACEXML_TRY_ENV defined (which should be the
// case normally)
# define ACEXML_TRY \
   do { \
     int ACEXML_TRY_FLAG = 1; \
     int ACEXML_EXCEPTION_NOT_CAUGHT = 1; \
   ACEXML_TRY_LABEL: \
     if (ACEXML_TRY_FLAG) \
       do {

// ACEXML_TRY_NEW_ENV functions like the macro ACEXML_TRY but defines a new
// ACEXML_ENV_TYPE variable ACEXML_TRY_ENV. It is most often used in the outer
// most function where no ACEXML_TRY_ENV is available.
# define ACEXML_TRY_NEW_ENV \
   do { \
     ACEXML_ENV_TYPE ACEXML_TRY_ENV;\
     int ACEXML_TRY_FLAG = 1; \
     int ACEXML_EXCEPTION_NOT_CAUGHT = 1; \
   ACEXML_TRY_LABEL: \
     if (ACEXML_TRY_FLAG) \
       do {

// ACEXML_TRY_EX works exactly like ACEXML_TRY macro except the label used
// in the try block is customizable to avoid name clashing. It should be
// used when nested try blocks or multiple try blocks are required, in the
// same function.
# define ACEXML_TRY_EX(LABEL) \
   do { \
     int ACEXML_TRY_FLAG = 1; \
     int ACEXML_EXCEPTION_NOT_CAUGHT = 1; \
   ACEXML_TRY_LABEL ## LABEL: \
     if (ACEXML_TRY_FLAG) \
       do {

// Check for exceptions within try blocks.
# define ACEXML_TRY_CHECK \
    { \
      if (ACEXML_TRY_ENV.exception () != 0) \
        { \
          ACEXML_TRY_FLAG = 0; \
          goto ACEXML_TRY_LABEL; \
        } \
    }

// Checking exception within EX try blocks.
# define ACEXML_TRY_CHECK_EX(LABEL) \
    { \
      if (ACEXML_TRY_ENV.exception () != 0) \
        { \
          ACEXML_TRY_FLAG = 0; \
          goto ACEXML_TRY_LABEL ## LABEL; \
        } \
    }

// Throwing exception within TRY blocks.
# define ACEXML_TRY_THROW(EXCEPTION) \
    { \
      ACEXML_TRY_ENV.exception (new EXCEPTION); \
      ACEXML_TRY_FLAG = 0; \
      goto ACEXML_TRY_LABEL; \
    }

# define ACEXML_TRY_THROW_EX(EXCEPTION,LABEL) \
    { \
      ACEXML_TRY_ENV.exception (new EXCEPTION); \
      ACEXML_TRY_FLAG = 0; \
      goto ACEXML_TRY_LABEL ## LABEL; \
    }

// When exceptions occur or try block finishes execution without exception,
// control will continue in the catch block. This macro first checks if
// there's any uncaught exception left. If all the conditions are met, we
// have caught an exception. It then resets ACEXML_EXCEPTION_NOT_CAUGHT to
// prevent subsequent catch blocks from catching the same exception again,
// and extracts out the underlying exception in ACEXML_TRY_ENV. We also make a
// copy of ACEXML_TRY_ENV in ACEXML_CAUGHT_ENV, in case we want to rethrow the
// exception. ACEXML_TRY_ENV is cleared out after the exception is caught so
// you should not use ACEXML_TRY_ENV within the catch block(You should use the
// exception directly).
# define ACEXML_CATCH(TYPE,VAR) \
     } while (0); \
   do \
     if (ACEXML_TRY_ENV.exception () != 0 && ACEXML_EXCEPTION_NOT_CAUGHT && \
       TYPE::_downcast(ACEXML_TRY_ENV.exception ()) != 0) \
       { \
         ACEXML_ENV_TYPE ACEXML_CAUGHT_ENV = ACEXML_TRY_ENV;\
         ACEXML_EXCEPTION_NOT_CAUGHT = 0; \
         TYPE &VAR = *TYPE::_downcast (ACEXML_CAUGHT_ENV.exception ()); \
         ACE_UNUSED_ARG (VAR); \
         ACEXML_TRY_ENV.clear ();

// ACEXML_CATCHANY uses ACEXML_CATCH to catch all exceptions derived from
// ACEXML_EXCEPTION_TYPE
# define ACEXML_CATCHANY ACEXML_CATCH (ACEXML_EXCEPTION_TYPE, ACEXML_ANY_EXCEPTION)

// Since there's no other exception for compilers without exception
// support, we simply catch all ACEXML_EXCEPTION_TYPE exceptions for
// ACEXML_CATCHALL.
# define ACEXML_CATCHALL ACEXML_CATCHANY

// Rethrowing exception within catch blocks. Notice that we depend on the
// ACEXML_CHECK/ACEXML_CHECK_RETURN following the ACEXML_ENDTRY, or
// ACEXML_TRY_CHECK/ ACEXML_TRY_CHECK_EX following the ACEXML_ENDTRY when
// the catch block is within another try block, to do the "Right
// Thing[TM]."
# define ACEXML_RE_THROW \
    do {\
      ACEXML_TRY_ENV = ACEXML_CAUGHT_ENV; \
      goto ACEXML_TRY_LABEL; \
    } while (0)
# define ACEXML_RE_THROW_EX(LABEL) \
    do {\
      ACEXML_TRY_ENV = ACEXML_CAUGHT_ENV; \
      goto ACEXML_TRY_LABEL ## LABEL; \
    } while (0)

// Close the try block. Since exceptions may not get caught, and exceptions
// can also be rethrown from the catch block, it's always a good idea to
// follow ACEXML_ENDTRY with ACEXML_CHECK or ACEXML_TRY_CHECK (depending on
// the context.)
# define ACEXML_ENDTRY \
      } while (0); \
    } while (0)

#endif /* ! ACE_USES_NATIVE_EXCEPTIONS */

// ACE_HAS_EXCEPTIONS is not the same as ACE_NEW_THROWS_EXCEPTIONS.
#if defined(ACE_NEW_THROWS_EXCEPTIONS)

#   define ACEXML_NEW_THROW_EX(POINTER,CONSTRUCTOR,EXCEPTION) \
     do { try { POINTER = new CONSTRUCTOR; } \
       catch (ACE_bad_alloc) { errno = ENOMEM; ACEXML_THROW_R_INT (EXCEPTION); } \
     } while (0)

#else /* ! ACE_NEW_THROWS_EXCEPTIONS */

#   define ACEXML_NEW_THROW_EX(POINTER,CONSTRUCTOR,EXCEPTION) \
     do { POINTER = new CONSTRUCTOR; \
       if (POINTER == 0) { errno = ENOMEM; ACEXML_THROW_R_INT (EXCEPTION); } \
     } while (0)

#endif /* ACE_NEW_THROWS_EXCEPTIONS */

# define ACEXML_GUARD_THROW_EX(MUTEX,OBJ,LOCK,EXCEPTION) \
  ACE_Guard< MUTEX > OBJ (LOCK); \
    if (OBJ.locked () == 0) ACEXML_THROW_R_INT (EXCEPTION);

# define ACEXML_READ_GUARD_THROW_EX(MUTEX,OBJ,LOCK,EXCEPTION) \
  ACE_Read_Guard< MUTEX > OBJ (LOCK); \
    if (OBJ.locked () == 0) ACEXML_THROW_R_INT (EXCEPTION);

# define ACEXML_WRITE_GUARD_THROW_EX(MUTEX,OBJ,LOCK,EXCEPTION) \
  ACE_Write_Guard< MUTEX > OBJ (LOCK); \
    if (OBJ.locked () == 0) ACEXML_THROW_R_INT (EXCEPTION);

//@{
/**
 * @name Native C++ exceptions portability macros.
 *
 * The following macros are used to write code portable between platforms
 * with and without native C++ exception support. Their main goal is to
 * hide the presence of the ACEXML_ENV_TYPE argument, but they collaborate
 * with the ACEXML_TRY_* macros to emulate the try/catch blocks.
 */

/// Define a macro to emit code only when ACEXML_ENV_TYPE is used
#if !defined (ACE_USES_NATIVE_EXCEPTIONS) || defined (ACEXML_ENV_BKWD_COMPAT)
#  define ACEXML_ENV_EMIT_CODE(X) X
#else
#  define ACEXML_ENV_EMIT_CODE(X)
#endif /* ACE_USES_NATIVE_EXCEPTIONS && ! ACEXML_ENV_BKWD_COMPAT */

/// Another macro to emit code only when ACEXML_ENV_TYPE is used
#if !defined (ACE_USES_NATIVE_EXCEPTIONS) || defined (ACEXML_ENV_BKWD_COMPAT)
#  define ACEXML_ENV_EMIT_CODE2(X,Y) X,Y
#else
#  define ACEXML_ENV_EMIT_CODE2(X,Y)
#endif /* ACE_USES_NATIVE_EXCEPTIONS && ! ACEXML_ENV_BKWD_COMPAT */

/// Helper macro
#define ACEXML_ENV_EMIT_DUMMY

/// Declare a ACEXML_ENV_TYPE argument as the last argument of a
/// function
/**
 * Normally this macro is used as follows:
 *
 * <CODE>void my_funct (int x, int y ACEXML_ENV_ARG_DECL);</CODE>
 *
 * Its purpose is to provide developers (and users) with a mechanism to
 * write code that is portable to platforms with and without native C++
 * exceptions.
 */
#define ACEXML_ENV_ARG_DECL \
    ACEXML_ENV_EMIT_CODE2(ACEXML_ENV_EMIT_DUMMY, \
                       ACEXML_ENV_TYPE &ACEXML_TRY_ENV)

/// Declare a ACEXML_ENV_TYPE argument that is not used by the
/// function definition.
/**
 * Similar to ACEXML_ENV_ARG_DECL, but the formal parameter name is dropped to
 * avoid warnings about unused parameters
 */
#define ACEXML_ENV_ARG_DECL_NOT_USED \
    ACEXML_ENV_EMIT_CODE2(ACEXML_ENV_EMIT_DUMMY, \
                       ACEXML_ENV_TYPE &)

/// Declare a ACEXML_ENV_TYPE argument for methods that do not take any other
/// parameters
#define ACEXML_ENV_SINGLE_ARG_DECL \
    ACEXML_ENV_EMIT_CODE(ACEXML_ENV_TYPE &ACEXML_TRY_ENV)

/// Declare a ACEXML_ENV_TYPE argument for methods which don't use it.
#define ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED \
    ACEXML_ENV_EMIT_CODE(ACEXML_ENV_TYPE &)

/// Use the ACEXML_ENV_TYPE argument in a nested call
#define ACEXML_ENV_ARG_PARAMETER \
    ACEXML_ENV_EMIT_CODE2(ACEXML_ENV_EMIT_DUMMY, \
                       ACEXML_TRY_ENV)

/// Use the ACEXML_ENV_TYPE argument in a nested call, assuming that the
/// called function takes only the ACEXML_TRY_ENV argument.
#define ACEXML_ENV_SINGLE_ARG_PARAMETER \
    ACEXML_ENV_EMIT_CODE(ACEXML_TRY_ENV)

/// Eliminate unused argument warnings about ACEXML_TRY_ENV
#define ACEXML_ENV_ARG_NOT_USED \
    ACEXML_ENV_EMIT_CODE(ACE_UNUSED_ARG(ACEXML_TRY_ENV))
//@}

#if !defined (ACE_USES_NATIVE_EXCEPTIONS)
// This thing can be moved above when we drop ACEXML_ENV_BKWD_COMPAT.
#  define ACEXML_ENV_RAISE(ex) ACEXML_TRY_ENV.exception (ex)
#else
#  define ACEXML_ENV_RAISE(ex) (ex)->_raise ()
#endif /* ACEXML_CORBA_HAS_EXCEPTIONS */

#include /**/ "ace/post.h"

#endif /* _ACEXML_MACROS_H */
