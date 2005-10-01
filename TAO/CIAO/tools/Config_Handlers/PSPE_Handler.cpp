// $Id$

#include "PSPE_Handler.h"
#include "IDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    PSPE_Handler::sub_component_port_endpoints (
        const PlanConnectionDescription &src,
        ::Deployment::PlanSubcomponentPortEndpoints &dest)
    {
      PlanConnectionDescription::internalEndpoint_const_iterator iei_e =
        src.end_internalEndpoint ();
      for (PlanConnectionDescription::internalEndpoint_const_iterator iei_b =
             src.begin_internalEndpoint ();
           iei_b != iei_e;
           ++iei_b)
        {
          CORBA::ULong len =
            dest.length ();

          dest.length (len + 1);

          (void) PSPE_Handler::sub_component_port_endpoint (
              (*iei_b),
              dest[len]);
        }
      return true;
    }

    void
    PSPE_Handler::sub_component_port_endpoint (
        const PlanSubcomponentPortEndpoint &src,
        ::Deployment::PlanSubcomponentPortEndpoint &dest)
    {
      dest.portName =
        src.portName ().c_str ();

      if (src.provider_p ())
        {
          dest.provider = !(src.provider ().empty ());
        }
      else
        {
          dest.provider = 0;
        }

      CORBA::ULong tmp = 0;
      IDD_Handler::IDREF.find_ref (ACE_CString (src.instance ().id ().c_str ()), 
					    tmp);
            
      dest.instanceRef = tmp;

      switch (src.kind ().integral ())
        {
          case CCMComponentPortKind::Facet_l:
            dest.kind = Deployment::Facet;
            break;
            
          case CCMComponentPortKind::SimplexReceptacle_l:
            dest.kind = Deployment::SimplexReceptacle;
            break;
            
          case CCMComponentPortKind::MultiplexReceptacle_l:
            dest.kind = Deployment::MultiplexReceptacle;
            break;
            
          case CCMComponentPortKind::EventEmitter_l:
            dest.kind = Deployment::EventEmitter;
            break;
            
          case CCMComponentPortKind::EventPublisher_l:
            dest.kind = Deployment::EventPublisher;
            break;
            
          case CCMComponentPortKind::EventConsumer_l:
            dest.kind = Deployment::EventConsumer;
            break;
          
          default:
            ACE_DEBUG ((LM_DEBUG, "Invalid port kind in connection\n"));
            
         }
    }

    PlanSubcomponentPortEndpoint
    PSPE_Handler::sub_component_port_endpoint (
      const Deployment::PlanSubcomponentPortEndpoint &src)
    { // @@MAJO
      XMLSchema::string< char > pname ((src.portName));
      XMLSchema::string< char > tval  ("true");
      XMLSchema::string< char > prov  ("");
      ACE_CString id;
      IDD_Handler::IDREF.find_ref(src.instanceRef, id);
      XMLSchema::IDREF < ACE_TCHAR > idref(id.c_str()); 
      
      if (src.provider)
        prov = tval;

      PlanSubcomponentPortEndpoint pspe (pname,
					 CCMComponentPortKind::Facet,
					 idref);
      pspe.provider (prov);
      
      if (src.kind == ::Deployment::Facet)
        pspe.kind (CCMComponentPortKind::Facet);
      if (src.kind == ::Deployment::SimplexReceptacle)
        pspe.kind (CCMComponentPortKind::SimplexReceptacle);
      if (src.kind == ::Deployment::MultiplexReceptacle)
        pspe.kind (CCMComponentPortKind::MultiplexReceptacle);
      if (src.kind == ::Deployment::EventEmitter)
        pspe.kind (CCMComponentPortKind::EventEmitter);
      if (src.kind == ::Deployment::EventPublisher)
        pspe.kind (CCMComponentPortKind::EventPublisher);
      if (src.kind == ::Deployment::EventConsumer)
        pspe.kind (CCMComponentPortKind::EventConsumer);

      return pspe;
    }
  }
}
