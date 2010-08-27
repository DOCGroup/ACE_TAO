// $Id$

#include "orbsvcs/Notify/RT_Builder.h"

#include "ace/Auto_Ptr.h"
#include "ace/Dynamic_Service.h"
#include "orbsvcs/Notify/ETCL_FilterFactory.h"
#include "orbsvcs/Notify/RT_POA_Helper.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/NotifyExtC.h"
#include "orbsvcs/Notify/Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_RT_Builder::TAO_Notify_RT_Builder (void)
{
}

TAO_Notify_RT_Builder::~TAO_Notify_RT_Builder ()
{
}

void
TAO_Notify_RT_Builder::apply_thread_pool_concurrency (TAO_Notify_Object& object
                                                       , const NotifyExt::ThreadPoolParams& tp_params)
{
  TAO_Notify_RT_POA_Helper* proxy_poa = 0;

  // Bootstrap EC Proxy POA
  ACE_NEW_THROW_EX (proxy_poa,
                    TAO_Notify_RT_POA_Helper (),
                    CORBA::NO_MEMORY ());

  ACE_Auto_Ptr<TAO_Notify_POA_Helper> auto_proxy_poa (proxy_poa);

  PortableServer::POA_var default_poa = TAO_Notify_PROPERTIES::instance ()->default_poa ();

  proxy_poa->init (default_poa.in (), tp_params);

  // Give ownership of proxy_poa
  object.set_proxy_poa (auto_proxy_poa.release ());
}

void
TAO_Notify_RT_Builder::apply_lane_concurrency (TAO_Notify_Object& object
                                                , const NotifyExt::ThreadPoolLanesParams& tpl_params)
{
  TAO_Notify_RT_POA_Helper* proxy_poa = 0;

  // Bootstrap EC Proxy POA
  ACE_NEW_THROW_EX (proxy_poa,
                    TAO_Notify_RT_POA_Helper (),
                    CORBA::NO_MEMORY ());

  ACE_Auto_Ptr<TAO_Notify_POA_Helper> auto_proxy_poa (proxy_poa);

  PortableServer::POA_var default_poa = TAO_Notify_PROPERTIES::instance ()->default_poa ();

  proxy_poa->init (default_poa.in (), tpl_params);

  // Give ownership of proxy_poa
  object.set_proxy_poa (auto_proxy_poa.release ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
