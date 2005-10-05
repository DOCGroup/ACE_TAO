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
      
      switch (desc.kind ().integral ())
        {
        case CCMComponentPortKind::Facet_l:
          toconfig.kind = Deployment::Facet;
          break;
          
        case CCMComponentPortKind::SimplexReceptacle_l:
          toconfig.kind = Deployment::SimplexReceptacle;
          break;
          
        case CCMComponentPortKind::MultiplexReceptacle_l:
          toconfig.kind = Deployment::MultiplexReceptacle;
          break;
          
        case CCMComponentPortKind::EventEmitter_l:
          toconfig.kind = Deployment::EventEmitter;
          break;
          
        case CCMComponentPortKind::EventPublisher_l:
          toconfig.kind = Deployment::EventPublisher;
          break;
          
        case CCMComponentPortKind::EventConsumer_l:
          toconfig.kind = Deployment::EventConsumer; 
          break;
          
        default:
          ACE_ERROR ((LM_ERROR, "Invalid port type in connection %s\n",
                      desc.name ().c_str ()));
          throw 1;
        }
      
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
      ::XMLSchema::string< char > fval ("false"); 
      ::XMLSchema::string< char > provider ("");
      ::XMLSchema::string< char > exclusiveProvider ("");
      ::XMLSchema::string< char > exclusiveUser ("");
      ::XMLSchema::string< char > optional ("");
      
      if (src.provider)
        provider = tval;
      else
        provider = fval;
      
      if (src.exclusiveUser)
        exclusiveUser = tval;
      else
        provider = fval;
      
      if (src.exclusiveProvider)
        exclusiveProvider = tval;
      else
        provider = fval;
      
      if (src.optional)
        optional = tval;
      else
        provider = fval;
      
      ComponentPortDescription cpd (
        name,
        stype,
        tval,
        provider,
        exclusiveProvider,
        exclusiveUser,
        optional,
        CCMComponentPortKind::Facet);

      switch (src.kind)
        {
        case ::Deployment::Facet:
          cpd.kind (CCMComponentPortKind::Facet);
          break;
          
        case ::Deployment::SimplexReceptacle:
          cpd.kind (CCMComponentPortKind::SimplexReceptacle);
          break;
          
        case ::Deployment::MultiplexReceptacle:
          cpd.kind (CCMComponentPortKind::MultiplexReceptacle);
          break;
          
        case ::Deployment::EventEmitter:
          cpd.kind (CCMComponentPortKind::EventEmitter);
          break;
          
        case ::Deployment::EventPublisher:
          cpd.kind (CCMComponentPortKind::EventPublisher);
          break;
          
        case ::Deployment::EventConsumer:
          cpd.kind (CCMComponentPortKind::EventConsumer);
          break;
          
        default:
          ACE_ERROR ((LM_ERROR, "Invalid port kind in connection %s\n",
                      name.c_str ()));
        }
      

      if (src.supportedType.length () > 0)
        cpd.supportedType (
          XMLSchema::string< char > ((src.supportedType[0])));      

      return cpd;
    }
  
  }
}
