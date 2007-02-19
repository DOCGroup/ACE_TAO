// $Id$

#include "CPD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    void
    CPD_Handler::handle_component_port_description (
                                             const ComponentPortDescription& desc,
                                             Deployment::ComponentPortDescription& toconfig)
    {
      CIAO_TRACE("CPD_Handler::component_port_description");
      toconfig.name =
        desc.name ().c_str ();

      if (desc.specificType_p ())
        toconfig.specificType =
          desc.specificType ().c_str ();

      toconfig.supportedType.length (desc.count_supportedType ());
      std::for_each (desc.begin_supportedType (),
                     desc.end_supportedType (),
                     String_Seq_Functor (toconfig.supportedType));


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
      toconfig.provider = desc.provider ();
      toconfig.exclusiveProvider = desc.exclusiveProvider ();
      toconfig.exclusiveUser = desc.exclusiveUser ();
      toconfig.optional =  desc.optional ();
    }

    ComponentPortDescription
    CPD_Handler::component_port_description (
                                             const Deployment::ComponentPortDescription& src)
    {
      CIAO_TRACE("CPD_Handler::component_port_description - reverse");
      ::XMLSchema::string< char > name ((src.name));
      ::XMLSchema::string< char > stype ((src.specificType));

      ::XMLSchema::string< char > tval ("true");
      ::XMLSchema::string< char > fval ("false");
      XMLSchema::boolean provider;
      XMLSchema::boolean  exclusiveProvider;
      XMLSchema::boolean exclusiveUser;
      XMLSchema::boolean optional;

      if (src.provider)
        provider = true;
      else
        provider = false;

      if (src.exclusiveUser)
        exclusiveUser = true;
      else
        provider = false;

      if (src.exclusiveProvider)
        exclusiveProvider = true;
      else
        provider = false;

      if (src.optional)
        optional = true;
      else
        provider = false;

      ComponentPortDescription cpd (name,
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

      for (CORBA::ULong i = 0; i < src.supportedType.length (); ++i)
        cpd.add_supportedType (XMLSchema::string< char > ((src.supportedType[i])));

      return cpd;
    }

  }
}
