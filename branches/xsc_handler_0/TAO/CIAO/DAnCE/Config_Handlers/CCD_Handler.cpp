
// $Id$


#include "CCD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "ccd.hpp"
#include "Property_Handler.h"
#include "CPD_Handler.h"
#include "ComponentPropertyDescription_Handler.h"



namespace CIAO
{
  namespace Config_Handlers
  {

    CCD_Handler::CCD_Handler (void)
    {
    }

    CCD_Handler::~CCD_Handler (void)
    {
    }


    void
    CCD_Handler::comp_interface_descr (
                    const ComponentInterfaceDescription& desc,
                    Deployment::ComponentInterfaceDescription& toconfig)
    {


      
      toconfig.UUID=
           CORBA::string_dup (desc.UUID ().c_str ());
      
      toconfig.label=
           CORBA::string_dup (desc.label ().c_str ());
      
      toconfig.specificType=
           CORBA::string_dup (desc.specificType ().c_str ());
      
      ComponentInterfaceDescription::supportedType_const_iterator supportedType_end =
        desc.end_supportedType ();
      ComponentInterfaceDescription::supportedType_const_iterator supportedType_beg =
        desc.begin_supportedType ();  
        
      CORBA::ULong supportedType_length = toconfig.supportedType.length ();
      size_t supportedType_delta = supportedType_end - supportedType_beg;
      supportedType_length += supportedType_delta;
      
      toconfig.supportedType.length (supportedType_length);  
      size_t supportedType_count = 0;     
           
      for (ComponentInterfaceDescription::supportedType_const_iterator
           item (desc.begin_supportedType ());
           item != supportedType_end;
           ++item)
        {	 
           toconfig.supportedType[supportedType_length - (supportedType_delta - supportedType_count)] =
             CORBA::string_dup (item->c_str ());
           ++supportedType_count;
        }
      
      ComponentInterfaceDescription::idlFile_const_iterator idlFile_end =
        desc.end_idlFile ();
      ComponentInterfaceDescription::idlFile_const_iterator idlFile_beg =
        desc.begin_idlFile ();  
        
      CORBA::ULong idlFile_length = toconfig.idlFile.length ();
      size_t idlFile_delta = idlFile_end - idlFile_beg;
      idlFile_length += idlFile_delta;
      
      toconfig.idlFile.length (idlFile_length);  
      size_t idlFile_count = 0;     
           
      for (ComponentInterfaceDescription::idlFile_const_iterator
           item (desc.begin_idlFile ());
           item != idlFile_end;
           ++item)
        {	 
           toconfig.idlFile[idlFile_length - (idlFile_delta - idlFile_count)] =
             CORBA::string_dup (item->c_str ());
           ++idlFile_count;
        }
      
      ComponentInterfaceDescription::configProperty_const_iterator configProperty_end =
        desc.end_configProperty ();
      ComponentInterfaceDescription::configProperty_const_iterator configProperty_beg =
        desc.begin_configProperty ();  
        
      CORBA::ULong configProperty_length = toconfig.configProperty.length ();
      size_t configProperty_delta = configProperty_end - configProperty_beg;
      configProperty_length += configProperty_delta;
      
      toconfig.configProperty.length (configProperty_length);  
      size_t configProperty_count = 0;        

      for (ComponentInterfaceDescription::configProperty_const_iterator
           item (configProperty_beg);
           item != configProperty_end;
           ++item)
        {	        
          Property_Handler::property (
            *item,
            toconfig.configProperty[configProperty_length - (configProperty_delta - configProperty_count)]);
          ++configProperty_count;  
        }
      
      ComponentInterfaceDescription::port_const_iterator port_end =
        desc.end_port ();
      ComponentInterfaceDescription::port_const_iterator port_beg =
        desc.begin_port ();  
        
      CORBA::ULong port_length = toconfig.port.length ();
      size_t port_delta = port_end - port_beg;
      port_length += port_delta;
      
      toconfig.port.length (port_length);  
      size_t port_count = 0;        

      for (ComponentInterfaceDescription::port_const_iterator
           item (port_beg);
           item != port_end;
           ++item)
        {	        
          CPD_Handler::comp_port_descr (
            *item,
            toconfig.port[port_length - (port_delta - port_count)]);
          ++port_count;  
        }
      
     /* if (desc.property_p ())
        {
          CORBA::ULong length = toconfig.property.length ();
          
          toconfig.property.length (length + 1);
          ComponentPropertyDescription_Handler::comp_property_descr (
            desc.property (),
            toconfig.property[length - 1]);
        }
      
      if (desc.infoProperty_p ())
        {
          CORBA::ULong length = toconfig.infoProperty.length ();
          
          toconfig.infoProperty.length (length + 1);
          Property_Handler::property (
            desc.infoProperty (),
            toconfig.infoProperty[length - 1]);
        }*/

      
    }

  }

}
