// $Id$

#include "FT_Endpoint_Selector_Factory.h"
#include "FT_Invocation_Endpoint_Selectors.h"
#include "tao/Invocation.h"

ACE_RCSID(FaultTolerance, FT_Endpoint_Selector_Factory, "$Id$")

TAO_FT_Endpoint_Selector_Factory::TAO_FT_Endpoint_Selector_Factory (void)
  :ft_endpoint_selector_ (0)
{
}

TAO_FT_Endpoint_Selector_Factory::~TAO_FT_Endpoint_Selector_Factory (void)
{
  if (this->ft_endpoint_selector_)
    delete this->ft_endpoint_selector_;
}


TAO_Invocation_Endpoint_Selector *
TAO_FT_Endpoint_Selector_Factory::get_selector (TAO_GIOP_Invocation *
                                                ACE_ENV_ARG_DECL)
{
  if (this->ft_endpoint_selector_ == 0)
    {
      ACE_NEW_THROW_EX (ft_endpoint_selector_,
                        TAO_FT_Invocation_Endpoint_Selector (),
                        CORBA::NO_MEMORY ());
    }

  return this->ft_endpoint_selector_;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_FT_Endpoint_Selector_Factory,
                       ACE_TEXT ("FT_Endpoint_Selector_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_FT_Endpoint_Selector_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_FT, TAO_FT_Endpoint_Selector_Factory)
