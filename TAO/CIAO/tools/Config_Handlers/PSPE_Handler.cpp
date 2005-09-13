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
      bool r = IDD_Handler::IDREF.find_ref (ACE_CString (src.instance ().id ().c_str ()), 
					    tmp);
      ACE_UNUSED_ARG (r);
      
      // @@ MAJO:  What should we do if find_ref fails??
      dest.instanceRef = tmp;

      if (src.kind () == CCMComponentPortKind::Facet)
        dest.kind = Deployment::Facet;
      if (src.kind () == CCMComponentPortKind::SimplexReceptacle)
        dest.kind = Deployment::SimplexReceptacle;
      if (src.kind () == CCMComponentPortKind::MultiplexReceptacle)
        dest.kind = Deployment::MultiplexReceptacle;
      if (src.kind () == CCMComponentPortKind::EventEmitter)
        dest.kind = Deployment::EventEmitter;
      if (src.kind () == CCMComponentPortKind::EventPublisher)
        dest.kind = Deployment::EventPublisher;
      if (src.kind () == CCMComponentPortKind::EventConsumer)
        dest.kind = Deployment::EventConsumer;
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
