#include "FT_Service_Activate.h"
#include "FT_Service_Callbacks.h"

#include "ace/Dynamic_Service.h"

ACE_RCSID(FaultTolerance, FT_Service_Activate, "$Id$")

TAO_FT_Service_Activate::TAO_FT_Service_Activate (void)
  :orb_core_ (0),
   ft_service_callback_ (0)
{

}

TAO_FT_Service_Activate::~TAO_FT_Service_Activate (void)
{
  delete this->ft_service_callback_;
}

TAO_Service_Callbacks *
TAO_FT_Service_Activate::activate_services (TAO_ORB_Core *orb_core)
  ACE_THROW_SPEC (())
{
  // @@ We have a problem here. Multiple ORBs can use this and we will
  // @@ be stuck badly. Let me get the basic stuff to work and then I
  // @@ will iterate over this again..

  this->orb_core_ = orb_core;

  cout << "Did we gete here "<<endl;
  if (this->ft_service_callback_ == 0)
    {
      // Construct service callback object
      ACE_NEW_RETURN (this->ft_service_callback_,
                      TAO_FT_Service_Callbacks (),
                      0);
    }

  return this->ft_service_callback_;
  // @@ All the other things that we may need, will be added from time
  // @@ to time..

}



ACE_FACTORY_DEFINE (TAO_FT, TAO_FT_Service_Activate);

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_FT_Service_Activate>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_FT_Service_Activate>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
