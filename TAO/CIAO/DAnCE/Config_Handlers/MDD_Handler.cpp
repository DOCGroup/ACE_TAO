// $Id$

#include "MDD_Handler.h"
#include "Req_Handler.h"
#include "Prop_Handler.h"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    MDD_Handler::MDD_Handler (void)
    {
    }

    MDD_Handler::~MDD_Handler (void)
    {
    }


    void 
    MDD_Handler::get_MonolithicDeploymentDescription (
                    Deployment::MonolithicDeploymentDescription& toconfig,
                    MonolithicDeploymentDescription& desc)
    {
      //Transfer the name.
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());
      
      //Increase the length of the sequence.
      toconfig.source.length (toconfig.source.length () + 1);
      
      //The IDL specifies a sequence for source, but the
      //schema specifies a string, so we map that single string
      //to the first position in the sequence.
      toconfig.source[toconfig.source.length () - 1] = 
        CORBA::string_dup (desc.source ().c_str ());
        
      /*
       * 
       * 
       *    NEED TO FIGURE OUT WHAT THE VALUE
       *    THAT GETS STORED IN artifactRef will be
       * 
       *    IT WILL GO HERE
       */
       
       
       if (desc.execParameter_p ())
         {
           //Create the property handler to
           //delegate to.
           Prop_Handler phandler; 
           
           //Lengthen the sequence for the new element.
           toconfig.execParameter.length (
             toconfig.execParameter.length () + 1);
           
           //Configure the new property using the handler.
           phandler.get_Property (
             toconfig.execParameter[toconfig.execParameter.length () -1],
             desc.execParameter ());
         }
      
       if (desc.deployRequirement_p ())
         {
           //Create the Requirement handler.
           Requirement_Handler reqhandler; 
           
           //Lengthen the sequence for the new element.
           toconfig.deployRequirement.length (
             toconfig.deployRequirement.length () + 1);
             
           //Configure the new requirement using the handler.
           reqhandler.get_Requirement (
             toconfig.deployRequirement[toconfig.deployRequirement.length () -1],
             desc.deployRequirement ());
         }
      
      
    }
    
  }
}
