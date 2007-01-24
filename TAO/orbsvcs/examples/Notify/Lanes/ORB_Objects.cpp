// $Id$

#include "ORB_Objects.h"

ACE_RCSID (Notify, TAO_Notify_ORB_Objects, "$Id$")

TAO_Notify_ORB_Objects::TAO_Notify_ORB_Objects (void)
{
}

void
TAO_Notify_ORB_Objects::init (CORBA::ORB_var& orb ACE_ENV_ARG_DECL)
{
  this->orb_ = orb;

  CORBA::Object_var object =  this->orb_->resolve_initial_references("RootPOA"
                                                                     ACE_ENV_ARG_PARAMETER);

  this->root_poa_ = PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

  // Resolve the RTORB.
  object = this->orb_->resolve_initial_references ("RTORB"
                                                       ACE_ENV_ARG_PARAMETER);

  this->rt_orb_ = RTCORBA::RTORB::_narrow (object.in ()
                                           ACE_ENV_ARG_PARAMETER);

  // Resolve the Current
  object = this->orb_->resolve_initial_references ("RTCurrent"
                                                   ACE_ENV_ARG_PARAMETER);

  this->current_ = RTCORBA::Current::_narrow (object.in ()
                                              ACE_ENV_ARG_PARAMETER);

  // Resolve the Naming service
  object = this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);

  this->naming_ = CosNaming::NamingContextExt::_narrow (object.in ()
                                                                    ACE_ENV_ARG_PARAMETER);
}

TAO_Notify_ORB_Objects::~TAO_Notify_ORB_Objects ()
{
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_ORB_Objects::notify_factory (void)
{
  CosNotifyChannelAdmin::EventChannelFactory_var ecf;

  // Look for the Notification Service
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup ("NotifyEventChannelFactory");

  CORBA::Object_var object = this->naming_->resolve (name ACE_ENV_ARG_PARAMETER);

  ecf = CosNotifyChannelAdmin::EventChannelFactory::_narrow (object.in() ACE_ENV_ARG_PARAMETER);

  return ecf._retn ();
}
