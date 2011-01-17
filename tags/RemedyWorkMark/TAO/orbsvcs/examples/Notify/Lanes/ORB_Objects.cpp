// $Id$

#include "ORB_Objects.h"



TAO_Notify_ORB_Objects::TAO_Notify_ORB_Objects (void)
{
}

void
TAO_Notify_ORB_Objects::init (CORBA::ORB_var& orb)
{
  this->orb_ = orb;

  CORBA::Object_var object =  this->orb_->resolve_initial_references("RootPOA");

  this->root_poa_ = PortableServer::POA::_narrow (object.in ());

  // Resolve the RTORB.
  object = this->orb_->resolve_initial_references ("RTORB");

  this->rt_orb_ = RTCORBA::RTORB::_narrow (object.in ());

  // Resolve the Current
  object = this->orb_->resolve_initial_references ("RTCurrent");

  this->current_ = RTCORBA::Current::_narrow (object.in ());

  // Resolve the Naming service
  object = this->orb_->resolve_initial_references ("NameService");

  this->naming_ = CosNaming::NamingContextExt::_narrow (object.in ());
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

  CORBA::Object_var object = this->naming_->resolve (name);

  ecf = CosNotifyChannelAdmin::EventChannelFactory::_narrow (object.in());

  return ecf._retn ();
}
