// $Id$

#include "RTCORBA_Notify_Service.h"

#if ! defined (__ACE_INLINE__)
#include "RTCORBA_Notify_Service.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RTCORBA_Notify_Service, "$Id$")

#include "Properties.h"
#include "RTCORBA_Properties.h"
#include "RTCORBA_Factory.h"
#include "RTCORBA_Builder.h"

TAO_NS_RTCORBA_Notify_Service::TAO_NS_RTCORBA_Notify_Service (void)
{
}

TAO_NS_RTCORBA_Notify_Service::~TAO_NS_RTCORBA_Notify_Service ()
{
}

void
TAO_NS_RTCORBA_Notify_Service::init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Loading the Real-Time Notification Service...\n"));

  this->init_i (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_RTCORBA_Notify_Service::init_i (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  //init the base class.
  TAO_NS_Notify_Service::init_i (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_NS_RTCORBA_Properties* properties = TAO_NS_RTCORBA_PROPERTIES::instance();

  // Resolve RTORB
  CORBA::Object_var object =
    orb->resolve_initial_references ("RTORB"
				      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  RTCORBA::RTORB_var rt_orb =
    RTCORBA::RTORB::_narrow (object.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Resolve RTCurrent
  object =
    orb->resolve_initial_references ("RTCurrent"
				     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (object.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

 /// Set the properties
  properties->rt_orb (rt_orb.in ());
  properties->current (current.in ());
}

void
TAO_NS_RTCORBA_Notify_Service::init_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (this->factory_,
		    TAO_NS_RTCORBA_Factory (),
		    CORBA::NO_MEMORY ());
  ACE_CHECK;

  TAO_NS_PROPERTIES::instance()->factory (this->factory_);
}

void
TAO_NS_RTCORBA_Notify_Service::init_builder (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (this->builder_,
		    TAO_NS_RTCORBA_Builder (),
		    CORBA::NO_MEMORY ());
  ACE_CHECK;

  TAO_NS_PROPERTIES::instance()->builder (this->builder_);
}

ACE_FACTORY_DEFINE (TAO_RT_Notify,TAO_NS_RTCORBA_Notify_Service)
