// $Id$

#include "ComponentAttributesSetter.h"
#include "Deployment/Deployment_ApplicationC.h"
//#include "Cdmw_ccm_dance1_cif.stub.hpp"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include <tao/DynamicInterface/Context.h>
#include <tao/DynamicInterface/Request.h>
#include "DAnCE/Logger/Log_Macros.h"

//bool read_config_value( const ACE_CString & name,
//                        const Deployment::Properties & prop,
//                        CORBA::Any_out value)
//    throw()
//{
//    ACE_CString cdmw_name = name;
//    bool found = false;
//    CORBA::ULong len = prop.length();
//    for (CORBA::ULong count = 0; count < len; ++count)
//    {
//        if ( cdmw_name.compare(prop[count].name.in()) == 0 )
//        {
//            value = new CORBA::Any(prop[count].value);
//            found = true;
//            break;
//        }
//    }
//    return found;
//}


ComponentAttributesSetter::ComponentAttributesSetter()
{
}

ComponentAttributesSetter::~ComponentAttributesSetter()
{
}

void
ComponentAttributesSetter::SetComponentAttributes (ACE_CString /*componentName*/,
                                                   Components::CCMObject_ptr obj,
                                                   const Deployment::Properties& prop,
                                                   CORBA::ORB_ptr orb)
{
  DANCE_DEBUG ( (LM_DEBUG, "[%M] ComponentAttributesSetter::SetComponentAttributes - started\n"));
  for (unsigned int i = 0; i < prop.length(); i++)
    {
      ACE_CString name = prop[i].name.in();
      if (name.find ("cdmw") != ACE_CString::npos)
        {
          continue;
        }
      DANCE_DEBUG ( (LM_DEBUG, "[%M] ComponentAttributesSetter::SetComponentAttributes - check attribute name %C\n", name.c_str()));
      ACE_CString method = "_set_";
      method += prop[i].name.in();
      CORBA::NVList_var argList;
      CORBA::NamedValue_ptr argValue;
      CORBA::NamedValue_var result = 0;
      CORBA::Request_ptr request;

      orb->create_list (1, argList);
      argValue = argList->add_value (prop[i].name.in(), prop[i].value, CORBA::ARG_IN);
      CORBA::Any* value = argValue->value();
      const char* szvalue;
      *value >>= szvalue;
      DANCE_DEBUG ( (LM_DEBUG, "[%M] ComponentAttributesSetter::SetComponentAttributes - set attribute value to %C\n", szvalue));
      try
        {
          obj->_create_request (CORBA::Context::_nil(),
                                method.c_str(),
                                argList.in (),
                                result.out (),
                                request,
                                0);

          request->invoke();
          CORBA::release (request);
        }
      catch (const CORBA::Exception &e)
        {
          CORBA::release (request);
          e._tao_print_exception ("ComponentAttributesSetter.cpp::SetComponentAttributes ");
          throw ::Deployment::StartError();
        }
      //Question - How exceptions will be processed, rised by invoked method
    }
  DANCE_DEBUG ( (LM_DEBUG, "[%M] ComponentAttributesSetter::SetComponentAttributes - finished\n"));
}
