// $Id$

#include "BaseTypes_Handler.h"

CIAO::Config_Handlers::BaseTypes_Handler::BaseTypes_Handler()
{}
CIAO::Config_Handlers::BaseTypes_Handler::~BaseTypes_Handler()
{}

///This method takes a <XMLSchema::string>
///and returns a duplicate char* that is
///created with CORBA::string_dup. 
char* 
CIAO::Config_Handlers::BaseTypes_Handler::get_string (XMLSchema::string<char>& desc)
{
    //Convert duplicate the XMLSchema::string into
    //a char*. The caller receives ownership of the pointer.
    char* str = CORBA::string_dup(desc.c_str());
    
    return str;
}

