// $Id$

#include "tao/Fault_Tolerance_Service.h"
#include "tao/Services_Activate.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "tao/Fault_Tolerance_Service.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Fault_Tolerance_Service::~TAO_Fault_Tolerance_Service (void)
{
  delete this->ft_service_callback_;
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

  return;
}

TAO_END_VERSIONED_NAMESPACE_DECL
