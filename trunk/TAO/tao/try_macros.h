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
// ============================================================================

#ifndef TAO_TRY_MACROS_H
#define TAO_TRY_MACROS_H

#include "tao/orbconf.h"

// These macros can be used to write "portable" code between platforms
// supporting CORBA exceptions natively (using the C++ exceptions) or
// through the Enviroment parameter.  Their use requires some
// discipline, but they certainly help...

#if defined (TAO_HAS_EXCEPTIONS)

#define TAO_TRY_ENV __env
// The first "do" scope is for the env.
// The second "do" scope is for the TAO_CHECK_ENV continues.
// These are all on one line so the keywords don't confuse compilers.
#define TAO_TRY do { CORBA_Environment TAO_TRY_ENV; try {
// TAO_*_SYS macros don't declare a new environment variable but use
// existing ones.
#define TAO_TRY_SYS do { try {
#define TAO_TRY_EX(LABEL) do { CORBA_Environment TAO_TRY_ENV; try {

#define TAO_CATCH(TYPE,VAR) } catch (TYPE & VAR) { ACE_UNUSED_ARG (VAR);
#define TAO_CATCH_SYS(TYPE,VAR) TAO_CATCH(TYPE,VAR)

#define TAO_CATCHANY } catch (...) {
#define TAO_CATCHANY_SYS TAO_CATCHANY

#define TAO_ENDTRY }} while (0)

// Use this macro if there's a return statement following TAO_ENDTRY
// and the statement is the last statement in the function.
#define TAO_ENDTRY_RETURN(X) TAO_ENDTRY; ACE_NOTREACHED (return X)

// No need to do checking, exception handling does it for us.
#define TAO_CHECK_ENV
#define TAO_CHECK_ENV_SYS
#define TAO_CHECK_ENV_EX(LABEL)
#define TAO_CHECK_ENV_RETURN(X, Y)
#define TAO_CHECK_ENV_SYS_RETURN(X, Y)

#define TAO_THROW(EXCEPTION) throw EXCEPTION
#define TAO_THROW_ENV(EXCEPTION, ENV) throw EXCEPTION
#define TAO_RETHROW throw

#define TAO_GOTO(LABEL) goto LABEL
#define TAO_LABEL(LABEL) LABEL:

#if defined (ACE_WIN32)

// MSVC++ gives a warning if there is no return after the throw
// expression, it is possible that other compilers have the same
// problem.
#define TAO_THROW_RETURN(EXCEPTION, RETURN) do {\
  throw EXCEPTION; \
  return RETURN; } while (0)
#define TAO_THROW_ENV_RETURN(EXCEPTION, ENV, RETURN) do { \
  throw EXCEPTION; \
  return RETURN; } while (0)
#define TAO_RETHROW_RETURN(RETURN) throw; \
  return RETURN
#define TAO_RETHROW_RETURN_VOID_SYS throw; \
  return

#else

#define TAO_THROW_RETURN(EXCEPTION, RETURN) throw EXCEPTION
#define TAO_THROW_ENV_RETURN(EXCEPTION, ENV, RETURN) throw EXCEPTION
#define TAO_RETHROW_RETURN(RETURN) throw
#define TAO_RETUROW_RETURN_VOID_SYS throw

#endif /* ACE_WIN32 */

#define TAO_RETHROW_RETURN_SYS(RETURN) TAO_RETHROW_RETURN (RETURN)

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

// Define a local enviroment variable...
#define TAO_TRY_ENV __env

// I would like to experiment with this idea in the future....
#if 0
#define TAO_TRY_VAR(X) \
do { CORBA_Environment &_env = CORBA_Environment::default_environment ()= X; \
int TAO_TRY_FLAG = 1; \
TAO_TRY_LABEL: \
if (TAO_TRY_FLAG) \
do {
#endif /* 0 */

// The first "do" scope is for the env.
// The second "do" scope is for the TAO_CHECK_ENV continues.
#define TAO_TRY \
do { CORBA_Environment TAO_TRY_ENV; \
int TAO_TRY_FLAG = 1; \
TAO_TRY_LABEL: \
if (TAO_TRY_FLAG) \
do {

#define TAO_TRY_SYS \
do { \
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

// Each CATCH statement ends the previous scope and starts a new one.
// Since all CATCH statements can end the TAO_TRY macro, they must all
// start a new scope for the next potential TAO_CATCH.  The TAO_ENDTRY
// will finish them all.  Cool, eh?
#define TAO_CATCH_BASE(TYPE,VAR,ENV) \
} while (0); \
do \
if (ENV.exception () != 0 && \
    TYPE::_narrow(ENV.exception ()) != 0) { \
  TYPE &VAR = *TYPE::_narrow (ENV.exception ()); \
  ACE_UNUSED_ARG (VAR);

#define TAO_CATCH(TYPE,VAR) TAO_CATCH_BASE(TYPE,VAR,TAO_TRY_ENV)
#define TAO_CATCH_SYS(TYPE,VAR) TAO_CATCH_BASE(TYPE,VAR,env)

#define TAO_CATCHANY_BASE(ENV) \
} while (0); \
do { \
if (ENV.exception () != 0)

#define TAO_CATCHANY TAO_CATCHANY_BASE(TAO_TRY_ENV)
#define TAO_CATCHANY_SYS TAO_CATCHANY_BASE(env)

// The first "while" closes the local scope.  The second "while"
// closes the TAO_TRY_ENV scope.
#define TAO_ENDTRY \
} while (0); \
} while (0)

// Use this macro if there's a return statement following TAO_ENDTRY
// and the statement is the last statement in the function.
#define TAO_ENDTRY_RETURN(X) TAO_ENDTRY; return X

// If continue is called, control will skip to the next TAO_CATCHANY
// statement.
#define TAO_CHECK_ENV_BASE(ENV) \
{\
if (ENV.exception () != 0) \
  { \
    TAO_TRY_FLAG = 0; \
    goto TAO_TRY_LABEL; \
  } \
}

#define TAO_CHECK_ENV TAO_CHECK_ENV_BASE(TAO_TRY_ENV)
#define TAO_CHECK_ENV_SYS TAO_CHECK_ENV_BASE(env)

// Same as above but for TAO_TRY_EX
#define TAO_CHECK_ENV_EX(LABEL) \
do {\
if (TAO_TRY_ENV.exception () != 0) \
  { \
    TAO_TRY_FLAG = 0; \
    goto TAO_TRY_LABEL ## LABEL; \
  } \
} while (0)

#define TAO_CHECK_ENV_RETURN(X, Y) \
 if ( X . exception () != 0) return Y

#define TAO_THROW(EXCEPTION) \
do {\
  _env.exception (new EXCEPTION); \
  return; } while (0)

#define TAO_THROW_ENV(EXCEPTION, ENV) \
do {\
  ENV.exception (new EXCEPTION); \
  return; } while (0)

#define TAO_THROW_RETURN(EXCEPTION, RETURN) \
do {\
 _env.exception (new EXCEPTION); \
 return RETURN; } while (0)

#define TAO_THROW_ENV_RETURN(EXCEPTION, ENV, RETURN) \
do {\
 ENV.exception (new EXCEPTION); \
 return RETURN; } while (0)

#define TAO_RETHROW \
_env.exception (TAO_TRY_ENV.exception ()); \
return

#define TAO_GOTO(LABEL) goto LABEL
#define TAO_LABEL(LABEL) LABEL:

#define TAO_RETHROW_RETURN(RETURN) \
_env.exception (TAO_TRY_ENV.exception ()); \
return RETURN

#define TAO_RETHROW_RETURN_SYS(RETURN) return RETURN
#define TAO_RETHROW_RETURN_VOID_SYS return

#define TAO_THROW_SPEC(X)

#define TAO_RAISE(EXCEPTION)

#endif /* TAO_HAS_EXCEPTIONS */

// This macros can be used even outside the TAO_TRY blocks, in fact
// some are designed for that purpose.

#define TAO_CHECK_ENV_RETURN_VOID(X) \
 if ( X . exception () != 0) return

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

// Throwing an exception from within a TAO_TRY block has slightly
// different semantics, we need to check that in the near future.
# define TAO_TRY_THROW(EXCEPTION) TAO_THROW(EXCEPTION)

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
