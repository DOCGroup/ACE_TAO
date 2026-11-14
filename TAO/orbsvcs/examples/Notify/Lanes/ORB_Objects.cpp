#include "ORB_Objects.h"

void
TAO_Notify_ORB_Objects::init (CORBA::ORB_var const& orb)
{
  this->orb_ = orb;

  CORBA::Object_var object {};
  object = this->orb_->resolve_initial_references ("RootPOA");

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

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_ORB_Objects::notify_factory () const
{
  // Look for the Notification Service
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup ("NotifyEventChannelFactory");

  CORBA::Object_var const object = this->naming_->resolve (name);
  return CosNotifyChannelAdmin::EventChannelFactory::_narrow (object.in ());
}
