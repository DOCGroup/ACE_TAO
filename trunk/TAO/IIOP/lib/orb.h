// This may look like C, but it's really -*- C++ -*-
//
// @(#) $Id$

/*

COPYRIGHT AND LICENSING
-----------------------
This notice applies to all files in this software distribution. 
Please read it!


Copyright 1995 Sun Microsystems, Inc.
Printed in the United States of America.
All Rights Reserved.

This software product (LICENSED PRODUCT), implementing the Object Management
Group's "Internet Inter-ORB Protocol", is protected by copyright and is
distributed under the following license restricting its use.  Portions of
LICENSED PRODUCT may be protected by one or more U.S. or foreign patents, or
pending applications.

LICENSED PRODUCT is made available for your use provided that you include
this license and copyright notice on all media and documentation and the
software program in which this product is incorporated in whole or part.

You may copy, modify, distribute, or sublicense the LICENCED PRODUCT without
charge as part of a product or software program developed by you, so long as
you preserve the functionality of interoperating with the Object Management
Group's "Internet Inter-ORB Protocol" version one.  However, any uses other
than the foregoing uses shall require the express written consent of Sun
Microsystems, Inc.

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of the LICENSED PRODUCT as permitted herein.

This license is effective until terminated by Sun for failure to comply with
this license.  Upon termination, you shall destroy or return all code and
documentation for the LICENSED PRODUCT.

LICENSED PRODUCT IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF DEALING, USAGE OR
TRADE PRACTICE.

LICENSED PRODUCT IS PROVIDED WITH NO SUPPORT AND WITHOUT ANY OBLIGATION ON
THE PART OF SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES TO ASSIST IN ITS
USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
LICENSED PRODUCT OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to restrictions
as set forth in subparagraph (c)(1)(ii) of the Rights in Technical Data and
Computer Software clause at DFARS 252.227-7013 and FAR 52.227-19.

SunOS, SunSoft, Sun, Solaris, Sun Microsystems and the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

SunSoft, Inc.  
2550 Garcia Avenue 
Mountain View, California  94043

*/

//
// MASTER HEADER file for C/C++ Win32 interface to a CORBA 2.0 ORB.
// Include only this file, to see all ORB interfaces declared.
//
// This interface uses COM as the basis for its binary standard, and
// follows the OMG C++ mapping for compilers which don't support C++
// exceptions (COM doesn't use them) or namespaces.  Objects which
// are implemented using this source base support OLE Automation.
//
// XXX Note re status of this as reference, cosubmitted with RFP?
//
#ifndef	_CORBA_orb_hh
#define	_CORBA_orb_hh

#include <stdlib.h>

//
// For some reason, PC compilers don't implement "natural" alignment, but
// only use fixed alignment policies.  The following #pragmas configure
// fixed one-byte alignment policy, since some fixed policy needs to apply
// throughout an ORB.
//
#if	defined (_MSC_VER)
#	pragma	pack (push, 1)		// VC++, stack 1-byte alignment policy

#	ifdef	_DEBUG			// convert from VC++ convention ...
#		define	DEBUG		// ... to normal convention
#	endif

#elif	defined (__BORLANDC__)
#	pragma option -a		// BC++, use 1 byte alignment

#endif

//
// Get various definitions facilitating portability.
//
#include "orbconf.h"

//
// Forward declarations of some data types are needed.
//
class CORBA_Any;
typedef class CORBA_Any 		*CORBA_Any_ptr;

class CORBA_TypeCode;
typedef class CORBA_TypeCode		*CORBA_TypeCode_ptr;

class CORBA_BOA;
typedef class CORBA_BOA                  *CORBA_BOA_ptr;

class CORBA_Exception;
typedef class CORBA_Exception           *CORBA_Exception_ptr;

class CORBA_Request;
typedef class CORBA_Request 		*CORBA_Request_ptr;

class CORBA_NamedValue;
typedef class CORBA_NamedValue          *CORBA_NamedValue_ptr;

class CORBA_NVList;
typedef class CORBA_NVList              *CORBA_NVList_ptr;

class CORBA_ORB;
typedef class CORBA_ORB                 *CORBA_ORB_ptr;

class CORBA_Object;
typedef class CORBA_Object              *CORBA_Object_ptr;

class CORBA_ServerRequest;
typedef class CORBA_ServerRequest 	*CORBA_ServerRequest_ptr;

class CORBA_Environment;
typedef class CORBA_Environment 	*CORBA_Environment_ptr;

class CORBA_Principal;
typedef class CORBA_Principal 	        *CORBA_Principal_ptr;


typedef class CORBA_ImplementationDef 	*CORBA_ImplementationDef_ptr;

typedef class CORBA_InterfaceDef 	*CORBA_InterfaceDef_ptr;

class CORBA_String_var;

typedef unsigned CORBA_Flags;	        // enum values defined in nvlist.hh,
                                        // bitwise ORed



typedef void (*TAO_Skeleton)(CORBA_ServerRequest &, CORBA_Object_ptr, CORBA_Environment &);
//
// Basic types used in the CORBA 2.0 runtime
//
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
#include        "boa.h"

//
// Bootstrapping, etc
//
#include	"orbobj.h"


// NOTE:  stub APIs are nonportable, and must be explicitly #included
// by code emitted from an IDL compiler.


#if	defined (_MSC_VER)
#	pragma pack (pop)		// VC++, goes back to other padding rules
#endif	// VC++

    // BC++ we leave with 1-byte padding rules...

#endif	// _CORBA_orb_hh

