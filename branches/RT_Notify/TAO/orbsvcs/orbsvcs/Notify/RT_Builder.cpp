// $Id$

#include "RT_Builder.h"

#include "ace/Auto_Ptr.h"
#include "ace/Dynamic_Service.h"
#include "ETCL_FilterFactory.h"
#include "RT_POA_Helper.h"
#include "Properties.h"
#include "orbsvcs/NotifyExtC.h"

#if ! defined (__ACE_INLINE__)
#include "RT_Builder.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RT_Builder, "$Id$")

TAO_NS_RT_Builder::TAO_NS_RT_Builder (void)
{
}

TAO_NS_RT_Builder::~TAO_NS_RT_Builder ()
{
}

CosNotifyFilter::FilterFactory_ptr
TAO_NS_RT_Builder::build_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  return TAO_NS_Builder::build_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);

  TAO_NS_FilterFactory* ff = ACE_Dynamic_Service<TAO_NS_FilterFactory>::instance ("TAO_NS_FilterFactory");

  if (ff == 0)
    {
      ACE_NEW_THROW_EX (ff,
                        TAO_NS_ETCL_FilterFactory (),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CosNotifyFilter::FilterFactory::_nil ());
    }

  PortableServer::POA_var default_poa = TAO_NS_PROPERTIES::instance ()->default_poa ();

  TAO_NS_RT_POA_Helper filter_poa;

  NotifyExt::ThreadPoolParams tp_params = {0, 1, 0, 0, 0, 0, 0 };

  //filter_poa.init (default_poa.in (), tp_params ACE_ENV_ARG_PARAMETER);
  filter_poa.init (default_poa.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POA_var filter_poa_var = filter_poa.poa ();

  return ff->create (filter_poa_var ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_Builder::apply_thread_pool_concurrency (TAO_NS_Object& object
                                                       , const NotifyExt::ThreadPoolParams& tp_params ACE_ENV_ARG_DECL)
{
  TAO_NS_RT_POA_Helper* proxy_poa = 0;

  // Bootstrap EC Proxy POA
  ACE_NEW_THROW_EX (proxy_poa,
                    TAO_NS_RT_POA_Helper (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (ec_ret._retn ());

  auto_ptr<TAO_NS_POA_Helper> auto_proxy_poa (proxy_poa);

  PortableServer::POA_var default_poa = TAO_NS_PROPERTIES::instance ()->default_poa ();

  proxy_poa->init (default_poa.in (), tp_params ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Give ownership of proxy_poa
  object.proxy_poa_own (proxy_poa);

  // release auto ref.
  auto_proxy_poa.release ();
}

void
TAO_NS_RT_Builder::apply_lane_concurrency (TAO_NS_Object& object
                                                , const NotifyExt::ThreadPoolLanesParams& tpl_params ACE_ENV_ARG_DECL)
{
  TAO_NS_RT_POA_Helper* proxy_poa = 0;

  // Bootstrap EC Proxy POA
  ACE_NEW_THROW_EX (proxy_poa,
                    TAO_NS_RT_POA_Helper (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (ec_ret._retn ());

  auto_ptr<TAO_NS_POA_Helper> auto_proxy_poa (proxy_poa);

  PortableServer::POA_var default_poa = TAO_NS_PROPERTIES::instance ()->default_poa ();

  proxy_poa->init (default_poa.in (), tpl_params ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Give ownership of proxy_poa
  object.proxy_poa_own (proxy_poa);

  // release auto ref.
  auto_proxy_poa.release ();
}
