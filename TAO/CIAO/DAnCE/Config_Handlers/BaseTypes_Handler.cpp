// $Id$

#include "BaseTypes_Handler.h"

CIAO::Config_Handlers::BaseTypes_Handler::BaseTypes_Handler()
{}
CIAO::Config_Handlers::BaseTypes_Handler::~BaseTypes_Handler()
{}

///This method takes a <CORBA::string> and an 
///<XMLSchema::string> and stores the value from
///the <XMLSchema::string> in the <CORBA::string>.
void 
CIAO::Config_Handlers::get_string (Corba::string& str, 
                                   XMLSchema::string& desc)
{
    //Convert duplicate the XMLSchema::string and
    //assign it to the Corba::string.
    str = CORBA::string_dup(desc.c_str());
}
