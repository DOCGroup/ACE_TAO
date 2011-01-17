// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/FT_ProxyAdmin_Base.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/PortableServer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

FT_ProxyAdmin_Base::FT_ProxyAdmin_Base(PortableServer::ServantBase* servant,
                           PortableServer::POA_var poa)
  : servant_(servant), poa_(poa)
{
}

FT_ProxyAdmin_Base::~FT_ProxyAdmin_Base()
{
}

void
FT_ProxyAdmin_Base::activate(const FtRtecEventComm::ObjectId& oid)
{
  poa_->activate_object_with_id(
    reinterpret_cast<const PortableServer::ObjectId&> (oid),
    servant_);
  object_id_ = oid;
}


const FtRtecEventComm::ObjectId&
FT_ProxyAdmin_Base::object_id(void) const
{
  return object_id_;
}


CORBA::Object_var
FT_ProxyAdmin_Base::reference(void) const
{
  return poa_->servant_to_reference(servant_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
