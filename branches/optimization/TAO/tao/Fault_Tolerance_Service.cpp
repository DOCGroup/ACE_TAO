#include "tao/Fault_Tolerance_Service.h"
#include "tao/Service_Callbacks.h"
#include "ace/Dynamic_Service.h"
#include "tao/Services_Activate.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"

#if !defined (__ACE_INLINE__)
# include "tao/Fault_Tolerance_Service.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Fault_Tolerance_Service, "$Id$")

TAO_Fault_Tolerance_Service::~TAO_Fault_Tolerance_Service (void)
{
  delete this->ft_service_callback_;
  delete this->ft_object_retention_id_lock_;
}

void
TAO_Fault_Tolerance_Service::init (TAO_ORB_Core *orb_core)
{
  // Look in to the svc conf stuff to get an instance of the
  // FT_Service.
  if (this->ft_service_callback_ == 0)
    {
      TAO_Services_Activate *service =
        ACE_Dynamic_Service <TAO_Services_Activate>::instance ("FT_Service_Activate");

      // Activate the callback
      if (service)
        this->ft_service_callback_ = service->activate_services (orb_core);
    }

  // If we have a valid service then we instantiate our lock and the
  // client id
  if (this->ft_service_callback_)
    {
      // Initialize the lock
      this->ft_object_retention_id_lock_ =
        orb_core->client_factory ()
          ->create_ft_service_retention_id_lock ();

      // Initialize the client id
      this->ft_object_id_ = "TAO_Client";
    }

  return;
}
