// $Id$

#include "tao/DynamicInterface/Request.h"

#include "ComponentAttributesSetter.h"
#include "tao/DynamicInterface/DII_CORBA_methods.h"
#include "tao/DynamicInterface/Context.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "Deployment/Deployment_ApplicationC.h"

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
      ACE_CString const name = prop[i].name.in();

      // Ignore configuration properties, since attributes can't have . in them, this seems like a good method.
      if (name.find (".") != ACE_CString::npos)
        {
          continue;
        }

      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                    ACE_TEXT("ComponentAttributesSetter::SetComponentAttributes - ")
                    ACE_TEXT("Populating attribute name <%C>\n"), name.c_str()));
      ACE_CString method = "_set_";
      method += prop[i].name.in();

      try
        {
          ::CORBA::Request_var req = obj->_request (method.c_str ());
          req->add_in_arg ("x") = prop[i].value;
          req->invoke();
        }
      catch (const CORBA::Exception &e)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT("ComponentAttributesSetter::SetComponentAttributes - ")
                        ACE_TEXT("ERROR: Exception while trying to set attribute <%C>\n"),
                        name.c_str ()));
          e._tao_print_exception ("ComponentAttributesSetter.cpp::SetComponentAttributes - ");
          throw ::Deployment::StartError();
        }
    }
}
