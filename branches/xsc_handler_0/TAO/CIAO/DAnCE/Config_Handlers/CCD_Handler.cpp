// $Id$

#include "CCD_Handler.h"
#include "Property_Handler.h"
#include "CPD_Handler.h"
#include "ComponentPropertyDescription_Handler.h"


namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    CCD_Handler::component_interface_descr (
        Deployment::ComponentInterfaceDescription& toconfig,
        ComponentInterfaceDescription& desc)
    {
      if(desc.UUID_p ())
        {
          toconfig.UUID =
            CORBA::string_dup (desc.UUID ().c_str ());
        }

      if(desc.label_p ())
        {
          toconfig.label =
            CORBA::string_dup (desc.label ().c_str ());
        }

      if(desc.specificType_p ())
        {
          toconfig.specificType =
            CORBA::string_dup (desc.specificType ().c_str ());
        }

      ComponentInterfaceDescription::supportedType_iterator
        end = desc.end_supportedType ();

      for (ComponentInterfaceDescription::supportedType_iterator s =
             desc.begin_supportedType ();
           s != end; ++s)
        {
          // This loop is going to be very slow! :(!
          CORBA::ULong len =
            toconfig.supportedType.length ();

          toconfig.supportedType.length (len + 1);
          toconfig.supportedType[len] =
            CORBA::string_dup ((*s).c_str ());
        }

     if (desc.idlFile_p ())
       {
         CORBA::ULong len =
           toconfig.supportedType.length ();

         if (!desc.idlFile ().empty ())
           {
             toconfig.supportedType.length (len + 1);
             toconfig.idlFile [len] =
               CORBA::string_dup (desc.idlFile ().c_str ());
           }
       }


     ComponentInterfaceDescription::configProperty_iterator pend =
       desc.end_configProperty ();

     for (ComponentInterfaceDescription::configProperty_iterator pstart =
            desc.begin_configProperty ();
          pstart != pend;
          ++pstart)
       {
         // Need to improve this. This is clearly O(n^2).
         CORBA::ULong len =
           toconfig.configProperty.length ();

         toconfig.configProperty.length (len + 1);

         Property_Handler::get_property (*pstart,
                                         toconfig.configProperty [len]);
       }


     /*******************************
     * Will need to rework the ones in the bottom like the above.
     *
     */

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

        cpd_handler.get_ComponentPortDescription (
                     toconfig.port [toconfig.port.length () - 1],
                     *port);
      }


     //If there is a
     //<CIAO::ConfigHandlers::ComponentPropertyDescription>
     //then propogate its value to the
     //<Deployment::ComponentInterfaceDescription>.
     if (desc.property_p ())
       {
         //Create the ComponentPropertyDescription handler.
         ComponentPropertyDescription_Handler cprop_handler;

         //Increase the size of the property sequence.
         toconfig.property.length (
                           toconfig.property.length () + 1);
         //Now delegate the propogation to the
         //<ComponentPropertyDescription_Handler>.
         cprop_handler.get_ComponentPropertyDescription (
                       toconfig.property [toconfig.property.length () -1],
                       desc.property ());
       }



     //The IDL for the <infoProperty> specifies
     //a sequence of <Property> structs but the schema
     //specifies <infoProperty> as a single
     //<Property>. We construct that single property
     //element and assign it to the first position in the
     //<infoProperty> sequence. We only do this if it
     //is present.
     if (desc.infoProperty_p () )
       {
         //First construct the <Deployment::Property>
         //to configure.
         Deployment::Property prop;

         //Now, propogate the values from the <desc> into <prop>.
         Property_Handler::get_property (desc.infoProperty (),
                                         prop);

         //Finally, add it to the sequence.
         toconfig.infoProperty.length (
                  toconfig.infoProperty.length () + 1);
         toconfig.infoProperty [toconfig.infoProperty.length () - 1] = prop;
       }


     return 1;
    }
  }
}
