//$Id$

#include "Prop_Handler.h"
#include "ANY_Handler.h"

CIAO::Config_Handlers::Prop_Handler::Prop_Handler()
{}
CIAO::Config_Handlers::Prop_Handler::~Prop_Handler()
{}

            
///This method maps the values from the
///XSC object <CIAO::Config_Handlers::Property> to
///the CORBA IDL type <Deployment::Property>.    
Deployment::Property&
CIAO::Config_Handlers::Prop_Handler::get_Property(
                                    Deployment::Property& toconfig,
                                    Property& desc)
{
  toconfig.name = CORBA::string_dup (desc.name().c_str());
  
  //Create the ANY_Handler to
  //delegate the Any configuration
  //to.
  ANY_Handler anyhandler;
  
  //Delegate the Any configuration.
  anyhandler.get_Any (toconfig.value, desc.value());
  
  return toconfig;
}
