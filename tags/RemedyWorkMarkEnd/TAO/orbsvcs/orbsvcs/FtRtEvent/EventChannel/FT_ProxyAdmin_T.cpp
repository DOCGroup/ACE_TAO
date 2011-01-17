// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/IOGR_Maker.h"
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Service.h"
#include "tao/PortableServer/POAC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class EC_PROXY_ADMIN, class Proxy,
          class ProxyInterface, class State>
FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface, State>::FT_ProxyAdmin(
  EC_PROXY_ADMIN* admin, PortableServer::POA_var poa)
  : FT_ProxyAdmin_Base(admin, poa), admin_(admin)
{
}

template <class EC_PROXY_ADMIN, class Proxy,
          class ProxyInterface, class State>
void
FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface,State>::obtain_proxy (
  const FtRtecEventChannelAdmin::Operation& op)
{
  Request_Context_Repository().set_object_id(op.object_id);

  ProxyInterface_var result =  admin_->obtain();

  FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
  try{
    ACE_READ_GUARD (FTRTEC::Replication_Service, locker, *svc);

    svc->replicate_request(op,
      Proxy::rollback_obtain);
  }
  catch (...){
    admin_->disconnect(result.in());
    throw;
  }
}

template <class EC_PROXY_ADMIN, class Proxy,
          class ProxyInterface, class State>
typename FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface, State>::ProxyInterface_ptr
FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface, State>::obtain_proxy (void)
{
  CORBA::Any_var any = Request_Context_Repository().get_cached_result();

  CORBA::Object_var obj;
  if (any >>= CORBA::Any::to_object(obj))
    return ProxyInterface::_narrow(obj.in());

  FtRtecEventChannelAdmin::ObjectId oid;
  Request_Context_Repository().generate_object_id(oid);

  FtRtecEventChannelAdmin::Operation update;

  update.object_id = oid;
  update.param._d(EC_PROXY_ADMIN::OBTAIN_ID);

  ProxyInterface_var result
    =  admin_->obtain();

  try{
    FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
    ACE_READ_GUARD_RETURN (FTRTEC::Replication_Service, locker, *svc, 0);
    obj = IOGR_Maker::instance()->forge_iogr(result.in());

    result = ProxyInterface::_narrow(obj.in());

    svc->replicate_request(update,
      Proxy::rollback_obtain);
  }
  catch (...){
    admin_->disconnect(result.in());
    throw;
  }

  return result._retn();
}


template <class EC_PROXY_ADMIN, class Proxy,
          class ProxyInterface, class State>
void FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface, State>::get_state(
  State& state)
{
  typename EC_PROXY_ADMIN::StateWorker worker(state.proxies);
  admin_->for_each(&worker);
}

template <class EC_PROXY_ADMIN, class Proxy,
          class ProxyInterface, class State>
void FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface, State>::set_state(
  const State& state)
{
  for (size_t i =0; i < state.proxies.length(); ++i)
  {
    const typename Proxy::State& proxy_state
      = state.proxies[i];

    Request_Context_Repository().set_object_id(proxy_state.object_id);

    ProxyInterface_var
      proxy_ior = admin_->obtain();

    typedef typename Proxy::Skeleton Skeleton;
    const void *temp = &proxy_state.object_id;
    const PortableServer::ObjectId *oid =
      reinterpret_cast<const PortableServer::ObjectId *> (temp);
    const PortableServer::Servant servant =
      poa_->id_to_servant (*oid);

    Skeleton skeleton = dynamic_cast<Skeleton> (servant);

    static_cast<Proxy*> (skeleton)->set_state(proxy_state);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
