// $Id$


#include "tao/Default_Endpoint_Selector_Factory.h"

#if !defined (TAO_HAS_COLLOCATION)
#include "tao/Invocation_Endpoint_Selectors.h"
#endif

#include "ace/Log_Msg.h"


ACE_RCSID (tao,
           Default_Endpoint_Selector_Factory,
           "$Id$")


TAO_Default_Endpoint_Selector_Factory::TAO_Default_Endpoint_Selector_Factory (void)
{

#if !defined (TAO_HAS_COLLOCATION)
  ACE_NEW (this->default_endpoint_selector_,
           TAO_Default_Endpoint_Selector);
#endif

}

TAO_Default_Endpoint_Selector_Factory::~TAO_Default_Endpoint_Selector_Factory (void)
{

#if !defined (TAO_HAS_COLLOCATION)
  delete this->default_endpoint_selector_;
#endif

}


TAO_Invocation_Endpoint_Selector *
TAO_Default_Endpoint_Selector_Factory::get_selector (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{

#if !defined (TAO_HAS_COLLOCATION)
  // Trivial endpoint selector.  Just return the default selector.
  return this->default_endpoint_selector_;
#else
  return 0;
#endif
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Endpoint_Selector_Factory,
                       ACE_TEXT ("Default_Endpoint_Selector_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Endpoint_Selector_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Endpoint_Selector_Factory)
