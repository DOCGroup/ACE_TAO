// This may look like C, but it's really -*- C++ -*-

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

#if !defined (TAO_ORB_H)
#define	TAO_ORB_H

#include <stdlib.h>

// For some reason, PC compilers don't implement "natural" alignment,
// but only use fixed alignment policies.  The following #pragmas
// configure fixed one-byte alignment policy, since some fixed policy
// needs to apply throughout an ORB.

#if	defined (_MSC_VER)
#	pragma	pack (push, 1)		// VC++, stack 1-byte alignment policy

#	ifdef	_DEBUG			// convert from VC++ convention ...
#		define	DEBUG		// ... to normal convention
#	endif

#elif	defined (__BORLANDC__)
#	pragma option -a		// BC++, use 1 byte alignment

#endif

// Get various definitions facilitating portability.
#include "orbconf.h"

// Forward declarations of some data types are needed.

class CORBA_Any;
typedef class CORBA_Any *CORBA_Any_ptr;

class CORBA_TypeCode;
typedef class CORBA_TypeCode *CORBA_TypeCode_ptr;

class CORBA_BOA;
typedef class CORBA_BOA *CORBA_BOA_ptr;

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

// enum values defined in nvlist.hh, bitwise ORed.
typedef u_int CORBA_Flags; 

typedef void (*TAO_Skeleton)(CORBA_ServerRequest &,
			     CORBA_Object_ptr, 
			     CORBA_Environment &);

// Basic types used in the CORBA 2.0 runtime

#include	"corbacom.h"
#include	"except.h"
#include	"object.h"
#include	"sequence.h"
#include	"principa.h"
#include	"typecode.h"
#include	"any.h"
#include	"nvlist.h"
#include	"request.h"
#include	"svrrqst.h"

#if !defined(__IIOP_BUILD)
#include        "boa.h"

// Bootstrapping, etc
#include	"orbobj.h"
#endif

// NOTE: stub APIs are nonportable, and must be explicitly #included
// by code emitted from an IDL compiler.

#if	defined (_MSC_VER)
#	pragma pack (pop)		// VC++, goes back to other padding rules
#endif /* VC++ */

    // BC++ we leave with 1-byte padding rules...

#endif /* TAO_ORB_H */

