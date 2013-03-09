// $Id$

#include "orbsvcs/FaultTolerance/FT_Endpoint_Selector_Factory.h"
#include "orbsvcs/FaultTolerance/FT_Invocation_Endpoint_Selectors.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FT_Endpoint_Selector_Factory::TAO_FT_Endpoint_Selector_Factory (void)
  : ft_endpoint_selector_ (0)
{
}

TAO_FT_Endpoint_Selector_Factory::~TAO_FT_Endpoint_Selector_Factory (void)
{
  delete this->ft_endpoint_selector_;
}


TAO_Invocation_Endpoint_Selector *
TAO_FT_Endpoint_Selector_Factory::get_selector ()
{
    if (this->ft_endpoint_selector_ == 0)
    {
      ACE_MT (ACE_GUARD_RETURN  (TAO_SYNCH_MUTEX,
                                 guard,
                                 this->mutex_,
                                 0));

      // Double checked locking..
      if (this->ft_endpoint_selector_ == 0)
        {
          ACE_NEW_THROW_EX (ft_endpoint_selector_,
                            TAO_FT_Invocation_Endpoint_Selector (),
                            CORBA::NO_MEMORY ());
        }
    }

  return this->ft_endpoint_selector_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_FT_Endpoint_Selector_Factory,
                       ACE_TEXT ("FT_Endpoint_Selector_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_FT_Endpoint_Selector_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_FT_ClientORB, TAO_FT_Endpoint_Selector_Factory)
