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

// ACE specific includes
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"

#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/Exception.h"
#include "tao/Any.h"
#include "tao/NVList.h"
#include "tao/Principal.h"
#include "tao/Request.h"
#include "tao/Server_Request.h"
#include "tao/Object.h"
#include "tao/Context.h"
#include "tao/Typecode.h"

#include "tao/varout.h"
#include "tao/Marshal.h"
#include "tao/singletons.h"
#include "tao/CDR.h"

#include "tao/PolicyC.h"
#include "tao/CurrentC.h"
#include "tao/POA.h"

// TAO specific files, avoid them as much as possible.
#include "tao/Stub.h"
#include "tao/params.h"
#include "tao/ORB_Core.h"
#include "tao/Operation_Table.h"
#include "tao/debug.h"

#include "tao/Client_Strategy_Factory.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Active_Object_Map.h"
#include "tao/GIOP.h"
#include "tao/Invocation.h"

// Dynamic Any includes
#include "tao/InconsistentTypeCodeC.h"
#include "tao/DynAny_i.h"

#include "tao/Managed_Types.h"
#include "tao/Sequence.h"
#include "tao/Sequence_T.h"
#include "tao/Object_KeyC.h"
#include "tao/Union.h"

#if !defined (ACE_NESTED_CLASS)
#if defined (ACE_WIN32)
#define ACE_NESTED_CLASS(SCOPE,CLASS) CLASS
#else /* !ACE_WIN32 */
#define ACE_NESTED_CLASS(SCOPE,CLASS) SCOPE::CLASS
#endif /* ACE_WIN32 */
#endif /* ACE_NESTED_CLASS */

#endif /* TAO_MASTER_CORBA_H */
