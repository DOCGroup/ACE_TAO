/* -*- C++ -*- */
/**
 *  @file   EC_Event_Channel.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Marina Spivak (marina@atdesk.com)
 *  @author Jason Smith (jason@atdesk.com)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_EVENT_CHANNEL_H
#define TAO_EC_EVENT_CHANNEL_H
#include /**/ "ace/pre.h"

#include "EC_Event_Channel_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_EC_Event_Channel
 *
 * @brief The RtecEventChannelAdmin::EventChannel implementation.
 *
 * This class is the Mediator between all the classes in the EC
 * implementation, its main task is to redirect the messages to
 * the right components, to hold and manage the lifetime of the
 * long lived objects (Timer_Module, SupplierAdmin,
 * ConsumerAdmin and Dispatching) and to provide a simpler
 * interface to the EC_Factory.
 */
class TAO_RTEvent_Export TAO_EC_Event_Channel :
  public TAO_EC_Event_Channel_Base
{
public:
  /**
   * constructor
   * If @a own_factory is not 0 it assumes ownership of the factory.
   * If the @a factory is nil it uses the Service_Configurator to load
   * the Factory, if not found it uses TAO_EC_Default_Factory
   */
  TAO_EC_Event_Channel (const TAO_EC_Event_Channel_Attributes& attributes,
                        TAO_EC_Factory* factory = 0,
                        int own_factory = 0);
};

#if defined (__ACE_INLINE__)
#include "EC_Event_Channel.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_EVENT_CHANNEL_H */
