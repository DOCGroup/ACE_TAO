//$Id$

#include "DT_Handler.h"
#include "tao/Typecode.h"
#include "Utils.h"

void
CIAO::Config_Handler::DT_Handler::process_DataType (DOMNodeIterator * iter, 
                                                    ::CORBA::TypeCode_ptr type)
{
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());
  if (name == XStr (ACE_TEXT ("Deployment:TCKind")))
  {
    // For all types defined return the appropriate typecode
    char * code_value = Utils::parse_string (iter);

    // For this piece of code to work, it is necessary that ORB_init
    // be called by the top level handler which calls the process_DataType
    // common handler.
    if (code_value == "tk_null")
     type = CORBA::_tc_null;
    else if (code_value == "tk_short")
     type = CORBA::_tc_short;
    else if (code_value == "tk_long")
     type = CORBA::_tc_long;
    else if (code_value == "tk_ushort")
     type = CORBA::_tc_ushort;
    else if (code_value == "tk_ulong")
     type = CORBA::_tc_ulong;
    else if (code_value == "tk_float")
     type = CORBA::_tc_float;
    else if (code_value == "tk_double")
     type = CORBA::_tc_double;
    else if (code_value == "tk_boolean")
     type = CORBA::_tc_boolean;
    else if (code_value == "tk_char")
     type = CORBA::_tc_char;
    else if (code_value == "tk_octet")
     type = CORBA::_tc_octet;
    else if (code_value == "tk_string")
     type = CORBA::_tc_string;
    else if (code_value == "tk_longlong")
     type = CORBA::_tc_longlong;
    else if (code_value == "tk_ulonglong")
     type = CORBA::_tc_ulonglong;
    else if (code_value == "tk_longdouble")
     type = CORBA::_tc_longdouble;
    else if (code_value == "tk_wchar")
     type = CORBA::_tc_wchar;
    else if (code_value == "tk_wstring")
     type = CORBA::_tc_wstring;
    else if (code_value == "tk_any")
     type = CORBA::_tc_any;
    else if (code_value == "tk_TypeCode")
     type = CORBA::_tc_TypeCode;
    else if (code_value == "tk_Object")
     type = CORBA::_tc_Object; 
    else
    {
      ACE_DEBUG ((LM_DEBUG, "Config_Handlers::DT_Handler::process_Datatype \
                            currently unsupported TCkind value %s encountered \
                            abortingi\n", type)); 
      ACE_THROW (CORBA::INTERNAL ());
    }
  }
  else
  {
     ACE_DEBUG ((LM_DEBUG, "Config_Handlers::Property_Handler::process_ \
                           DataType supports only <TCKind> simple type \
                           support for recursive types will be added later\n"));
      ACE_THROW (CORBA::INTERNAL ());

  } 
}

