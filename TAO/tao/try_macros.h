// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    try_macros.h
//
// = DESCRIPTION
//   Writing code that is portable between platforms with a without
//   native C++ exceptions is hard.  The following macros offer some
//   help on this task, mostly oriented to making the ORB code and the
//   IDL generated code portable.
//
// = AUTHOR
//     Copyright by Douglas C. Schmidt.
//
//   *****************************************************************
//   *                                                               *
//   *        NOTICE !!!    NOTICE !!!   NOTICE !!!   NOTICE !!!     *
//   *                                                               *
//   *                                                               *
//   *        The macros in this file is deprecated.                 *
//   *                                                               *
//   *        Please check out $ACE_ROOT/ace/CORBA_macros.h          *
//   *        for a new set of helper macros that replace            *
//   *        this file.                                             *
//   *                                                               *
//   *****************************************************************
//
// ============================================================================

#ifndef TAO_TRY_MACROS_H
#define TAO_TRY_MACROS_H

#include "tao/orbconf.h"
#include "ace/CORBA_macros.h"

// Define a local enviroment variable...
#define TAO_IN_ENV  ACE_TRY_ENV
#define TAO_TRY_ENV _tao_try_environment

// These macros can be used to write "portable" code between platforms
// supporting CORBA exceptions natively (using the C++ exceptions) or
// through the Enviroment parameter.  Their use requires some
// discipline, but they certainly help...

#if defined (TAO_HAS_EXCEPTIONS)

// The first "do" scope is for the TAO_TRY_ENV.
// The second "do" scope is for the TAO_CHECK_ENV continues.
// These are all on one line so the keywords don't confuse compilers.
#define TAO_TRY do { CORBA_Environment TAO_TRY_ENV; try {
#define TAO_TRY_VAR(X) do { CORBA_Environment &TAO_TRY_ENV = X; \
                            ACE_UNUSED_ARG (TAO_TRY_ENV); try {

#define TAO_TRY_EX(LABEL) do { CORBA_Environment TAO_TRY_ENV; try {
#define TAO_TRY_VAR_EX(X,LABEL) do { CORBA_Environment &TAO_TRY_ENV = X; \
                                ACE_UNUSED_ARG (TAO_TRY_ENV); try {

#define TAO_CATCH(TYPE,VAR) } catch (TYPE & VAR) { ACE_UNUSED_ARG (VAR);

// @@ @@ @@ This conditional compilation is meant to be catch abnormal
//          exceptions so the debugger can catch the exception for us.
#if defined (TAO_DONT_CATCH_DOT_DOT_DOT)
#define TAO_CATCHALL TAO_CATCH (TAO_DONT_CATCH, ex)
#else
#define TAO_CATCHALL } catch (...) {
#endif /* TAO_DONT_CATCH_DOT_DOT_DOT */

#define TAO_ENDTRY }} while (0)

// Use this macro if there's a return statement following TAO_ENDTRY
// and the statement is the last statement in the function.
#define TAO_ENDTRY_RETURN(X) TAO_ENDTRY; ACE_NOTREACHED (return X)

// No need to do checking, exception handling does it for us.
#define TAO_CHECK_ENV
#define TAO_CHECK_ENV_EX(LABEL)
#define TAO_CHECK_ENV_RETURN(X, Y)
#define TAO_CHECK_ENV_RETURN_VOID(X)

#define TAO_TRY_THROW(EXCEPTION) throw EXCEPTION
#define TAO_TRY_THROW_EX(EXCEPTION,LABEL) throw EXCEPTION

#define TAO_THROW(EXCEPTION) throw EXCEPTION
#define TAO_THROW_ENV(EXCEPTION, ENV) throw EXCEPTION
#define TAO_RETHROW throw

#define TAO_GOTO(LABEL) goto LABEL
#define TAO_LABEL(LABEL) LABEL:

#if defined (ACE_WIN32)

// MSVC++ gives a warning if there is no return after the throw
// expression, it is possible that other compilers have the same
// problem.
#define TAO_THROW_ENV_RETURN(EXCEPTION, ENV, RETURN) do { \
  throw EXCEPTION; \
  return RETURN; } while (0)
#define TAO_RETHROW_RETURN(RETURN) throw; \
  return RETURN
#define TAO_RETHROW_RETURN_VOID throw; \
  return

#else

#define TAO_THROW_ENV_RETURN(EXCEPTION, ENV, RETURN) throw EXCEPTION
#define TAO_RETHROW_RETURN(RETURN) throw
#define TAO_RETHROW_RETURN_VOID throw

#endif /* ACE_WIN32 */

#define TAO_THROW_RETURN(EXCEPTION,RETURN) TAO_THROW_ENV_RETURN(EXCEPTION,TAO_IN_ENV,RETURN)

#define TAO_RETHROW_SAME_ENV_RETURN(RETURN) TAO_RETHROW_RETURN (RETURN)
#define TAO_RETHROW_SAME_ENV_RETURN_VOID TAO_RETHROW_RETURN_VOID

// #define TAO_THROW_SPEC(X) ACE_THROW_SPEC(X)
#define TAO_THROW_SPEC(X)
// The IDL compiler is not generating throw specs, so putting them in
// the servant implementation only results in compilation
// errors. Further the spec does not say that we should generate them
// and I'm concerned that putting a throw spec can result in an
// "unexpected" exception, which *could* be translated into a
// CORBA::UNKNOWN, but I'm not sure.

#define TAO_RAISE(EXCEPTION) throw EXCEPTION
// This is used in the implementation of the _raise methods

#else /* ACE_HAS_EXCEPTIONS && TAO_IDL_COMPILES_HAS_EXCEPTIONS */

#define TAO_TRY_VAR(X) \
do { CORBA_Environment &TAO_TRY_ENV = X; \
int TAO_TRY_FLAG = 1; \
TAO_TRY_LABEL: \
if (TAO_TRY_FLAG) \
do {

// The first "do" scope is for the TAO_TRY_ENV.
// The second "do" scope is for the TAO_CHECK_ENV continues.
#define TAO_TRY \
do { CORBA_Environment TAO_TRY_ENV; \
int TAO_TRY_FLAG = 1; \
TAO_TRY_LABEL: \
if (TAO_TRY_FLAG) \
do {

// This serves a similar purpose as the macro above,
// The second "do" scope is for the TAO_CHECK_ENV continues.
#define TAO_TRY_EX(LABEL) \
do { \
CORBA_Environment TAO_TRY_ENV; \
int TAO_TRY_FLAG = 1; \
TAO_TRY_LABEL ## LABEL: \
if (TAO_TRY_FLAG) \
do {

#define TAO_TRY_VAR_EX(X,LABEL) \
do { \
CORBA_Environment &TAO_TRY_ENV = X; \
int TAO_TRY_FLAG = 1; \
TAO_TRY_LABEL ## LABEL: \
if (TAO_TRY_FLAG) \
do {

// Throwing an exception within a try block must be treated differently
// on platforms that don't support native exception because the exception
// won't get caught by the catch clauses automatically.
#define TAO_TRY_THROW(EXCEPTION) \
{\
  TAO_TRY_ENV.exception (new EXCEPTION); \
  TAO_TRY_FLAG = 0; \
  goto TAO_TRY_LABEL; \
}

#define TAO_TRY_THROW_EX (EXCEPTION,LABEL) \
{\
  TAO_TRY_ENV.exception (new EXCEPTION); \
  TAO_TRY_FLAG = 0; \
  goto TAO_TRY_LABEL ## LABEL; \
}

// Each CATCH statement ends the previous scope and starts a new one.
// Since all CATCH statements can end the TAO_TRY macro, they must all
// start a new scope for the next potential TAO_CATCH.  The TAO_ENDTRY
// will finish them all.  Cool, eh?
#define TAO_CATCH(TYPE,VAR) \
} while (0); \
do \
if (TAO_TRY_ENV.exception () != 0 && \
    TYPE::_narrow(TAO_TRY_ENV.exception ()) != 0) { \
  TYPE &VAR = *TYPE::_narrow (TAO_TRY_ENV.exception ()); \
  ACE_UNUSED_ARG (VAR);

#define TAO_CATCHALL TAO_CATCH(CORBA_Exception, ex)

// The first "while" closes the local scope.  The second "while"
// closes the TAO_TRY_ENV scope.
#define TAO_ENDTRY \
} while (0); \
} while (0)

// Use this macro if there's a return statement following TAO_ENDTRY
// and the statement is the last statement in the function.
#define TAO_ENDTRY_RETURN(X) TAO_ENDTRY; return X

// If continue is called, control will skip to the next TAO_CATCH
// statement.
#define TAO_CHECK_ENV \
{\
if (TAO_TRY_ENV.exception () != 0) \
  { \
    TAO_TRY_FLAG = 0; \
    goto TAO_TRY_LABEL; \
  } \
}

// Same as above but for TAO_TRY_EX
#define TAO_CHECK_ENV_EX(LABEL) \
{\
if (TAO_TRY_ENV.exception () != 0) \
  { \
    TAO_TRY_FLAG = 0; \
    goto TAO_TRY_LABEL ## LABEL; \
  } \
}

#define TAO_CHECK_ENV_RETURN(X, Y) \
 if ( X . exception () != 0) return Y

#define TAO_CHECK_ENV_RETURN_VOID(X) \
 if ( X . exception () != 0) return

#define TAO_THROW(EXCEPTION) \
do {\
 TAO_IN_ENV.exception (new EXCEPTION); \
  return; } while (0)

#define TAO_THROW_ENV(EXCEPTION, ENV) \
do {\
  ENV.exception (new EXCEPTION); \
  return; } while (0)

#define TAO_THROW_RETURN(EXCEPTION, RETURN) \
do {\
 TAO_IN_ENV.exception (new EXCEPTION); \
 return RETURN; } while (0)

#define TAO_THROW_ENV_RETURN(EXCEPTION, ENV, RETURN) \
do {\
 ENV.exception (new EXCEPTION); \
 return RETURN; } while (0)

#define TAO_RETHROW \
TAO_IN_ENV.exception (TAO_TRY_ENV.exception ()); \
return

#define TAO_GOTO(LABEL) goto LABEL
#define TAO_LABEL(LABEL) LABEL:

#define TAO_RETHROW_RETURN(RETURN) \
TAO_IN_ENV.exception (TAO_TRY_ENV.exception ()); \
return RETURN

#define TAO_RETHROW_RETURN_VOID \
TAO_IN_ENV.exception (TAO_TRY_ENV.exception ()); \
return

#define TAO_RETHROW_SAME_ENV_RETURN(RETURN) return RETURN
#define TAO_RETHROW_SAME_ENV_RETURN_VOID return

#define TAO_THROW_SPEC(X)

#define TAO_RAISE(EXCEPTION)

#endif /* TAO_HAS_EXCEPTIONS */

#define TAO_CATCHANY TAO_CATCH(CORBA_Exception, ex)

#define TAO_CHECK_RETURN(Y) TAO_CHECK_ENV_RETURN (TAO_IN_ENV, Y)
#define TAO_CHECK_RETURN_VOID TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV)

// This macros can be used even outside the TAO_TRY blocks, in fact
// some are designed for that purpose.

#define TAO_CHECK_ENV_PRINT_RETURN(ENV, PRINT_STRING, RETURN) \
 if (ENV . exception () != 0) \
    { \
        ENV.print_exception (PRINT_STRING); \
        return RETURN; \
    }

#define TAO_CHECK_ENV_PRINT_RETURN_VOID(ENV, PRINT_STRING) \
 if (ENV . exception () != 0) \
    { \
        ENV.print_exception (PRINT_STRING); \
        return; \
    }

// This macros are used to grab a lock using a Guard, test that the
// lock was correctly grabbed and throw an exception on failure.
// If you do not wish to throw an exception, but just to return a
// value please use the ACE macros.
#define TAO_GUARD_THROW(MUTEX,OBJ,LOCK,ENV,EXCEPTION) \
  ACE_Guard<MUTEX> OBJ (LOCK); \
  if (OBJ.locked () == 0) \
      TAO_THROW_ENV (EXCEPTION,ENV)

#define TAO_GUARD_THROW_RETURN(MUTEX,OBJ,LOCK,RETURN,ENV,EXCEPTION) \
  ACE_Guard<MUTEX> OBJ (LOCK); \
  if (OBJ.locked () == 0) \
      TAO_THROW_ENV_RETURN (EXCEPTION,ENV,RETURN)

#define TAO_READ_GUARD_THROW(MUTEX,OBJ,LOCK,ENV,EXCEPTION) \
  ACE_Read_Guard<MUTEX> OBJ (LOCK); \
  if (OBJ.locked () == 0) \
      TAO_THROW_ENV (EXCEPTION,ENV)

#define TAO_READ_GUARD_THROW_RETURN(MUTEX,OBJ,LOCK,RETURN,ENV,EXCEPTION) \
  ACE_Read_Guard<MUTEX> OBJ (LOCK); \
  if (OBJ.locked () == 0) \
      TAO_THROW_ENV_RETURN (EXCEPTION,ENV,RETURN)

#define TAO_WRITE_GUARD_THROW(MUTEX,OBJ,LOCK,ENV,EXCEPTION) \
  ACE_Write_Guard<MUTEX> OBJ (LOCK); \
  if (OBJ.locked () == 0) \
      TAO_THROW_ENV (EXCEPTION,ENV)

#define TAO_WRITE_GUARD_THROW_RETURN(MUTEX,OBJ,LOCK,RETURN,ENV,EXCEPTION) \
  ACE_Write_Guard<MUTEX> OBJ (LOCK); \
  if (OBJ.locked () == 0) \
      TAO_THROW_ENV_RETURN (EXCEPTION,ENV,RETURN)

#endif /* TAO_TRY_MACROS_H */
