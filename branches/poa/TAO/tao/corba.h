// This may look like C, but it's really -*- C++ -*-
//
// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    corba.h
//
// = DESCRIPTION
//     MASTER HEADER file for C/C++ Win32 interface to a CORBA 2.0 ORB.
//     Include only this file, to see all ORB interfaces declared.
//
//     This interface uses COM as the basis for its binary standard,
//     and follows the OMG C++ mapping for compilers which don't
//     support C++ exceptions (COM doesn't use them) or namespaces.
//     Objects which are implemented using this source base support
//     OLE Automation.
//
//     XXX Note re status of this as reference, cosubmitted with RFP?
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//
// ============================================================================

#if !defined (TAO_CORBA_H)
#define TAO_CORBA_H

// ACE specific includes
#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/SOCK_Stream.h"
#include "ace/Synch_T.h"
#include "ace/ACE.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Strategies.h"
#include "ace/Connector.h"
#include "ace/Singleton.h"
#include "ace/Reactor.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Synch.h"
#include "ace/Svc_Handler.h"

#if !defined (TAO_HAS_DLL)
// On Windows NT, the default is to build TAO as a DLL library.
#  define TAO_HAS_DLL 1
#endif /* TAO_HAS_DLL */

// Here are definition for TAO library.
#if defined (TAO_HAS_DLL) && (TAO_HAS_DLL == 1)
#  if defined (TAO_BUILD_DLL)
#    define TAO_Export ACE_Proper_Export_Flag
#    define TAO_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TAO_SINGLETON_INSTANTIATION(T) ACE_PROPER_SINGLETON_INSTANTIATION (T)
#  else
#    define TAO_Export ACE_Proper_Import_Flag
#    define TAO_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TAO_SINGLETON_INSTANTIATION(T)
#  endif /* TAO_BUILD_DLL */
#else /* TAO_HAS_DLL */
#  define TAO_Export
#  define TAO_SINGLETON_DECLARATION(T)
#  define TAO_SINGLETON_INSTANTIATION(T)
#endif /* TAO_HAS_DLL */

// COM stuff
#include "tao/compat/objbase.h"
#include "tao/compat/initguid.h"

// ORB configuration
#include "tao/orbconf.h"

#include "tao/orb.h"

// Alignment macros
#include "tao/align.h"

// Defines CORBA namespace
#include "tao/corbacom.h"
#include "tao/objkeyC.h"

// individual CORBA classes
#include "tao/sequence.h"
#include "tao/varout.h"
#include "tao/any.h"
#include "tao/poa.h"

#include "tao/params.h"
#include "tao/client_factory.h"
#include "tao/client_factory.h"
#include "tao/server_factory.h"
#include "tao/default_client.h"
#include "tao/default_server.h"

#include "tao/except.h"
#include "tao/orbobj.h"
#include "tao/nvlist.h"
#include "tao/object.h"
#include "tao/orbobj.h"
#include "tao/principa.h"
#include "tao/request.h"
#include "tao/svrrqst.h"
#include "tao/typecode.h"

// Marshaling
#include "tao/marshal.h"
#include "tao/cdr.h"
#include "tao/stub.h"

// TAO specific includes
#include "tao/connect.h"
#include "tao/orb_core.h"
#include "tao/objtable.h"
#include "tao/optable.h"
#include "tao/debug.h"
//#include "tao/managed_types.h"

// GIOP - IIOP related includes
#include "tao/iiopobj.h"
#include "tao/iioporb.h"
#include "tao/giop.h"

//#include "tao/xdr.h"

// The following *.i files are always included here
#  if !defined(__ACE_INLINE__)
#    undef ACE_INLINE
#    define ACE_INLINE inline
#    define do_undef_on_ACE_INLINE
#  endif

#include "tao/orb_core.i"
#include "tao/corbacom.i"
#include "tao/sequence.i"
#include "tao/typecode.i"
#include "tao/nvlist.i"
#include "tao/any.i"
#include "tao/stub.i"
#include "tao/object.i"
#include "tao/orbobj.i"
#include "tao/marshal.i"
#include "tao/cdr.i"
#include "tao/poa.i"
#include "tao/giop.i"
#include "tao/iioporb.i"
#include "tao/iiopobj.i"
//#include "tao/managed_types.i"

#  if defined (do_undef_on_ACE_INLINE)
#    undef do_undef_on_ACE_INLINE
#    undef ACE_INLINE
#    define ACE_INLINE
#  endif /* do_undef_on_ACE_INLINE */

// the following *.i files are conditionally included here
#if defined (__ACE_INLINE__)
#include "tao/params.i"
#include "tao/server_factory.i"
#include "tao/default_client.i"
#include "tao/default_server.i"
#include "tao/connect.i"
#endif /* __ACE_INLINE__ */

#if defined (__ACE_INLINE__)
#endif /* __ACE_INLINE__ */

#include "tao/singletons.h"

#if !defined (ACE_NESTED_CLASS)
#if defined (ACE_WIN32)
#define ACE_NESTED_CLASS(SCOPE,CLASS) CLASS
#else /* !ACE_WIN32 */
#define ACE_NESTED_CLASS(SCOPE,CLASS) SCOPE::CLASS
#endif /* ACE_WIN32 */
#endif /* ACE_NESTED_CLASS */

//
// These macros can be used to write "portable" code between platforms
// supporting CORBA exceptions natively (using the C++ exceptions) or
// through the Enviroment parameter.
// Their use requires some discipline, but they certainly help...
//
// TODO: Currently the IDL compiler does not support C++ exceptions, so we
// cannot use them even if the platform has them.
//
#if defined (ACE_HAS_EXCEPTIONS) && defined (TAO_IDL_COMPILER_HAS_EXCEPTIONS)

#define TAO_TRY_ENV __env
// The first "do" scope is for the env.
// The second "do" scope is for the TAO_CHECK_ENV continues.
#define TAO_TRY \
try { CORBA::Environment TAO_TRY_ENV;
#define TAO_CATCH(TYPE,VAR) \
} catch (TYPE VAR) {
#define TAO_CATCHANY \
} catch (...) {
#define TAO_ENDTRY }

// No need to do checking, exception handling does it for us.
#define TAO_CHECK_ENV
#define TAO_CHECK_ENV_RETURN(X, Y)

#define TAO_THROW(EXCEPTION) throw EXCEPTION;
#define TAO_THROW_RETURN(EXCEPTION, RETURN) throw EXCEPTION
#define TAO_RETHROW throw;

#define TAO_THROW_SPEC(X) throw X

#else /* ACE_HAS_EXCEPTIONS && TAO_IDL_COMPILES_HAS_EXCEPTIONS */

// Define a local enviroment variable...
#define TAO_TRY_ENV __env

// I would like to experiment with this idea in the future....
#if 0 /* g++ on VxWorks didn't like the backslashes at the end of comment
         lines . . . */
#define TAO_TRY_VAR(X) \
do { CORBA::Environment &TAO_TRY_ENV = X; \
int TAO_TRY_FLAG = 1; \
TAO_TRY_LABEL: \
if (TAO_TRY_FLAG) \
do {
#endif /* 0 */

// The first "do" scope is for the env.
// The second "do" scope is for the TAO_CHECK_ENV continues.
#define TAO_TRY \
do { CORBA::Environment TAO_TRY_ENV; \
int TAO_TRY_FLAG = 1; \
TAO_TRY_LABEL: \
if (TAO_TRY_FLAG) \
do {

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


#define TAO_CATCHANY \
} while (0); \
do { \
if (TAO_TRY_ENV.exception () != 0)

// The first "while" closes the local scope.  The second "while"
// closes the TAO_TRY_ENV scope.
#define TAO_ENDTRY \
} while (0); \
} while (0)

// If continue is called, control will skip to the next TAO_CATCHANY
// statement.
#define TAO_CHECK_ENV \
{\
if (TAO_TRY_ENV.exception () != 0) \
  { \
    TAO_TRY_FLAG = 0; \
    goto TAO_TRY_LABEL; \
  } \
}

#define TAO_CHECK_ENV_RETURN(X, Y) \
if ( X . exception () != 0) return Y

#define TAO_THROW(EXCEPTION) \
do {\
  _env.exception (new EXCEPTION); \
  return; } while (0)

#define TAO_THROW_RETURN(EXCEPTION, RETURN) \
do {\
 _env.exception (new EXCEPTION); \
 return RETURN; } while (0)

#define TAO_RETHROW \
_env.exception (TAO_TRY_ENV.exception ()); \
return

#define TAO_RETHROW_RETURN (RETURN) \
_env.exception (TAO_TRY_ENV.exception ()); \
return RETURN

#define TAO_THROW_SPEC(X)

#endif /* ACE_HAS_EXCEPTIONS */

#endif /* TAO_CORBA_H */
