// $Id$

#include "ERE_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    ERE_Handler::external_ref_endpoints (
        const PlanConnectionDescription &src,
        Deployment::ExternalReferenceEndpoints &dest)
    {
      PlanConnectionDescription::externalReference_const_iterator erep_e =
        src.end_externalReference ();

     for (PlanConnectionDescription::externalReference_const_iterator erep_b =
            src.begin_externalReference ();
          erep_b != erep_e;
          ++erep_b)
       {
         CORBA::ULong len =
           dest.length ();
         dest.length (len + 1);

         ERE_Handler::external_ref_endpoint ((*erep_b),
                                             dest[0]);
       }

     return true;
    }

    void
    ERE_Handler::external_ref_endpoint (
        const ExternalReferenceEndpoint &src,
        Deployment::ExternalReferenceEndpoint &dest)
    {
      dest.location =
        src.location ().c_str ();
    }
   
    ExternalReferenceEndpoint 
    ERE_Handler::external_ref_endpoint (
        const Deployment::ExternalReferenceEndpoint& src)
    {
      XMLSchema::string< char > loc ((src.location));
      ExternalReferenceEndpoint erp (loc);
      return erp;
    }
  }
}
