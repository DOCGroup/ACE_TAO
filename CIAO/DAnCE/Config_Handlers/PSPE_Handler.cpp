// $Id$

#include "PSPE_Handler.h"
#include "IDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    void
    PSPE_Handler::sub_component_port_endpoints (
                                                const PlanConnectionDescription &src,
                                                ::Deployment::PlanSubcomponentPortEndpoints &dest)
    {
      DANCE_TRACE("PSPE_Handler::sub_component_port_endpoints");

      PlanConnectionDescription::internalEndpoint_const_iterator iei_e =
        src.end_internalEndpoint ();
      CORBA::ULong pos = 0;
      dest.length (src.count_internalEndpoint ());

      for (PlanConnectionDescription::internalEndpoint_const_iterator iei_b =
             src.begin_internalEndpoint ();
           iei_b != iei_e;
           ++iei_b)
        {
          PSPE_Handler::sub_component_port_endpoint (*(*iei_b),
                                                     dest[pos++]);
        }
    }

    void
    PSPE_Handler::sub_component_port_endpoint (
                                               const PlanSubcomponentPortEndpoint &src,
                                               ::Deployment::PlanSubcomponentPortEndpoint &dest)
    {
      DANCE_TRACE("PSPE_Handler::sub_component_port_endpoint");
      dest.portName =
        ACE_TEXT_ALWAYS_CHAR (src.portName ().c_str ());

      if (src.provider_p ())
        {
          dest.provider = src.provider () == ACE_TEXT ("true");
        }
      else
        {
          dest.provider = false;
        }

      CORBA::ULong tmp = 0;

      IDD_Handler::IDREF.find_ref (ACE_TString (src.instance ().idref ().id ().c_str ()),
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
          DANCE_DEBUG (1, (LM_DEBUG, "Invalid port kind in connection\n"));

        }
    }

    PlanSubcomponentPortEndpoint
    PSPE_Handler::sub_component_port_endpoint (
                                               const Deployment::PlanSubcomponentPortEndpoint &src)
    { // @@MAJO
      DANCE_TRACE("PSPE_Handler::sub_component_port_endpoint - reverse");
      XMLSchema::string< ACE_TCHAR > pname (ACE_TEXT_CHAR_TO_TCHAR (src.portName));
      XMLSchema::string< ACE_TCHAR > tval  (ACE_TEXT ("true"));
      XMLSchema::string< ACE_TCHAR > prov  (ACE_TEXT (""));
      ACE_TString id;
      IDD_Handler::IDREF.find_ref(src.instanceRef, id);
      XMLSchema::IDREF < ACE_TCHAR > idref(id.c_str());

      if (src.provider)
        prov = tval;

      IdRef idr;
      idr.idref (idref);

      PlanSubcomponentPortEndpoint pspe (pname,
                                         CCMComponentPortKind::Facet,
                                         idr);
      pspe.provider (prov);

      switch (src.kind)
        {
        case ::Deployment::Facet:
          pspe.kind (CCMComponentPortKind::Facet);
          break;

        case ::Deployment::SimplexReceptacle:
          pspe.kind (CCMComponentPortKind::SimplexReceptacle);
          break;

        case ::Deployment::MultiplexReceptacle:
          pspe.kind (CCMComponentPortKind::MultiplexReceptacle);
          break;

        case ::Deployment::EventEmitter:
          pspe.kind (CCMComponentPortKind::EventEmitter);
          break;

        case ::Deployment::EventPublisher:
          pspe.kind (CCMComponentPortKind::EventPublisher);
          break;

        case ::Deployment::EventConsumer:
          pspe.kind (CCMComponentPortKind::EventConsumer);
          break;

        default:
          DANCE_ERROR (1, (LM_ERROR, "Invalid port kind in PSPE\n"));
        }


      return pspe;
    }
  }
}
