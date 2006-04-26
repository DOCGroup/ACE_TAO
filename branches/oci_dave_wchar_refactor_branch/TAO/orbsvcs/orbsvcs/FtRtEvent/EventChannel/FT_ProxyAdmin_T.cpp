// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/IOGR_Maker.h"
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Service.h"
#include "ace/Synch_T.h"

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
  const FtRtecEventChannelAdmin::Operation& op
  ACE_ENV_ARG_DECL)
{
  Request_Context_Repository().set_object_id(op.object_id
                                     ACE_ENV_ARG_PARAMETER);

  ProxyInterface_var result
    =  admin_->obtain(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
  ACE_TRY   {
    ACE_Read_Guard<FTRTEC::Replication_Service> locker(*svc);

    svc->replicate_request(op,
      Proxy::rollback_obtain
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHALL {
    admin_->disconnect(result.in());
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK;
}

template <class EC_PROXY_ADMIN, class Proxy,
          class ProxyInterface, class State>
typename FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface, State>::ProxyInterface_ptr
FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface, State>::obtain_proxy (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Any_var any = Request_Context_Repository().get_cached_result(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  CORBA::Object_var obj;
  if (any >>= CORBA::Any::to_object(obj))
    return ProxyInterface::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);

  FtRtecEventChannelAdmin::ObjectId oid;
  Request_Context_Repository().generate_object_id(oid
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  FtRtecEventChannelAdmin::Operation update;

  update.object_id = oid;
  update.param._d(EC_PROXY_ADMIN::OBTAIN_ID);

  ProxyInterface_var result
    =  admin_->obtain(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  ACE_TRY {
    FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
    ACE_Read_Guard<FTRTEC::Replication_Service> locker(*svc);
    obj = IOGR_Maker::instance()->forge_iogr(result.in()
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    result = ProxyInterface::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    svc->replicate_request(update,
      Proxy::rollback_obtain
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHALL {
    admin_->disconnect(result.in());
    ACE_RE_THROW;
  }
  ACE_ENDTRY;

  return result._retn();
}


template <class EC_PROXY_ADMIN, class Proxy,
          class ProxyInterface, class State>
void FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface, State>::get_state(
  State& state
  ACE_ENV_ARG_DECL)
{
  typename EC_PROXY_ADMIN::StateWorker worker(state.proxies);
  admin_->for_each(&worker ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

template <class EC_PROXY_ADMIN, class Proxy,
          class ProxyInterface, class State>
void FT_ProxyAdmin<EC_PROXY_ADMIN, Proxy, ProxyInterface, State>::set_state(
  const State& state
  ACE_ENV_ARG_DECL)
{
  for (size_t i =0; i < state.proxies.length(); ++i)
  {
    const typename Proxy::State& proxy_state
      = state.proxies[i];

    Request_Context_Repository().set_object_id(proxy_state.object_id
                                       ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    ProxyInterface_var
      proxy_ior = admin_->obtain(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    typedef typename Proxy::Skeleton Skeleton;
    const PortableServer::Servant servant = poa_->id_to_servant(
      reinterpret_cast<const PortableServer::ObjectId&> (proxy_state.object_id)
      ACE_ENV_ARG_PARAMETER);

    ACE_CHECK;
    Skeleton skeleton = dynamic_cast<Skeleton> (servant);
    ACE_CHECK;

    static_cast<Proxy*> (skeleton)->set_state(proxy_state
                                              ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
