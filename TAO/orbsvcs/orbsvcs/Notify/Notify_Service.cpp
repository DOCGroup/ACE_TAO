// $Id$

#include "Notify_Service.h"
#include "tao/ORB_Core.h"
#include "Builder.h"
#include "Properties.h"
#include "Factory.h"

ACE_RCSID(RT_Notify, TAO_NS_Notify_Service, "$Id$")

TAO_NS_Notify_Service::TAO_NS_Notify_Service (void)
{
}

TAO_NS_Notify_Service::~TAO_NS_Notify_Service ()
{
  delete factory_;
  delete builder_;
}

void
TAO_NS_Notify_Service::init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Loading the Cos Notification Service...\n"));

  this->init_i (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Notify_Service::init_i (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  // Obtain the Root POA
  CORBA::Object_var object  =
    orb->resolve_initial_references("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (object.in ()))
    ACE_ERROR ((LM_ERROR,
                       " (%P|%t) Unable to resolve the RootPOA.\n"));

  PortableServer::POA_var default_poa = PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  /// Set the properties
  TAO_NS_Properties* properties = TAO_NS_PROPERTIES::instance();
  
  properties->default_poa (default_poa.in ());
  properties->sched_policy (orb->orb_core ()->orb_params ()->sched_policy ());
  properties->scope_policy (orb->orb_core ()->orb_params ()->scope_policy ());

  // Init the factory and builder

  this->init_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->init_builder (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Notify_Service::init_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (this->factory_,
		    TAO_NS_Factory (),
		    CORBA::NO_MEMORY ());
  ACE_CHECK;

  TAO_NS_PROPERTIES::instance()->factory (this->factory_);
}

void
TAO_NS_Notify_Service::init_builder (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (this->builder_,
		    TAO_NS_Builder (),
		    CORBA::NO_MEMORY ());
  ACE_CHECK;

  TAO_NS_PROPERTIES::instance()->builder (this->builder_);
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_NS_Notify_Service::create (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  return this->builder_->build_event_channel_factory (poa ACE_ENV_ARG_PARAMETER);
}

ACE_FACTORY_DEFINE (TAO_Notify, TAO_NS_Notify_Service)
