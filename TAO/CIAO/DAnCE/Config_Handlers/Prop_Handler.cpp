//$Id$

#include "Prop_Handler.h"
#include "ANY_Handler.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    CIAO::Config_Handlers::Prop_Handler::Prop_Handler (void)
    {
    }
    CIAO::Config_Handlers::Prop_Handler::~Prop_Handler (void)
    {
    }    
                
    ///This method maps the values from the
    ///XSC object <CIAO::Config_Handlers::Property> to
    ///the CORBA IDL type <Deployment::Property>.    
    void
    CIAO::Config_Handlers::Prop_Handler::get_Property (
                                         Deployment::Property& toconfig,
                                         Property& desc)
    {
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());
      
      //Create the ANY_Handler to
      //delegate the Any configuration
      //to.
      ANY_Handler anyhandler;
      
      //Delegate the Any configuration.
      anyhandler.get_Any (toconfig.value, desc.value ());
      
    }
  }
}
