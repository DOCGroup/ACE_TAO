#include "tao/Fault_Tolerance_Service.h"
#include "tao/Services_Activate.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "tao/Fault_Tolerance_Service.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Fault_Tolerance_Service,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
        ACE_Dynamic_Service <TAO_Services_Activate>::instance ("FT_ClientService_Activate");

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

CORBA::Long
TAO_Fault_Tolerance_Service::retention_id (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->ft_object_retention_id_lock_,
                            0));

  return ++this->ft_object_retention_id_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
