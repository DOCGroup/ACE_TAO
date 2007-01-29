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

#ifndef TAO_Notify_PROPERTY_H
#define TAO_Notify_PROPERTY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/NotifyExtC.h"

#include "tao/orbconf.h"
#include "tao/TimeBaseC.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
template <class LOCK, class TYPE> class ACE_Atomic_Op;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TYPE> class TAO_Notify_Property_T;
template <class TYPE> class TAO_Notify_StructProperty_T;

typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX,CORBA::Long> TAO_Notify_Atomic_Property_Long;
typedef TAO_Notify_Property_T<CORBA::Long> TAO_Notify_Property_Long;
typedef TAO_Notify_Property_T<CORBA::ULong> TAO_Notify_Property_ULong;
typedef TAO_Notify_Property_T<CORBA::Short> TAO_Notify_Property_Short;
typedef TAO_Notify_Property_T<TimeBase::TimeT> TAO_Notify_Property_Time;
typedef TAO_Notify_StructProperty_T<NotifyExt::ThreadPoolParams> TAO_Notify_Property_ThreadPool;
typedef TAO_Notify_StructProperty_T<NotifyExt::ThreadPoolLanesParams> TAO_Notify_Property_ThreadPoolLanes;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_PROPERTY_H */
