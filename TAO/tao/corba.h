// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    orb.h
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

#if !defined(TAO_CORBA_H)
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
#include "ace/Strategies_T.h"
#include "ace/Connector.h"
#include "ace/Singleton.h"
#include "ace/Reactor.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Synch.h"
#include "ace/Svc_Handler.h"

// COM stuff
#include <objbase.h>
#include <initguid.h>

// ORB configuration
#include "tao/orbconf.h"

// Alignment macros
#include "tao/align.h"

// Defines CORBA namespace
#include "tao/corbacom.h"

// individual CORBA classes
#include "tao/sequence.h" // used by many
#include "tao/any.h"
#include "tao/boa.h"

#include "tao/params.h"
//#include "tao/factories.h"
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

// GIOP - IIOP related includes 
#include "tao/iiopobj.h"
#include "tao/iioporb.h"
#include "tao/giop.h"

// TAO specific includes
#include "tao/connect.h"
#include "tao/objtable.h"
#include "tao/optable.h"
#include "tao/debug.h"

// TAO's specialized BOA
#include "tao/roa.h"

//#include "tao/xdr.h"

// include all the *.i file here
#  if defined (__ACE_INLINE__)
#include "tao/any.i"
#include "tao/boa.i"
#include "tao/object.i"  // should always be
#include "tao/orbobj.i"
#include "tao/typecode.i"
#include "tao/svrrqst.i"
//#include "tao/factories.i"
#include "tao/params.i"
#include "tao/client_factory.i"
#include "tao/server_factory.i"
#include "tao/default_client.i"
#include "tao/default_server.i"
#include "cdr.i" //always
#include "marshal.i" // always
#include "iiopobj.i"
#include "iioporb.i"
#include "connect.i"
#include "roa.i"
#endif /* __ACE_INLINE__ */


#endif /* TAO_CORBA_H */
