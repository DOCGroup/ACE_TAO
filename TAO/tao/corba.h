// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    corba.h
//
// = DESCRIPTION
//     Master Header file for the TAO CORBA ORB.  Include only this
//     file, to see all public ORB interfaces declared by TAO.
//
//     This follows the OMG C++ mapping for compilers that don't
//     support C++ exceptions or namespaces.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc., Chris Cleeland,
//     and Douglas C. Schmidt.
//
// ============================================================================

#if !defined (TAO_MASTER_CORBA_H)
#define TAO_MASTER_CORBA_H

// Include user-defined and platform-specific ORB configuration stuff.
#include "tao/orbconf.h"

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
#include "ace/Containers.h"

#if !defined (TAO_HAS_DLL)
// On Windows NT, the default is to build TAO as a DLL library.
#  define TAO_HAS_DLL 1
#endif /* TAO_HAS_DLL */

// Here are definition for TAO library.
#if defined (TAO_HAS_DLL) && (TAO_HAS_DLL == 1)
#  if defined (TAO_BUILD_DLL)
#    define TAO_Export ACE_Proper_Export_Flag
#    define TAO_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#  else
#    define TAO_Export ACE_Proper_Import_Flag
#    define TAO_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#  endif /* TAO_BUILD_DLL */
#else /* TAO_HAS_DLL */
#  define TAO_Export
#  define TAO_SINGLETON_DECLARATION(T)
#endif /* TAO_HAS_DLL */

// For some reason, PC compilers don't implement "natural" alignment,
// but only use fixed alignment policies.  The following #pragmas
// configure fixed one-byte alignment policy, since some fixed policy
// needs to apply throughout an ORB.

#if     defined (_MSC_VER)
#       pragma  pack (push, 1)          // VC++, stack 1-byte alignment policy

#       ifdef   _DEBUG                  // convert from VC++ convention ...
#               define  DEBUG           // ... to normal convention
#       endif

#elif   defined (__BORLANDC__)
#       pragma option -a                // BC++, use 1 byte alignment

#endif /* _MSC_VER */

// Get various definitions facilitating portability.
#include "tao/orbconf.h"

// Forward declarations of some data types are needed.

class CORBA_Any;
typedef class CORBA_Any *CORBA_Any_ptr;

class CORBA_TypeCode;
typedef class CORBA_TypeCode *CORBA_TypeCode_ptr;

class CORBA_Exception;
typedef class CORBA_Exception *CORBA_Exception_ptr;

class CORBA_Request;
typedef class CORBA_Request *CORBA_Request_ptr;

class CORBA_NamedValue;
typedef class CORBA_NamedValue *CORBA_NamedValue_ptr;

class CORBA_NVList;
typedef class CORBA_NVList *CORBA_NVList_ptr;

class CORBA_ORB;
typedef class CORBA_ORB *CORBA_ORB_ptr;

class CORBA_Object;
typedef class CORBA_Object *CORBA_Object_ptr;

class CORBA_ServerRequest;
typedef class CORBA_ServerRequest *CORBA_ServerRequest_ptr;

class CORBA_Environment;
typedef class CORBA_Environment *CORBA_Environment_ptr;

class CORBA_Principal;
typedef class CORBA_Principal *CORBA_Principal_ptr;

typedef class CORBA_ImplementationDef *CORBA_ImplementationDef_ptr;

typedef class CORBA_InterfaceDef *CORBA_InterfaceDef_ptr;

class CORBA_String_var;

class CORBA_ExceptionList;

class TAO_InputCDR;
class TAO_OuputCDR;

// enum values defined in nvlist.hh, bitwise ORed.
typedef u_int CORBA_Flags;

typedef void (*TAO_Skeleton)(CORBA_ServerRequest &,
                             //                      CORBA_Object_ptr,
                             void *,
                             void *,
                             CORBA_Environment &);

// NOTE: stub APIs are nonportable, and must be explicitly #included
// by code emitted from an IDL compiler.

#if defined (_MSC_VER)
#pragma pack (pop)               // VC++, goes back to other padding rules
#endif /* _MSC_VER */

// Alignment macros
#include "tao/Align.h"

// CORBA class.
#include "tao/ORB.h"

typedef TAO_Unbounded_Sequence<CORBA::Octet> TAO_opaque;
extern CORBA::TypeCode_ptr TC_opaque;

extern TAO_Export TAO_OutputCDR&
operator<<(TAO_OutputCDR&, const TAO_opaque&);

extern TAO_Export TAO_InputCDR&
operator>>(TAO_InputCDR&, TAO_opaque&);

#include "tao/Exception.h"
#include "tao/Any.h"

#include "tao/NVList.h"
#include "tao/Principal.h"
#include "tao/Request.h"
#include "tao/Stub.h"
#include "tao/Object.h"
#include "tao/varout.h"
#include "tao/Typecode.h"

// Marshaling
#include "tao/Marshal.h"
#include "tao/CDR.h"

#include "tao/PolicyC.h"
#include "tao/CurrentC.h"
#include "tao/POA.h"

extern TAO_Export int operator== (const PortableServer::ObjectId &l,
                                  const PortableServer::ObjectId &r);

extern TAO_Export int operator== (const TAO_ObjectKey &l,
                                  const TAO_ObjectKey &r);

// TAO specific includes
#include "tao/params.h"

#include "tao/Connect.h"
#include "tao/ORB_Core.h"
#include "tao/Object_Table.h"
#include "tao/Operation_Table.h"
#include "tao/debug.h"

#include "tao/Client_Strategy_Factory.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/default_client.h"
#include "tao/default_server.h"

// GIOP - IIOP related includes
#include "tao/IIOP_Object.h"
#include "tao/IIOP_ORB.h"
#include "tao/IIOP_Interpreter.h"
#include "tao/GIOP.h"
#include "tao/Invocation.h"
#include "tao/Server_Request.h"

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

#define TAO_THROW_SPEC(X) ACE_THROW_SPEC(X)

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
  ACE_UNUSED_ARG (VAR);

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

#define TAO_RETHROW_RETURN(RETURN) \
_env.exception (TAO_TRY_ENV.exception ()); \
return RETURN

#define TAO_THROW_SPEC(X)

#endif /* ACE_HAS_EXCEPTIONS */

#endif /* TAO_MASTER_CORBA_H */
