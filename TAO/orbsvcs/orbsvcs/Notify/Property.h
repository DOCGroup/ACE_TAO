/* -*- C++ -*- */
/**
 *  @file Property.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PROPERTY_H
#define TAO_NS_PROPERTY_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "tao/orbconf.h"
#include "tao/TimeBaseC.h"
#include "orbsvcs/NotifyExtC.h"

template <class LOCK, class TYPE> class ACE_Atomic_Op;
template <class TYPE> class TAO_NS_Property_T;
template <class TYPE> class TAO_NS_StructProperty_T;

typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX,CORBA::Long> TAO_NS_Atomic_Property_Long;
typedef TAO_NS_Property_T<CORBA::Long> TAO_NS_Property_Long;
typedef TAO_NS_Property_T<CORBA::Short> TAO_NS_Property_Short;
typedef TAO_NS_Property_T<TimeBase::TimeT> TAO_NS_Property_Time;
typedef TAO_NS_StructProperty_T<NotifyExt::ThreadPoolParams> TAO_NS_Property_ThreadPool;
typedef TAO_NS_StructProperty_T<NotifyExt::ThreadPoolLanesParams> TAO_NS_Property_ThreadPoolLanes;

#include "ace/post.h"
#endif /* TAO_NS_PROPERTY_H */
