// $Id$

#include "RT_Notify_Service.h"

#if ! defined (__ACE_INLINE__)
#include "RT_Notify_Service.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RT_Notify_Service, "$Id$")

#include "ace/Dynamic_Service.h"
#include "Properties.h"
#include "RT_Properties.h"
#include "RT_Factory.h"
#include "RT_Builder.h"

TAO_RT_Notify_Service::TAO_RT_Notify_Service (void)
{
}

TAO_RT_Notify_Service::~TAO_RT_Notify_Service ()
{
}

int
TAO_RT_Notify_Service::init (int argc, char *argv[])
{
  return TAO_CosNotify_Service::init (argc, argv);
}

void
TAO_RT_Notify_Service::init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Loading the Real-Time Notification Service...\n"));

  this->init_i (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_RT_Notify_Service::init_i (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  //Init the base class.
  TAO_CosNotify_Service::init_i (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_NS_RT_Properties* properties = TAO_NS_RT_PROPERTIES::instance();

  // Resolve RTORB
  CORBA::Object_var object =
    orb->resolve_initial_references ("RTORB"
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTCORBA::RTORB_var rt_orb =
    RTCORBA::RTORB::_narrow (object.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Resolve RTCurrent
  object =
    orb->resolve_initial_references ("RTCurrent"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (object.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

 /// Set the properties
  properties->rt_orb (rt_orb.in ());
  properties->current (current.in ());
}

void
TAO_RT_Notify_Service::init_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  this->factory_ = ACE_Dynamic_Service<TAO_NS_Factory>::instance ("TAO_NS_Factory");

  if (this->factory_ == 0)
    {
      ACE_NEW_THROW_EX (this->factory_,
                        TAO_NS_RT_Factory (),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;
    }

  TAO_NS_PROPERTIES::instance()->factory (this->factory_);
}

void
TAO_RT_Notify_Service::init_builder (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (this->builder_,
                    TAO_NS_RT_Builder (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  TAO_NS_PROPERTIES::instance()->builder (this->builder_);
}

ACE_FACTORY_DEFINE (TAO_RT_Notify,TAO_RT_Notify_Service)
