// -*- C++ -*-

//=============================================================================
/**
 *  @file    CosEventChannelFactory_i.h
 *
 *  This class implements the CosEventChannelFactory
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_COSEVENTCHANNELFACTORY_I_H
#define TAO_COSEVENTCHANNELFACTORY_I_H

#include /**/ "ace/pre.h"

#include "CosEventChannelFactoryS.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_CosEventChannelFactory_i :
  public virtual POA_CosEventChannelFactory::ChannelFactory
{
public:
  /// Constructor.
  TAO_CosEventChannelFactory_i ();

  /// Destructor.
  ~TAO_CosEventChannelFactory_i () override = default;
  TAO_CosEventChannelFactory_i (TAO_CosEventChannelFactory_i const &) = delete;
  TAO_CosEventChannelFactory_i (TAO_CosEventChannelFactory_i &&) = delete;
  TAO_CosEventChannelFactory_i &operator= (TAO_CosEventChannelFactory_i const &) = delete;
  TAO_CosEventChannelFactory_i &operator= (TAO_CosEventChannelFactory_i &&) = delete;

  /**
   * This method creates a child poa with <poa> as the
   * parent. It also accepts a Naming_Context which is used to register
   * the event channels if specified.
   * Returns -1 on error, 0 on success.
   * @@ Pradeep: this looks OK. I wonder if it would be a good idea to
   *    raise exceptions, but I'm undecided.
   * @@ Pradeep: when is the child poa destroyed? Maybe we should add
   *    a destroy() method to the factory interface (in IDL).
   * @@ Carlos: if we add a <destroy> to the factory, any client will be
   * able to destroy the factory!
   * @@ Pradeep: it could be a method of the Factory_i class, it
   *    doesn't have to be exposed through the IDL interface. Anyway,
   *    there must be a way to cleanup any resources created by the
   *    factory, and you must avoid CORBA calls in the destructor,
   *    first because you won't have an and second because
   *    exceptions in destructors are evil.
   * @@ Pradeep: anyway you can just use exceptions and not return -1?
   */
  int init (PortableServer::POA_ptr poa,
            char const *child_poa_name,
            CosNaming::NamingContext_ptr naming = CosNaming::NamingContext::_nil ());

  // = CosEventChannelFactory::ChannelFactory methods.
  CosEventChannelAdmin::EventChannel_ptr create (char const *channel_id,
                                                  CORBA::Boolean store_in_naming_service) override;

  void destroy (char const *channel_id,
                CORBA::Boolean unbind_from_naming_service) override;

  CosEventChannelAdmin::EventChannel_ptr find (char const *channel_id) override;

  char *find_channel_id (CosEventChannelAdmin::EventChannel_ptr channel) override;
protected:
  /// The POA with which we activate all the Event Channels.
  PortableServer::POA_var poa_;

  /// The naming context to use.
  CosNaming::NamingContext_var naming_;
};

#include /**/ "ace/post.h"

#endif /* TAO_COSEVENTCHANNELFACTORY_I_H */
