// $Id$

#include "BaseTypes_Handler.h"

CIAO::Config_Handlers::BaseTypes_Handler::BaseTypes_Handler()
{}
CIAO::Config_Handlers::BaseTypes_Handler::~BaseTypes_Handler()
{}

///This method takes a <CORBA::string> and an 
///<XMLSchema::string> and stores the value from
///the <XMLSchema::string> in the <CORBA::string>.
CORBA::String_var& 
CIAO::Config_Handlers::get_string (CORBA::String_var& str, 
                                   XMLSchema::string<char>& desc)
{
    //Convert duplicate the XMLSchema::string and
    //assign it to the Corba::string.
    str = CORBA::string_dup(desc.c_str());
    
    return str;
}

