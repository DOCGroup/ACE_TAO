// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file corba.h
 *
 *  $Id$
 *
 *   Master Header file for the TAO CORBA ORB.  Include only this
 *   file, to see all public ORB interfaces declared by TAO.
 *   @par
 *   This follows the OMG C++ mapping for compilers that don't
 *   support C++ exceptions or namespaces.
 *
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author  Chris Cleeland
 *  @author  Douglas C. Schmidt
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

// The definitions are included in the same order as they are declared
// in corbafwd.h
#include "tao/Typecode.h"
#include "tao/Any.h"

#include "tao/Environment.h"
#include "tao/Exception.h"

#include "tao/NVList.h"

#include "tao/Object.h"
#include "tao/LocalObject.h"
#include "tao/Principal.h"
#include "tao/ORB.h"

#include "tao/CurrentC.h"
#include "tao/BoundsC.h"
#include "tao/ValueBase.h"
#include "tao/ValueFactory.h"
#include "tao/PolicyC.h"
#include "tao/Services.h"
#include "tao/DomainC.h"
#include "tao/WrongTransactionC.h"

// TAO specific files, avoid them as much as possible.

#include "tao/CDR.h"

#include "tao/Sequence.h"
#include "tao/Managed_Types.h"
#include "tao/Object_KeyC.h"
#include "tao/ObjectIDList.h"

#include "tao/Remote_Object_Proxy_Impl.h"

#include "tao/StringSeqC.h"
#include "tao/OctetSeqC.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "tao/PortableInterceptorC.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#if !defined (ACE_NESTED_CLASS)
#if defined (ACE_WIN32)
#define ACE_NESTED_CLASS(SCOPE,CLASS) CLASS
#else /* !ACE_WIN32 */
#define ACE_NESTED_CLASS(SCOPE,CLASS) SCOPE::CLASS
#endif /* ACE_WIN32 */
#endif /* ACE_NESTED_CLASS */

#include "ace/post.h"
#endif /* TAO_MASTER_CORBA_H */
