// $Id$

#include "ANY_Handler.h"

CIAO::Config_Handlers::ANY_Handler::ANY_Handler()
{}
CIAO::Config_Handlers::ANY_Handler::~ANY_Handler()
{}



using CIAO::Config_Handlers;

CORBA::Any& 
CIAO::Config_Handlers::ANY_Handler::get_Any(
                        CORBA::Any& toconfig, Any& desc)
{ 
 
 //Get the value that should be assigned to the Any.
 DataValue value = desc.value();
 
 //Here, we check each type to see if 
 //it is present. If a type is listed as 
 //present, we can assume that it is the 
 //intended value for the Any. This relieves
 //us from the burden of having to check the
 //type field on <desc>.
 if(value.short_p()){
  //what we want to do here is create
  //a utility class that handles converting
  //the XMLSchema::short_, etc.. classes to the
  //equivalent Corba class.
 }
 if(value.long_p()){
   
 }
 
 return toconfig;
}
