// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file corba.h
 *
 *  $Id$
 *
 *  Master header file for all TAO stubs.
 *
 *  @note This header should not be included in any core TAO library
 *        files.  Include the necessary individual headers instead.
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author  Chris Cleeland
 *  @author  Douglas C. Schmidt
 */
//=============================================================================


#ifndef TAO_MASTER_CORBA_H
#define TAO_MASTER_CORBA_H

#include /**/ "ace/pre.h"

// ACE specific includes
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"

// The definitions are included in the same order as they are declared
// in corbafwd.h
#include "tao/Typecode.h"

#include "tao/Environment.h"
#include "tao/Exception.h"

#include "tao/NVList.h"

#include "tao/Object.h"
#include "tao/LocalObject.h"
#include "tao/Principal.h"
#include "tao/ORB.h"

#include "tao/CurrentC.h"
#include "tao/BoundsC.h"
#include "tao/PolicyC.h"
#include "tao/ServicesC.h"
#include "tao/DomainC.h"
#include "tao/WrongTransactionC.h"

#include "tao/ObjectIdListC.h"

#include "tao/StringSeqC.h"
#include "tao/OctetSeqC.h"

// TAO specific files, avoid them as much as possible.

#include "tao/CDR.h"

#include "tao/Managed_Types.h"
#include "tao/Object_KeyC.h"

#include "tao/Remote_Object_Proxy_Impl.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "tao/PortableInterceptorC.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_MASTER_CORBA_H */
