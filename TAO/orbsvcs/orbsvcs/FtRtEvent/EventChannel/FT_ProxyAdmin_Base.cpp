#include "orbsvcs/FtRtEvent/EventChannel/FT_ProxyAdmin_Base.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

FT_ProxyAdmin_Base::FT_ProxyAdmin_Base(PortableServer::ServantBase* servant,
                           PortableServer::POA_var poa)
  : servant_(servant), poa_(poa)
{
}

void
FT_ProxyAdmin_Base::activate(FtRtecEventComm::ObjectId const& oid)
{
  poa_->activate_object_with_id(
    reinterpret_cast<PortableServer::ObjectId const&> (oid),
    servant_);
  object_id_ = oid;
}

FtRtecEventComm::ObjectId const&
FT_ProxyAdmin_Base::object_id() const
{
  return object_id_;
}

CORBA::Object_var
FT_ProxyAdmin_Base::reference() const
{
  return poa_->servant_to_reference(servant_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
