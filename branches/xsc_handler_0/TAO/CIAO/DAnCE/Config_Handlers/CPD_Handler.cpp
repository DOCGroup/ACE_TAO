// $Id$


#include "CPD_Handler.h"


using CIAO::Config_Handlers;

CIAO::Config_Handlers::CPD_Handler::CPD_Handler()
{}
CIAO::Config_Handlers::CPD_Handler::~CPD_Handler()
{}

using CIAO::Config_Handlers;

///This method maps the values from the
///XSC object <ComponentPortDescription> to
///the CORBA IDL type <Deployment::ComponentPortDescription>.
Deployment::ComponentPortDescription &
CIAO::Config_Handlers::CPD_Handler::get_ComponentPortDescription(
                   Deployment::ComponentPortDescription& toconfig,
				   ComponentPortDescription& desc)
{
	//We make sure that a value exists for supportedType
    //before increasing the size of the sequence.
	if(!desc.supportedType().empty()){
        toconfig.supportedType.length(1);
		toconfig.supportedType[0] = 
            CORBA::string_dup(desc.supportedType().c_str());
	}


	toconfig.name = CORBA::string_dup(desc.name().c_str());


	toconfig.specificType =
            CORBA::string_dup(desc.specificType().c_str());

    //The DnC spec maps these CORBA IDL booleans to 
    //strings. Therefore, we set them to true if a 
    //value was provided (they aren't an empty string)
    //and false otherwise.
	toconfig.provider = !desc.provider().empty();
	toconfig.exclusiveProvider = !desc.exclusiveProvider().empty();
	toconfig.exclusiveUser = !desc.exclusiveUser().empty();
	toconfig.optional = !desc.optional().empty();

    //Return the Deployment::ComponentPortDescription
	return toconfig;
	
}
