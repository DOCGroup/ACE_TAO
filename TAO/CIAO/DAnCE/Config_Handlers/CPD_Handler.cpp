
// $Id$


#include "CPD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"



namespace CIAO
{
  namespace Config_Handlers
  {

    CPD_Handler::CPD_Handler (void)
    {
    }

    CPD_Handler::~CPD_Handler (void)
    {
    }


    void
    CPD_Handler::comp_port_descr (
                    const ComponentPortDescription& desc,
                    Deployment::ComponentPortDescription& toconfig)
    {


      
      toconfig.name=
           CORBA::string_dup (desc.name ().c_str ());
      
      toconfig.specificType=
           CORBA::string_dup (desc.specificType ().c_str ());
      
      CORBA::ULong supportedType_length = toconfig.supportedType.length ();
         toconfig.supportedType.length (supportedType_length + 1);
         toconfig.supportedType[supportedType_length - 1]=
           CORBA::string_dup (desc.supportedType ().c_str ());
      
      toconfig.provider=
           !desc.provider ().empty ();
      
      toconfig.exclusiveProvider=
           !desc.exclusiveProvider ().empty ();
      
      toconfig.exclusiveUser=
           !desc.exclusiveUser ().empty ();
      
      if (desc.kind () ==
         CCMComponentPortKind::Facet)        
         toconfig.kind = Deployment::Facet;
      if (desc.kind () ==
         CCMComponentPortKind::SimplexReceptacle)        
         toconfig.kind = Deployment::SimplexReceptacle;
      if (desc.kind () ==
         CCMComponentPortKind::MultiplexReceptacle)        
         toconfig.kind = Deployment::MultiplexReceptacle;
      if (desc.kind () ==
         CCMComponentPortKind::EventEmitter)        
         toconfig.kind = Deployment::EventEmitter;
      if (desc.kind () ==
         CCMComponentPortKind::EventPublisher)        
         toconfig.kind = Deployment::EventPublisher;
      if (desc.kind () ==
         CCMComponentPortKind::EventConsumer)        
         toconfig.kind = Deployment::EventConsumer;

      
    }

  }

}
