// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/FT_ProxyAdmin_Base.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/PortableServer.h"


ACE_RCSID (EventChannel,
           FT_ProxyAdmin_Base,
           "$Id$")

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
FT_ProxyAdmin_Base::activate(const FtRtecEventComm::ObjectId& oid
                             ACE_ENV_ARG_DECL)
{
  poa_->activate_object_with_id(
    reinterpret_cast<const PortableServer::ObjectId&> (oid), 
    servant_
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  object_id_ = oid;
}


const FtRtecEventComm::ObjectId&
FT_ProxyAdmin_Base::object_id(ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return object_id_;
}


CORBA::Object_var
FT_ProxyAdmin_Base::reference(ACE_ENV_SINGLE_ARG_DECL) const
{
  return poa_->servant_to_reference(servant_
                                    ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
