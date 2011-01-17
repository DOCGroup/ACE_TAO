// $Id$

#include "tao/RTCORBA/RT_Endpoint_Selector_Factory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_Invocation_Endpoint_Selectors.h"
#include "tao/RTCORBA/RT_Endpoint_Utils.h"
#include "tao/RTCORBA/RT_Stub.h"

#include "tao/Protocols_Hooks.h"

#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

RT_Endpoint_Selector_Factory::RT_Endpoint_Selector_Factory (void)
{
  ACE_NEW (this->rt_invocation_endpoint_selector_,
           TAO_RT_Invocation_Endpoint_Selector);
}

RT_Endpoint_Selector_Factory::~RT_Endpoint_Selector_Factory (void)
{
  delete this->rt_invocation_endpoint_selector_;
}

TAO_Invocation_Endpoint_Selector *
RT_Endpoint_Selector_Factory::get_selector (void)
{
  return this->rt_invocation_endpoint_selector_;
}


// ****************************************************************

ACE_STATIC_SVC_DEFINE (RT_Endpoint_Selector_Factory,
                       ACE_TEXT ("RT_Endpoint_Selector_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (RT_Endpoint_Selector_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTCORBA, RT_Endpoint_Selector_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
