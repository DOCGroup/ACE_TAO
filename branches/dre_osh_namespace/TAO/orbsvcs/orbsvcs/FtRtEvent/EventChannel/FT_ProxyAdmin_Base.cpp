// $Id$

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "FT_ProxyAdmin_T.h"

ACE_RCSID (EventChannel,
           FT_ProxyAdmin_Base,
           "$Id$")

FT_ProxyAdmin_Base::FT_ProxyAdmin_Base(PortableServer::ServantBase* servant,
                           PortableServer::POA_var poa)
: servant_(servant), poa_(poa)
{
}

FT_ProxyAdmin_Base::~FT_ProxyAdmin_Base()
{
}

void FT_ProxyAdmin_Base::activate(const PortableServer::ObjectId& oid
              ACE_ENV_ARG_DECL)
{
  poa_->activate_object_with_id(oid, servant_
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  object_id_ = oid;
}


const PortableServer::ObjectId& FT_ProxyAdmin_Base::object_id(ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return object_id_;
}


CORBA::Object_var FT_ProxyAdmin_Base::reference(ACE_ENV_SINGLE_ARG_DECL) const
{
  return  poa_->servant_to_reference(servant_
                                ACE_ENV_ARG_PARAMETER);
}



