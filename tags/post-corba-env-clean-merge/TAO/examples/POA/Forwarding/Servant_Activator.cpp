// $Id$

#include "Servant_Activator.h"
#include "MyFooServant.h"

ACE_RCSID(Forwarding, Servant_Activator, "$Id$")

MyFooServantActivator::MyFooServantActivator (CORBA::ORB_ptr orb,
                                              CORBA::Object_ptr forward_to)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    forward_to_ (CORBA::Object::_duplicate (forward_to))
{
}

PortableServer::Servant
MyFooServantActivator::incarnate (const PortableServer::ObjectId &,
                                  PortableServer::POA_ptr
                                  TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  this->orb_->shutdown (0
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Throw forward exception
  ACE_THROW_RETURN (
    PortableServer::ForwardRequest (
      this->forward_to_.in ()),
    0);
}

void
MyFooServantActivator::etherealize (const PortableServer::ObjectId &,
                                    PortableServer::POA_ptr,
                                    PortableServer::Servant servant,
                                    CORBA::Boolean,
                                    CORBA::Boolean
                                    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  delete servant;
}

void
MyFooServantActivator::forward_requests (TAO_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->forward_to_.in ()))
    ACE_THROW (Foo::Cannot_Forward ());
}
