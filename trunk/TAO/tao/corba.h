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

#ifndef TAO_MASTER_CORBA_H
#define TAO_MASTER_CORBA_H

// Include user-defined and platform-specific ORB configuration stuff.
#include "tao/orbconf.h"

// ACE specific includes
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
#include "ace/CDR_Stream.h"

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

class TAO_ORB_Core;
extern TAO_Export TAO_ORB_Core *TAO_ORB_Core_instance (void);

// For some reason, PC compilers don't implement "natural" alignment,
// but only use fixed alignment policies.  The following #pragmas
// configure fixed one-byte alignment policy, since some fixed policy
// needs to apply throughout an ORB.

#if     defined (_MSC_VER)
//#       pragma  pack (push, 1)          // VC++, stack 1-byte alignment policy

#       ifdef   _DEBUG                  // convert from VC++ convention ...
#               define  DEBUG           // ... to normal convention
#       endif

#elif   defined (__BORLANDC__)
#       pragma option -a                // BC++, use 1 byte alignment

#endif /* _MSC_VER */

// Forward declarations of some data types are needed.

class CORBA_Any;
typedef class CORBA_Any *CORBA_Any_ptr;

class CORBA_DynAny;
typedef class CORBA_DynAny *CORBA_DynAny_ptr;

class CORBA_DynStruct;
typedef CORBA_DynStruct *CORBA_DynStruct_ptr;

class CORBA_DynSequence;
typedef CORBA_DynSequence *CORBA_DynSequence_ptr;

class CORBA_DynArray;
typedef CORBA_DynArray *CORBA_DynArray_ptr;

class CORBA_DynUnion;
typedef CORBA_DynUnion *CORBA_DynUnion_ptr;

class CORBA_DynEnum;
typedef CORBA_DynEnum *CORBA_DynEnum_ptr;

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

typedef void (*TAO_Skeleton)(
    CORBA_ServerRequest &,
    void *,
    void *,
    CORBA_Environment &TAO_IN_ENV
  );

// NOTE: stub APIs are nonportable, and must be explicitly #included
// by code emitted from an IDL compiler.

#if defined (_MSC_VER)
//#pragma pack (pop)               // VC++, goes back to other padding rules
#endif /* _MSC_VER */

// Alignment macros
#include "tao/Align.h"

// This class is used even in the ORB definition. Its header file only
// uses the forward declarations.
#include "tao/Environment.h"

// CORBA class.
#include "tao/ORB.h"

typedef TAO_Unbounded_Sequence<CORBA::Octet> TAO_opaque;
extern TAO_Export CORBA::TypeCode_ptr TC_opaque;

extern TAO_Export CORBA_Boolean
operator<< (TAO_OutputCDR&, const TAO_opaque&);

extern TAO_Export CORBA_Boolean
operator>> (TAO_InputCDR&, TAO_opaque&);

#include "tao/try_macros.h"

#include "tao/Exception.h"
#include "tao/Any.h"

#include "tao/NVList.h"
#include "tao/Principal.h"
#include "tao/Request.h"
#include "tao/Object.h"
#include "tao/varout.h"
#include "tao/Typecode.h"

// Marshaling
#include "tao/Marshal.h"
#include "tao/singletons.h"
#include "tao/CDR.h"

#include "tao/PolicyC.h"
#include "tao/CurrentC.h"
#include "tao/POA.h"

// TAO specific includes
// Pluggable Protocol Related Includes first
#include "tao/Pluggable.h"
#include "tao/MProfile.h"
#include "tao/Connect.h"
#include "tao/IIOP_Profile.h"
#include "tao/IIOP_Transport.h"
#include "tao/IIOP_Connector.h"
#include "tao/IIOP_Acceptor.h"
// end pluggable protocols
#include "tao/params.h"
#include "tao/ORB_Core.h"
#include "tao/Operation_Table.h"
#include "tao/debug.h"

#include "tao/Client_Strategy_Factory.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/default_client.h"
#include "tao/default_server.h"

// GIOP - IIOP related includes
#include "tao/Stub.h"
#include "tao/Active_Object_Map.h"
#include "tao/GIOP.h"
#include "tao/IIOP_ORB.h"
#include "tao/IIOP_Interpreter.h"
#include "tao/Invocation.h"
#include "tao/Server_Request.h"

// Dynamic Any includes
#include "tao/InconsistentTypeCodeC.h"
#include "tao/DynAny_i.h"

#if !defined (ACE_NESTED_CLASS)
#if defined (ACE_WIN32)
#define ACE_NESTED_CLASS(SCOPE,CLASS) CLASS
#else /* !ACE_WIN32 */
#define ACE_NESTED_CLASS(SCOPE,CLASS) SCOPE::CLASS
#endif /* ACE_WIN32 */
#endif /* ACE_NESTED_CLASS */

#endif /* TAO_MASTER_CORBA_H */
