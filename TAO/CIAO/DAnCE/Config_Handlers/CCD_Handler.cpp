// $Id$

#include "CCD_Handler.h"
#include "ccd.hpp"
#include "Property_Handler.h"
#include "CPD_Handler.h"
#include "ComponentPropertyDescription_Handler.h"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    CCD_Handler::component_interface_descr (
        const ComponentInterfaceDescription &desc,
        Deployment::ComponentInterfaceDescription& toconfig)
    {
      if (desc.UUID_p ())
        toconfig.UUID =
          CORBA::string_dup (desc.UUID ().c_str ());

      if (desc.label_p ())
        {
          toconfig.label =
            CORBA::string_dup (desc.label ().c_str ());
        }

      if (desc.specificType_p ())
        {
          toconfig.specificType =
            CORBA::string_dup (desc.specificType ().c_str ());
        }

      ComponentInterfaceDescription::supportedType_const_iterator
        end = desc.end_supportedType ();

      for (ComponentInterfaceDescription::supportedType_const_iterator s =
             desc.begin_supportedType ();
           s != end;
           ++s)
        {
          // This loop is going to be very slow! :(!
          CORBA::ULong len =
            toconfig.supportedType.length ();

          toconfig.supportedType.length (len + 1);
          toconfig.supportedType[len] =
            CORBA::string_dup ((*s).c_str ());
        }

      ComponentInterfaceDescription::idlFile_const_iterator
        eidl = desc.end_idlFile ();

      for (ComponentInterfaceDescription::idlFile_const_iterator sidl=
             desc.begin_idlFile ();
           sidl != eidl;
           ++sidl)
        {
          // @@ Another n^2 algorithm
          CORBA::ULong len =
            toconfig.idlFile.length ();

          toconfig.idlFile.length (len + 1);

          toconfig.idlFile [len] =
            (*sidl).c_str ();
        }

     ComponentInterfaceDescription::configProperty_const_iterator pend =
       desc.end_configProperty ();

     for (ComponentInterfaceDescription::configProperty_const_iterator pstart =
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

    for (ComponentInterfaceDescription::port_const_iterator
           port (desc.begin_port ());
           port != desc.end_port ();
           ++port)
      {
        CORBA::ULong len =
          toconfig.port.length ();

        toconfig.port.length (len + 1);

        CPD_Handler::component_port_description (
          *port,
          toconfig.port[len]);
      }

#if 0
    // @@ MAJO: I don't think we need to handle this now, since they
    // are not needed for this round. IOW, we don't really understand
    // how to use this stuff ;)
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
#endif /*if 0*/

     return 1;
    }

   ComponentInterfaceDescription
   CCD_Handler::component_interface_descr (
     const ::Deployment::ComponentInterfaceDescription&  src)
   {
     ComponentInterfaceDescription cid;

     XMLSchema::string< char > uuid ((src.UUID));
     XMLSchema::string< char > label ((src.label));
     XMLSchema::string< char > specifict ((src.specificType));
    
     cid.UUID (uuid);
     cid.label (label);
     cid.specificType (specifict);

     ::CORBA::ULong total = src.supportedType.length ();
     for (size_t i = 0; i < total; ++i)
       {
         XMLSchema::string< char > curr ((src.supportedType[i]));
         cid.add_supportedType (curr);
       }
     
     total = src.idlFile.length ();
     for (size_t j = 0; i < total; ++j)
       {
         XMLSchema::string< char > curr ((src.idlFile[j]));
         cid.add_idlFile (curr);
       }

     total = src.configProperty.length ();
     for (size_t k = 0; i < total; ++k)
       {
         cid.add_configProperty (
           Property_Handler::get_property (
             src.configProperty[k]));
       }
     
     total = src.port.length ();
     for (size_t l = 0; i < total; ++l)
       {
         cid.add_port (
           CPD_Handler::component_port_description (
             src.port[l]));
       }


     return cid;
   }
  }
}
