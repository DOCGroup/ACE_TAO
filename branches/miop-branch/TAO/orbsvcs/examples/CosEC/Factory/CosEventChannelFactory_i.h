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

#ifndef TAO_COSEVENTCHANNELFACTORY_I_H
#define TAO_COSEVENTCHANNELFACTORY_I_H

#include "ace/pre.h"

#include "CosEventChannelFactoryS.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_CosEventChannelFactory_i :
  public virtual POA_CosEventChannelFactory::ChannelFactory,
  public virtual PortableServer::RefCountServantBase
{
 public:
  // = Initialization and termination code.
  TAO_CosEventChannelFactory_i (void);
  // Constructor.

  ~TAO_CosEventChannelFactory_i (void);
  // Destructor.

  int init (PortableServer::POA_ptr poa,
            const char* child_poa_name,
            CosNaming::NamingContext_ptr naming = CosNaming::NamingContext::_nil ()
            TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  // This method creates a child poa with <poa> as the
  // parent. It also accepts a Naming_Context which is used to register
  // the event channels if specified.
  // Returns -1 on error, 0 on success.
  // @@ Pradeep: this looks OK. I wonder if it would be a good idea to
  //    raise exceptions, but I'm undecided.
  // @@ Pradeep: when is the child poa destroyed? Maybe we should add
  //    a destroy() method to the factory interface (in IDL).
  // @@ Carlos: if we add a <destroy> to the factory, any client will be
  // able to destroy the factory!
  // @@ Pradeep: it could be a method of the Factory_i class, it
  //    doesn't have to be exposed through the IDL interface. Anyway,
  //    there must be a way to cleanup any resources created by the
  //    factory, and you must avoid CORBA calls in the destructor,
  //    first because you won't have an TAO_ENV_SINGLE_ARG_PARAMETER and second because
  //    exceptions in destructors are evil.
  // @@ Pradeep: anyway you can just use exceptions and not return -1?

  // = CosEventChannelFactory::ChannelFactory methods.
  virtual CosEventChannelAdmin::EventChannel_ptr create
    (
     const char * channel_id,
     CORBA::Boolean store_in_naming_service
     TAO_ENV_ARG_DECL
     )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventChannelFactory::DuplicateChannel,
        CosEventChannelFactory::BindFailed
      ));

  virtual void destroy
    (
     const char * channel_id,
     CORBA::Boolean unbind_from_naming_service
     TAO_ENV_ARG_DECL
     )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventChannelFactory::NoSuchChannel
      ));

  virtual CosEventChannelAdmin::EventChannel_ptr find
    (
     const char * channel_id
     TAO_ENV_ARG_DECL
     )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventChannelFactory::NoSuchChannel
      ));

  virtual char * find_channel_id
    (
     CosEventChannelAdmin::EventChannel_ptr channel
     TAO_ENV_ARG_DECL
     )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventChannelFactory::NoSuchChannel
      ));
 protected:

  PortableServer::POA_var poa_;
  // The Poa with which we activate all the Event Channels.

  CosNaming::NamingContext_var naming_;
  // The naming context to use.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_COSEVENTCHANNELFACTORY_I_H */
