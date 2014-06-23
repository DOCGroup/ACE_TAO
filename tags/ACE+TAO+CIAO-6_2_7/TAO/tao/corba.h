// -*- C++ -*-

//=============================================================================
/**
 *  @file corba.h
 *
 *  $Id$
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

#include "tao/corbafwd.h"

#include "tao/Environment.h"
#include "tao/SystemException.h"
#include "tao/UserException.h"

#include "tao/Object.h"
#include "tao/LocalObject.h"
#include "tao/Principal.h"
#include "tao/ORB.h"

#include "tao/CurrentC.h"
#include "tao/PolicyC.h"
#include "tao/Policy_ManagerC.h"
#include "tao/Policy_CurrentC.h"
#include "tao/ServicesC.h"
#include "tao/WrongTransactionC.h"

#include "tao/ObjectIdListC.h"

#include "tao/BooleanSeqC.h"
#include "tao/CharSeqC.h"
#include "tao/DoubleSeqC.h"
#include "tao/FloatSeqC.h"
#include "tao/LongDoubleSeqC.h"
#include "tao/LongLongSeqC.h"
#include "tao/LongSeqC.h"
#include "tao/OctetSeqC.h"
#include "tao/ShortSeqC.h"
#include "tao/StringSeqC.h"
#include "tao/ULongLongSeqC.h"
#include "tao/ULongSeqC.h"
#include "tao/UShortSeqC.h"
#include "tao/WCharSeqC.h"
#include "tao/WStringSeqC.h"

// TAO specific files, avoid them as much as possible.

#include "tao/CDR.h"
#include "tao/Object_KeyC.h"
#include "tao/String_Manager_T.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "tao/PortableInterceptorC.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_MASTER_CORBA_H */
