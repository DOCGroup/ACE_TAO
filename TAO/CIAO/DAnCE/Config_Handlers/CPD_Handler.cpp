// $Id$
#include "CPD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
namespace CIAO
{
  namespace Config_Handlers
  {
    void
    CPD_Handler::component_port_description (
      const ComponentPortDescription& desc,
      Deployment::ComponentPortDescription& toconfig)
    {
      toconfig.name =
        desc.name ().c_str ();

      toconfig.specificType =
        desc.specificType ().c_str ();

      if (!desc.supportedType ().empty ())
        {
          toconfig.supportedType.length (1);
          toconfig.supportedType[0] =
            desc.supportedType ().c_str ();
        }
        
      if (desc.kind () == CCMComponentPortKind::Facet)
        toconfig.kind = Deployment::Facet;
      if (desc.kind () == CCMComponentPortKind::SimplexReceptacle)
        toconfig.kind = Deployment::SimplexReceptacle;
      if (desc.kind () == CCMComponentPortKind::MultiplexReceptacle)
        toconfig.kind = Deployment::MultiplexReceptacle;
      if (desc.kind () == CCMComponentPortKind::EventEmitter)
        toconfig.kind = Deployment::EventEmitter;
      if (desc.kind () == CCMComponentPortKind::EventPublisher)
        toconfig.kind = Deployment::EventPublisher;
      if (desc.kind () == CCMComponentPortKind::EventConsumer)
        toconfig.kind = Deployment::EventConsumer;      
      
      /* @@BUG: We need to consider how to handle booleans. */
      toconfig.provider = desc.provider () == "true";
      toconfig.exclusiveProvider = desc.exclusiveProvider () == "true";
      toconfig.exclusiveUser = desc.exclusiveUser () == "true";
      toconfig.optional =  desc.optional () == "true";
    }
    
    ComponentPortDescription
    CPD_Handler::component_port_description (
      const Deployment::ComponentPortDescription& src)
    {
      ::XMLSchema::string< char > name ((src.name));
      ::XMLSchema::string< char > stype ((src.specificType));
     
      ::XMLSchema::string< char > tval ("true"); 
      ::XMLSchema::string< char > provider ("");
      ::XMLSchema::string< char > exclusiveProvider ("");
      ::XMLSchema::string< char > exclusiveUser ("");
      ::XMLSchema::string< char > optional ("");

      ComponentPortDescription cpd (
        name,
        stype,
        tval,
        provider,
        exclusiveProvider,
        exclusiveUser,
        optional,
        CCMComponentPortKind::Facet);

      if (src.provider)
        provider = tval;
      if (src.exclusiveUser)
        exclusiveUser = tval;
      if (src.exclusiveProvider)
        exclusiveProvider = tval;
      if (src.optional)
        optional = tval;
 
      if (src.kind == ::Deployment::Facet)
        cpd.kind (CCMComponentPortKind::Facet);
      if (src.kind == ::Deployment::SimplexReceptacle)
        cpd.kind (CCMComponentPortKind::SimplexReceptacle);
      if (src.kind == ::Deployment::MultiplexReceptacle)
        cpd.kind (CCMComponentPortKind::MultiplexReceptacle);
      if (src.kind == ::Deployment::EventEmitter)
        cpd.kind (CCMComponentPortKind::EventEmitter);
      if (src.kind == ::Deployment::EventPublisher)
        cpd.kind (CCMComponentPortKind::EventPublisher);
      if (src.kind == ::Deployment::EventConsumer)
        cpd.kind (CCMComponentPortKind::EventConsumer);

      if (src.supportedType.length () > 0)
        cpd.supportedType (
          XMLSchema::string< char > ((src.supportedType[0])));      

      return cpd;
    }
  
  }
}
