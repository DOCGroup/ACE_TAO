// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_TypedEvent.h
 *
 *  $Id$
 *
 *  @author Jon Astle (jon@astle45.fsnet.co.uk)
 */
//=============================================================================

#ifndef TAO_CEC_TYPEDEVENT_H_
#define TAO_CEC_TYPEDEVENT_H_
#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/NVList.h"
#include "tao/CORBA_String.h"
#include "tao/CORBA_methods.h"

// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_CEC_TypedEvent
 *
 * @brief Defines the operation and args list for the TypedEvent
 *
 * Used to store the TypedEvent as it is passed from the supplier
 * side to the consumer side, in the TypedEventChannel.
 */

class TAO_CEC_ProxyPushSupplier;

class TAO_Event_Serv_Export TAO_CEC_TypedEvent
{
public:
  /// constructors...
  TAO_CEC_TypedEvent (void);

  TAO_CEC_TypedEvent (CORBA::NVList_ptr list,
                      const char * operation);

  TAO_CEC_TypedEvent & operator= (const TAO_CEC_TypedEvent &);

private:
  /// Only the ProxyPushSupplier can read the private fields.
  friend class TAO_CEC_ProxyPushSupplier;

  CORBA::NVList_ptr list_;
  CORBA::String_var operation_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_TypedEvent.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CEC_TYPEDEVENT_H_ */
