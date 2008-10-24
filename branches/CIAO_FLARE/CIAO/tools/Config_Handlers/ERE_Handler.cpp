// $Id$

#include "ERE_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "ciao/CIAO_common.h"
namespace CIAO
{
  namespace Config_Handlers
  {
    void
    ERE_Handler::external_ref_endpoints (
                                         const PlanConnectionDescription &src,
                                         Deployment::ExternalReferenceEndpoints &dest)
    {
      CIAO_TRACE("ERE_Handler::external_ref_endpoints");
      PlanConnectionDescription::externalReference_const_iterator erep_e =
        src.end_externalReference ();

      CORBA::ULong pos = 0;
      dest.length (src.count_externalReference ());
      for (PlanConnectionDescription::externalReference_const_iterator erep_b =
             src.begin_externalReference ();
           erep_b != erep_e;
           ++erep_b)
        {
          ERE_Handler::handle_external_ref_endpoint ((*erep_b),
                                              dest[pos++]);
        }
    }

    void
    ERE_Handler::handle_external_ref_endpoint (
                                        const ExternalReferenceEndpoint &src,
                                        Deployment::ExternalReferenceEndpoint &dest)
    {
      CIAO_TRACE("ERE_Handler::external_ref_endpoint");
      dest.location =
        src.location ().c_str ();
      
      dest.provider = 
        src.provider ();
      
      dest.portName = 
        src.portName ().c_str ();
      
      dest.supportedType.length (src.count_supportedType ());
      std::for_each (src.begin_supportedType (),
                     src.end_supportedType (),
                     String_Seq_Functor (dest.supportedType));

    }

    ExternalReferenceEndpoint
    ERE_Handler::external_ref_endpoint (
                                        const Deployment::ExternalReferenceEndpoint& src)
    {
      CIAO_TRACE("ERE_Handler::external_ref_endpoint - reverse");
      XMLSchema::string< char > loc ((src.location));
      //      XMLSchema::string< char > provider ((src.provider));
      XMLSchema::string< char > pn ((src.portName));
      //      XMLSchema::string< char > st ((src.supportedType));
      ExternalReferenceEndpoint erp (loc, src.provider);
      return erp;
    }
  }
}
