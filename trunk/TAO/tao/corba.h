// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    corba.h
 *
 *  $Id$
 *
 *   Master Header file for the TAO CORBA ORB.  Include only this
 *   file, to see all public ORB interfaces declared by TAO.
 *
 *   This follows the OMG C++ mapping for compilers that don't
 *   support C++ exceptions or namespaces.
 *
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author Chris Cleeland
 *  @author  and Douglas C. Schmidt.
 */
//=============================================================================


#ifndef TAO_MASTER_CORBA_H
#define TAO_MASTER_CORBA_H
#include "ace/pre.h"

// ACE specific includes
#include "ace/config-all.h"

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
#include "tao/TAO_Server_Request.h"
#include "tao/Object.h"
#include "tao/Remote_Object_Proxy_Impl.h"
#include "tao/LocalObject.h"
#include "tao/Typecode.h"

#include "tao/CDR.h"

#include "tao/PolicyC.h"
#include "tao/CurrentC.h"

// TAO specific files, avoid them as much as possible.

#include "tao/Stub.h"
#include "tao/params.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#include "tao/Client_Strategy_Factory.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Invocation.h"

#include "tao/Managed_Types.h"
#include "tao/Sequence.h"
#include "tao/Object_KeyC.h"
#include "tao/ValueBase.h"
#include "tao/ValueFactory.h"
#include "tao/Services.h"
#include "tao/ObjectIDList.h"
#include "tao/DomainC.h"
#include "tao/WrongTransactionC.h"

#ifdef TAO_HAS_INTERFACE_REPOSITORY
// Interface repository
#include "tao/InterfaceC.h"

#endif /*TAO_HAS_INTERFACE_REPOSITORY */

#include "tao/BoundsC.h"
#include "tao/TAOC.h"
#include "tao/MessagingC.h"
#include "tao/StringSeqC.h"
#include "tao/OctetSeqC.h"

#include "tao/PortableInterceptor.h" /* This needs to remain visible. */

#include "tao/ClientRequestInfo.h"

#if !defined (ACE_NESTED_CLASS)
#if defined (ACE_WIN32)
#define ACE_NESTED_CLASS(SCOPE,CLASS) CLASS
#else /* !ACE_WIN32 */
#define ACE_NESTED_CLASS(SCOPE,CLASS) SCOPE::CLASS
#endif /* ACE_WIN32 */
#endif /* ACE_NESTED_CLASS */

#include "ace/post.h"
#endif /* TAO_MASTER_CORBA_H */
