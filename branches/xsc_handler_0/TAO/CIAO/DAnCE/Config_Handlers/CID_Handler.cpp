// $Id$

#include "CID_Handler.h"

CIAO::Config_Handlers::CID_Handler::CID_Handler()
{}
CIAO::Config_Handlers::CID_Handler::~CID_Handler()
{}

Deployment::ComponentInterfaceDescription&
CIAO::Config_Handlers::CID_Handler::get_ComponentInterfaceDescription
(Deployment::ComponentInterfaceDescription& toconfig,
 ComponentInterfaceDescription& desc)
{
    
    toconfig.UUID = 
        CORBA::string_dup (desc.UUID().c_str());
    toconfig.label = 
        CORBA::string_dup (desc.label().c_str());
    toconfig.specificType = 
        CORBA::string_dup (desc.specificType().c_str());
        
    //Copy the values of the sequence types idlFile and 
    //supporteType to the 
    //<Deployment::ComponentInterfaceDescription>.
    if(!desc.supportedType().empty())
    {
     toconfig.supportedType.length(1);
     toconfig.supportedType[0] = 
              CORBA::string_dup(desc.supportedType().c_str());
    }
    
    if(!desc.idlFile().empty())
    {
     toconfig.idlFile.length(1);
     toconfig.idlFile[0] = 
              CORBA::string_dup(desc.idlFile().c_str());
    }
    
    //Create the handler for the
    //<ComponentPortDescriptions>.
	//CPDHandler cpd_handler(); 
	
    
  
	return toconfig; 	
}
