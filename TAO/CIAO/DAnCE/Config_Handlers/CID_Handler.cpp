// $Id$

#include "CID_Handler.h"
#include "Prop_Handler.h"
#include "CPD_Handler.h"

typedef ::std::vector< ::CIAO::Config_Handlers::ComponentPortDescription >::iterator port_iterator;

namespace CIAO
{
  namespace Config_Handlers
  {
    CID_Handler::CID_Handler (void)
    {
    }

    CID_Handler::~CID_Handler (void)
    {
    }

    bool
    CID_Handler::get_component_interface_descr (
        Deployment::ComponentInterfaceDescription& toconfig,
        ComponentInterfaceDescription& desc)
    {

    // We first set up some of the basic properties such as UUID. We
    // check to make sure each one is actually present before
    // assigning it to the corresponding field in <toconfig>.
    if(desc.UUID_p ())
      {
        toconfig.UUID =
          CORBA::string_dup (desc.UUID ().c_str ());
      }
    else if(desc.label_p ())
      {
        toconfig.label =
          CORBA::string_dup (desc.label ().c_str ());
      }
    else if(desc.specificType_p ())
      {
        toconfig.specificType =
          CORBA::string_dup (desc.specificType ().c_str ());
      }
    else if (desc.supportedType_p ())
      {
        //Copy the values of the sequence types idlFile and
        //supportedType to the
        //<Deployment::ComponentInterfaceDescription>.
        if(!desc.supportedType().empty())
        {
         toconfig.supportedType.length (1);
         toconfig.supportedType[0] =
                  CORBA::string_dup (desc.supportedType().c_str());
        }
    }

    //Make sure the <idlFile> property
    //is actually present before attempting
    //any operations on it.
    if(desc.idlFile_p () ){
        if(!desc.idlFile().empty())
        {
         toconfig.idlFile.length (1);
         toconfig.idlFile[0] =
                  CORBA::string_dup (desc.idlFile().c_str());
        }
    }


    Prop_Handler propertyhandler;

    //The IDL for the <configProperty> specifies
    //a sequence of <Property> structs but the schema
    //specifies <configProperty> as a single
    //<Property>. We construct that single property
    //element and assign it to the first position in the
    //<configProperty> sequence. We only do this if it
    //is present.
    if(desc.configProperty_p () )
    {
     //First construct the <Deployment::Property>
     //to configure.
     Deployment::Property prop;

     //Now, propogate the values from the <desc> into <prop>.
     propertyhandler.get_Property (prop,desc.configProperty());

     //Finally, add it to the sequence.
     toconfig.configProperty.length (1);
     toconfig.configProperty[0] = prop;
    }

    //Create the handler for the
    //<ComponentPortDescriptions>.
        CPD_Handler cpd_handler;

    //Iterate through each of the XSC Component
    //Port Descriptions and use the CPD_Handler
    //to propogate their values into the <port>
    //sequence of <Deployment::ComponentInterfaceDescription>
    for (ComponentInterfaceDescription::port_iterator
           port (desc.begin_port ());
           port != desc.end_port ();
           ++port)
    {
         toconfig.port.length (toconfig.port.length () + 1);
         cpd_handler.get_ComponentPortDescription(
                     toconfig.port[toconfig.port.length () - 1],
                     *port);
    }

    /*ComponentPropertyDescription stuff
     *
     *
     *
     *
     *
     */








        return toconfig;
}
