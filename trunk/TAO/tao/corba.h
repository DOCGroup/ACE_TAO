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

#if !defined(TAO_CORBA_H)
#  define TAO_CORBA_H

#include "tao/orbconf.h"

#include "tao/align.h"
#include "tao/any.h"
#include "tao/boa.h"
#include "tao/cdr.h"
#include "tao/connect.h"
#include "tao/corba.h"
#include "tao/corbacom.h"
#include "tao/debug.h"
#include "tao/except.h"
#include "tao/factories.h"
#include "tao/giop.h"
#include "tao/iiopobj.h"
#include "tao/iioporb.h"
#include "tao/marshal.h"
#include "tao/nvlist.h"
#include "tao/object.h"
#include "tao/objtable.h"
#include "tao/optable.h"
#include "tao/orb.h"
#include "tao/orbobj.h"
#include "tao/params.h"
#include "tao/principa.h"
#include "tao/request.h"
#include "tao/roa.h"
#include "tao/sequence.h"
#include "tao/stub.h"
#include "tao/svrrqst.h"
#include "tao/typecode.h"
#include "tao/xdr.h"

#endif
