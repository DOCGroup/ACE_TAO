
// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/examples/CosEC/Factory
//
// = FILENAME
//    CosEventChannelFactory_i.h
//
// = DESCRIPTION
//    This class implements the CosEventChannelFactory
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_COSEVENTCHANNELFACTORY_H
#define TAO_COSEVENTCHANNELFACTORY_H

#include "CosEventChannelFactoryS.h"
#include "orbsvcs/CosNamingS.h"
#include "orbsvcs/CosNamingC.h"

class TAO_CosEventChannelFactory_i :
  public virtual POA_CosEventChannelFactory::ChannelFactory,
  public virtual TAO_RefCountServantBase
{
 public:
  // = Initialization and termination code.
  TAO_CosEventChannelFactory_i (void);
  // Constructor.

  ~TAO_CosEventChannelFactory_i (void);
  // Destructor.

  int init (PortableServer::POA_ptr poa,
            CosNaming::NamingContext_ptr naming = CosNaming::NamingContext::_nil (),
            CORBA::Environment &ACE_TRY_ENV =  CORBA::Environment::default_environment ());
  // This method creates creates a child poa with <poa> as the
  // parent. It also accepts a Naming_Context which is used to register
  // the event channels if specified.
  // Returns -1 on error, 0 on success.

  // = CosEventChannelFactory::ChannelFactory methods.
  virtual CosEventChannelAdmin::EventChannel_ptr create
    (
     const char * channel_id,
      CORBA::Boolean store_in_naming_service,
     CORBA::Environment &ACE_TRY_ENV
     );

  virtual void destroy
    (
     const char * channel_id,
     CORBA::Boolean unbind_from_naming_service,
      CORBA::Environment &ACE_TRY_ENV
     );

  virtual CosEventChannelAdmin::EventChannel_ptr find
    (
     const char * channel_id,
     CORBA::Environment &ACE_TRY_ENV
     );

  virtual char * find_channel_id
    (
     CosEventChannelAdmin::EventChannel_ptr channel,
     CORBA::Environment &ACE_TRY_ENV
     );

 protected:

  PortableServer::POA_var poa_;
  // The Poa with which we activate all the Event Channels.

  CosNaming::NamingContext_var naming_;
  // The naming context to use.
};
#endif /* TAO_COSEVENTCHANNELFACTORY_H */
