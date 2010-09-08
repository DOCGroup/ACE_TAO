// $Id$

#include "ERE_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "DAnCE/Logger/Log_Macros.h"
namespace DAnCE
{
  namespace Config_Handlers
  {
    void
    ERE_Handler::external_ref_endpoints (
                                         const PlanConnectionDescription &src,
                                         Deployment::ExternalReferenceEndpoints &dest)
    {
      DANCE_TRACE("ERE_Handler::external_ref_endpoints");
      PlanConnectionDescription::externalReference_const_iterator erep_e =
        src.end_externalReference ();

      CORBA::ULong pos = 0;
      dest.length (src.count_externalReference ());
      for (PlanConnectionDescription::externalReference_const_iterator erep_b =
             src.begin_externalReference ();
           erep_b != erep_e;
           ++erep_b)
        {
          ERE_Handler::handle_external_ref_endpoint (*(*erep_b),
                                                     dest[pos++]);
        }
    }

    void
    ERE_Handler::handle_external_ref_endpoint (
                                        const ExternalReferenceEndpoint &src,
                                        Deployment::ExternalReferenceEndpoint &dest)
    {
      DANCE_TRACE("ERE_Handler::external_ref_endpoint");
      dest.location = ACE_TEXT_ALWAYS_CHAR ( src.location ().c_str ());
      dest.provider = src.provider ();
      if (src.portName_p ())
        dest.portName = ACE_TEXT_ALWAYS_CHAR ( src.portName ().c_str ());
      dest.supportedType.length (src.count_supportedType ());
      std::for_each (src.begin_supportedType (),
                     src.end_supportedType (),
                     String_Seq_Functor (dest.supportedType));
    }

    ExternalReferenceEndpoint
    ERE_Handler::external_ref_endpoint (
                                        const Deployment::ExternalReferenceEndpoint& src)
    {
      DANCE_TRACE("ERE_Handler::external_ref_endpoint - reverse");
      XMLSchema::string< ACE_TCHAR > loc (ACE_TEXT_CHAR_TO_TCHAR (src.location));
      //      XMLSchema::string< char > provider ((src.provider));
      XMLSchema::string< ACE_TCHAR > pn (ACE_TEXT_CHAR_TO_TCHAR (src.portName));
      //      XMLSchema::string< char > st ((src.supportedType));
      ExternalReferenceEndpoint erp (loc, src.provider);
      return erp;
    }
  }
}
