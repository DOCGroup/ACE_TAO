// $Id$

#include "tao/Endpoint_Selector_Factory.h"
#include "tao/Invocation.h"

ACE_RCSID(tao, Endpoint_Selector_Factory, "$Id$")

TAO_Endpoint_Selector_Factory::TAO_Endpoint_Selector_Factory (void)
{
  ACE_NEW (this->default_endpoint_selector_,
           TAO_Default_Endpoint_Selector);

}

TAO_Endpoint_Selector_Factory::~TAO_Endpoint_Selector_Factory (void)
{
  delete this->default_endpoint_selector_;
}


TAO_Invocation_Endpoint_Selector *
TAO_Endpoint_Selector_Factory::get_selector (TAO_GIOP_Invocation *,
                                             CORBA::Environment &)
{
  return this->default_endpoint_selector_;
}
