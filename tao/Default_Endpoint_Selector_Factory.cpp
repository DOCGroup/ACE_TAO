// $Id$

#include "tao/Default_Endpoint_Selector_Factory.h"
#include "tao/Invocation_Endpoint_Selectors.h"

#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Endpoint_Selector_Factory::TAO_Default_Endpoint_Selector_Factory (
  void)
  : default_endpoint_selector_ (new TAO_Default_Endpoint_Selector)
{
}

TAO_Default_Endpoint_Selector_Factory::~TAO_Default_Endpoint_Selector_Factory (void)
{
  delete this->default_endpoint_selector_;
}


TAO_Invocation_Endpoint_Selector *
TAO_Default_Endpoint_Selector_Factory::get_selector (void)
{
  // Trivial endpoint selector.  Just return the default selector.
  return this->default_endpoint_selector_;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Endpoint_Selector_Factory,
                       ACE_TEXT ("Default_Endpoint_Selector_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Endpoint_Selector_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Endpoint_Selector_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

