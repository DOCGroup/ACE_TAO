// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    CORBA_macros.h
//
// = DESCRIPTION
//   Writing code that is portable between platforms with or without
//   native C++ exceptions is hard.  The following macros offer some
//   help on this task, mostly oriented to making the ORB code and the
//   IDL generated code portable.
//
// = AUTHOR
//    Nanbor Wang <nanbor@cs.wustl.edu>
//        Based on the original <tao/try_macros.h> implementation by
//        Aniruddha Gokhale  <gokhale@sahyadri.research.bell-labs.com>
//        Carlos O'Ryan  <coryan@cs.wustl.edu>, et al.
// ============================================================================

// Macros for handling CORBA exceptions.

#ifndef ACE_CORBA_MACROS_H
#define ACE_CORBA_MACROS_H

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// All these macros assume the CORBA::Environment variable used to pass
// in/out the exception is call ACE_TRY_ENV.  Below is the name we use
// in TAO (The ACE ORB.)  Most other ORB's have their own naming
// convention.  You can redefine ACE_TRY_ENV to change the default name
// ACE_ADOPT_CORBA_ENV allows the use of non-standard name within a
// scope.
#if !defined (ACE_TRY_ENV)
# define ACE_TRY_ENV _ACE_CORBA_Environment_variable
#endif /* ACE_TRY_ENV */

// This is the exception caught by ACE_CATCHANY.
#if !defined (ACE_ANY_EXCEPTION)
# define ACE_ANY_EXCEPTION ex
#endif /* ACE_ANY_EXCEPTION */

// By default, if the compiler support native exception handling, assume
// CORBA also support native exception handling.  But it can be disabled
// by defining ACE_CORBA_HAS_EXCEPTIONS=0.
// If the compiler does not support exceptions handling, make sure native
// exception handling is disabled.
#if defined (ACE_HAS_EXCEPTIONS)
# if defined (ACE_CORBA_HAS_EXCEPTIONS)
#   if (ACE_CORBA_HAS_EXCEPTIONS == 0)
#     undef ACE_CORBA_HAS_EXCEPTIONS
#   endif /* ACE_CORBA_HAS_EXCEPTIONS == 0 */
# else /* !ACE_CORBA_HAS_EXCEPTIONS */
#   define ACE_CORBA_HAS_EXCEPTIONS
# endif /* ACE_CORBA_HAS_EXCEPTIONS */
#else
# if defined (ACE_CORBA_HAS_EXCEPTIONS)
#   undef ACE_CORBA_HAS_EXCEPTIONS
# endif /* ACE_CORBA_HAS_EXCEPTIONS */
#endif /* ACE_HAS_EXCEPTIONS */

#if defined (ACE_CORBA_HAS_EXCEPTIONS)
// -----------------------------------------------------------------
# define ACE_ADOPT_CORBA_ENV(ENV) ACE_UNUSED_ARG(ENV)

// No need to check.  Native exceptions handle the control
// flow automatically when an exception occurs.
# define ACE_CHECK
// Used then the function requires a return value.
# define ACE_CHECK_RETURN(RETV)

// Throwing an exception is easy. These two macros should _NOT_ be
// used within try blocks.
# define ACE_THROW(EXCEPTION) throw EXCEPTION
// Throwing an exception when the function reqires a return value.
# if defined (WIN32)
#   define ACE_THROW_RETURN(EXCEPTION, RETV) \
      do \
        { \
          throw EXCEPTION; \
          return RETV; \
        } while (0)
# else /* WIN32 */
#   define ACE_THROW_RETURN(EXCEPTION,RETV) throw EXCEPTION
# endif /* WIN32 */

// For compilers with native exceptions, we can simply use
// try to try. ;-)  do {} while (0) is required to avoid
// compilation warnings.
# define ACE_TRY \
   do \
     { \
       try \
         {
# define ACE_TRY_NEW_ENV \
   do \
     { \
       CORBA::Environment ACE_TRY_ENV; \
       try \
         {
# define ACE_TRY_EX(LABEL) \
   do \
     { \
       try \
         {

// No need to check for exceptions within try block for compilers with
// native exceptions.
# define ACE_TRY_CHECK
# define ACE_TRY_CHECK_EX(LABEL)

// Likewise, throwing exceptions within try blocks is easy.
# define ACE_TRY_THROW(EXCEPTION) throw EXCEPTION
# define ACE_TRY_THROW_EX(EXCEPTION,LABEL) throw EXCEPTION

// Same thing for catch.
# define ACE_CATCH(EXCEPTION,VAR) \
         } \
       catch (EXCEPTION & VAR) \
         { \
           ACE_UNUSED_ARG (VAR);
# define ACE_CATCHANY \
       ACE_CATCH(CORBA::Exception, ACE_ANY_EXCEPTION)
# define ACE_CATCHALL \
         } \
       catch (...) \
         {

// Rethrowing the exception from catch blocks.
# define ACE_RETHROW throw

// Close the catch block.
# define ACE_ENDTRY \
         } \
     } while (0)

#else /* ! ACE_CORBA_HAS_EXCEPTIONS */
// -----------------------------------------------------------------
// To handle compilers without native exceptions, things get a bit
// hairy.  Exceptions are simulated using CORBA::Environment.
// The trick here is to make sure the flow-of-control can simulate
// the case when native exceptions occur...

# define ACE_ADOPT_CORBA_ENV(ENV) CORBA::Environment &ACE_TRY_ENV = ENV

// Follow every statement that could throw exceptions with ACE_CHECK
// or ACE_CHECK_ENV.  These two macros should _NOT_ be used within
// try blocks.  Use ACE_TRY_CHECK or ACE_TRY_CHECK_EX instead.
# define ACE_CHECK \
    if (ACE_TRY_ENV . exception () != 0) \
      return
// When function requires a return value
# define ACE_CHECK_RETURN(RETV) \
    if (ACE_TRY_ENV . exception () != 0) \
      return RETV

// Throwing exceptions will inevitably cause an return from the current
// function.  These two macros should _NOT_ be used within try blocks.
// Use ACE_TRY_THROW or ACE_TRY_THROW_EX instead.
# define ACE_THROW(EXCEPTION) \
    do \
      { \
        ACE_TRY_ENV.exception (new EXCEPTION); \
        return; \
      } while (0)

# define ACE_THROW_RETURN(EXCEPTION,RETV) \
    do \
      { \
        ACE_TRY_ENV.exception (new EXCEPTION); \
        return RETV; \
      } while (0)

// ACE_TRY sets up flags to control program flow.  ACE_TRY_FLAG acts
// like a one-shot flip-flop.  When an exception occured (detect it
// using ACE_TRY_CHECK,) ACE_TRY_FLAG will be reset and the control
// goes back into ACE_TRY_LABEL.  Since ACE_TRY_FLAG is reset, the try
// block won't get executed again and the control proceeds to the following
// catch blocks.  ACE_EXECTION_NOT_CAUGHT flag is used to prevent
// catching an exception twice.  This macro assumes there's already an
// CORBA:;Environment variable ACE_TRY_ENV defined (which, should be
// the most usual case.)
# define ACE_TRY \
   do { \
     int ACE_TRY_FLAG = 1; \
     int ACE_EXCEPTION_NOT_CAUGHT = 1; \
   ACE_TRY_LABEL: \
     if (ACE_TRY_FLAG) \
       do {

// ACE_TRY_NEW_ENV functions like the macro ACE_TRY but defines a
// new CORBA::Environment variable ACE_TRY_ENV.  It is most often
// used in the outer most function where no ACE_TRY_ENV is available.
# define ACE_TRY_NEW_ENV \
   do { \
     CORBA::Environment ACE_TRY_ENV;\
     int ACE_TRY_FLAG = 1; \
     int ACE_EXCEPTION_NOT_CAUGHT = 1; \
   ACE_TRY_LABEL: \
     if (ACE_TRY_FLAG) \
       do {

// ACE_TRY_EX works exactly like ACE_TRY macro except the lable used
// in the try block is customizable to avoid name clashing.  It should
// be used when, nested try blocks or, multiple try blocks are required
// in the same function.
# define ACE_TRY_EX(LABEL) \
   do { \
     int ACE_TRY_FLAG = 1; \
     int ACE_EXCEPTION_NOT_CAUGHT = 1; \
   ACE_TRY_LABEL ## LABEL: \
     if (ACE_TRY_FLAG) \
       do {

// Check for exceptions within try blocks.
# define ACE_TRY_CHECK \
    { \
      if (ACE_TRY_ENV.exception () != 0) \
        { \
          ACE_TRY_FLAG = 0; \
          goto ACE_TRY_LABEL; \
        } \
    }

// Checking exception within EX try blocks.
# define ACE_TRY_CHECK_EX(LABEL) \
    { \
      if (ACE_TRY_ENV.exception () != 0) \
        { \
          ACE_TRY_FLAG = 0; \
          goto ACE_TRY_LABEL ## LABEL; \
        } \
    }

// Throwing exception within TRY blocks.
# define ACE_TRY_THROW(EXCEPTION) \
    { \
      ACE_TRY_ENV.exception (new EXCEPTION); \
      ACE_TRY_FLAG = 0; \
      goto ACE_TRY_LABEL; \
    }

# define ACE_TRY_THROW_EX(EXCEPTION,LABEL) \
    { \
      ACE_TRY_ENV.exception (new EXCEPTION); \
      ACE_TRY_FLAG = 0; \
      goto ACE_TRY_LABEL ## LABEL; \
    }

// When exceptions occur or try block finishes execution without
// exception, control will continue in the catch block.  This macro
// first check if there's non-caught exception we are waiting for
// left.  It all the conditions met, we have caught an exception.
// It then reset the ACE_EXCEPTION_NOT_CAUGHT to prevent subsequent
// catch blocks catch the same exception again and extract out the
// underlying exception in the ACE_TRY_ENV.  We also make a copy
// of ACE_TRY_ENV in ACE_CAUGHT_ENV in case we want to rethrow the
// exception.  ACE_TRY_ENV is cleared out after the exception is
// caught so you should not use ACE_TRY_ENV within the catch block.
// (In fact, you should use the exception directly.)
# define ACE_CATCH(TYPE,VAR) \
     } while (0); \
   do \
     if (ACE_TRY_ENV.exception () != 0 && ACE_EXCEPTION_NOT_CAUGHT && \
       TYPE::_narrow(ACE_TRY_ENV.exception ()) != 0) \
       { \
         CORBA::Environment ACE_CAUGHT_ENV = ACE_TRY_ENV;\
         ACE_EXCEPTION_NOT_CAUGHT = 0; \
         TYPE &VAR = *TYPE::_narrow (ACE_CAUGHT_ENV.exception ()); \
         ACE_UNUSED_ARG (VAR); \
         ACE_TRY_ENV.clear ();

// ACE_CATCHANY uses ACE_CATCH to catch all CORBA exceptions.
# define ACE_CATCHANY ACE_CATCH (CORBA::Exception, ACE_ANY_EXCEPTION)

// Since there's no other exception for compilers without exception
// support, we simply catch all CORBA exceptions for ACE_CATCHALL.
# define ACE_CATCHALL ACE_CATCHANY

// Rethrowing exception within catch blocks.  Notice that we depends
// on the ACE_CHECK/ACE_CHECK_RETURN following the ACE_ENDTRY to
// do the "Right Thing[TM]."
# define ACE_RETHROW \
    do \
      ACE_TRY_ENV = ACE_CAUGHT_ENV; \
    while (0)

// Close the try block.  Notice that since exception may not get
// caught, and exceptions can also be rethrown from the catch block,
// it's always a good idea to follow ACE_ENDTRY with ACE_CHECK or
// ACE_TRY_CHECK (depending on the context.)
# define ACE_ENDTRY \
      } while (0); \
    } while (0)

#endif /* ! ACE_CORBA_HAS_EXCEPTIONS */

#endif /* ACE_CORBA_MACROS_H */
