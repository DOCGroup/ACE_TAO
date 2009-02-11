// $Id$

#include "tao/DynamicInterface/Request.h"

#include "ComponentAttributesSetter.h"
//#include "Cdmw_ccm_dance1_cif.stub.hpp"
#include "tao/DynamicInterface/DII_CORBA_methods.h"
#include "tao/DynamicInterface/Context.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "Deployment/Deployment_ApplicationC.h"

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
                                                   ::CORBA::Object_ptr obj,
                                                   const Deployment::Properties& prop,
                                                   CORBA::ORB_ptr )
{
  DANCE_TRACE ("ComponentAttributesSetter::SetComponentAttributes");

  for (CORBA::ULong i = 0; i < prop.length(); i++)
    {
      ACE_CString name = prop[i].name.in();
      // Ignore configuration properties, since attributes can't have . in them, this seems like a good method.
      if (name.find (".") != ACE_CString::npos)
        {
          continue;
        }
      DANCE_DEBUG ((LM_DEBUG, DLINFO
                    "ComponentAttributesSetter::SetComponentAttributes - "
                    "Populating attribute name %C\n", name.c_str()));
      ACE_CString method = "_set_";
      method += prop[i].name.in();

      ::CORBA::Request_var req;

      try
        {
          req = obj->_request (method.c_str ());
          req->add_in_arg ("x") = prop[i].value;

          req->invoke();
        }
      catch (const CORBA::BAD_OPERATION &)
        {
          DANCE_ERROR ((LM_WARNING, DLINFO
                        "ComponentAttributesSetter::SetComponentAttributes - "
                        "Caught BAD_OPERATION while trying to set attribute %C\n",
                        name.c_str ()));
        }
      catch (const CORBA::Exception &e)
        {
          CORBA::release (req);
          e._tao_print_exception ("ComponentAttributesSetter.cpp::SetComponentAttributes ");
          throw ::Deployment::StartError();
        }
      //Question - How exceptions will be processed, rised by invoked method
    }
}
