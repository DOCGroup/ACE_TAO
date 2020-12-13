// -*- C++ -*-

//=============================================================================
/**
 *  @file corba.h
 *
 *  @note This header is for backwards compatibility only. It pulls
 *        in a lot of files increasing compilation time and footprint
 *        size. Include the necessary individual headers instead.
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

#include "corbafwd.h"

#include "Environment.h"
#include "SystemException.h"
#include "UserException.h"

#include "Object.h"
#include "LocalObject.h"
#include "Principal.h"
#include "ORB.h"

#include "CurrentC.h"
#include "PolicyC.h"
#include "Policy_ManagerC.h"
#include "Policy_CurrentC.h"
#include "ServicesC.h"
#include "WrongTransactionC.h"

#include "ObjectIdListC.h"

#include "BooleanSeqC.h"
#include "CharSeqC.h"
#include "DoubleSeqC.h"
#include "FloatSeqC.h"
#include "LongDoubleSeqC.h"
#include "LongLongSeqC.h"
#include "LongSeqC.h"
#include "OctetSeqC.h"
#include "ShortSeqC.h"
#include "StringSeqC.h"
#include "ULongLongSeqC.h"
#include "ULongSeqC.h"
#include "UShortSeqC.h"
#include "WCharSeqC.h"
#include "WStringSeqC.h"

// TAO specific files, avoid them as much as possible.

#include "CDR.h"
#include "Object_KeyC.h"
#include "String_Manager_T.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "PortableInterceptorC.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_MASTER_CORBA_H */
