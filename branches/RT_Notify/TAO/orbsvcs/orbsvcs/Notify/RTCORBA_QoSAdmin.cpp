// $Id$

#include "RTCORBA_QoSAdmin.h"
#include "RTCORBA_POA_Helper.h"
#include "Properties.h"
#include "Container.h"

#if ! defined (__ACE_INLINE__)
#include "RTCORBA_QoSAdmin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RTCORBA_QoSAdmin, "$Id$")

TAO_NS_RTCORBA_QoSAdmin::TAO_NS_RTCORBA_QoSAdmin (void)
{
}

TAO_NS_RTCORBA_QoSAdmin::~TAO_NS_RTCORBA_QoSAdmin ()
{
}

void
TAO_NS_RTCORBA_QoSAdmin::apply_threadpool_qos (TAO_NS_Container* container, NotifyExt::ThreadPoolParams* tp_params ACE_ENV_ARG_DECL)
{
  TAO_NS_RTCORBA_POA_Helper* proxy_poa = 0;

  // Bootstrap EC Proxy POA
  ACE_NEW_THROW_EX (proxy_poa,
                    TAO_NS_RTCORBA_POA_Helper (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (ec_ret._retn ());

  auto_ptr<TAO_NS_POA_Helper> auto_proxy_poa (proxy_poa);

  PortableServer::POA_var default_poa = TAO_NS_PROPERTIES::instance ()->default_poa ();

  proxy_poa->init (default_poa.in (), tp_params ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Give ownership of proxy_poa
  container->proxy_poa (proxy_poa);

  // release auto ref.
  auto_proxy_poa.release ();
}

void
TAO_NS_RTCORBA_QoSAdmin::apply_threadpool_lane_qos (TAO_NS_Container* container, NotifyExt::ThreadPoolLanesParams* tpl_params ACE_ENV_ARG_DECL)
{
  TAO_NS_RTCORBA_POA_Helper* proxy_poa = 0;

  // Bootstrap EC Proxy POA
  ACE_NEW_THROW_EX (proxy_poa,
                    TAO_NS_RTCORBA_POA_Helper (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (ec_ret._retn ());

  auto_ptr<TAO_NS_POA_Helper> auto_proxy_poa (proxy_poa);

  PortableServer::POA_var default_poa = TAO_NS_PROPERTIES::instance ()->default_poa ();

  proxy_poa->init (default_poa.in (), tpl_params ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Give ownership of proxy_poa
  container->proxy_poa (proxy_poa);

  // release auto ref.
  auto_proxy_poa.release ();
}
