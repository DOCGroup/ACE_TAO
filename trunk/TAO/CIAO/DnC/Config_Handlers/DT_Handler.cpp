//$Id$

#include "DT_Handler.h"
#include "tao/Typecode.h"
#include "Utils.h"

void
CIAO::Config_Handler::DT_Handler::process_basic_tc (DOMNodeIterator * iter,
                                                    ::CORBA::TypeCode_ptr &type)
{

    // For all types defined return the appropriate typecode
    char * code_value = Utils::parse_string (iter);

    // For this piece of code to work, it is necessary that ORB_init
    // be called by the top level handler which calls the
    // process_DataType common handler.
    if (ACE_OS::strcmp (code_value,
                        "tk_null") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
    else if (ACE_OS::strcmp (code_value,
                             "tk_short") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_short);
    else if (ACE_OS::strcmp (code_value,
                             "tk_long") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_long);
    else if (ACE_OS::strcmp (code_value,
                             "tk_ushort") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);
    else if (ACE_OS::strcmp (code_value,
                             "tk_ulong") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_ulong);
    else if (ACE_OS::strcmp (code_value,
                             "tk_float") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_float);
    else if (ACE_OS::strcmp (code_value,
                             "tk_double") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_double);
    else if (ACE_OS::strcmp (code_value,
                             "tk_boolean") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);
    else if (ACE_OS::strcmp (code_value,
                             "tk_char") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_char);
    else if (ACE_OS::strcmp (code_value,
                             "tk_octet") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_octet);
    else if (ACE_OS::strcmp (code_value,
                             "tk_string") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_string);
    else if (ACE_OS::strcmp (code_value,
                             "tk_longlong") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_longlong);
    else if (ACE_OS::strcmp (code_value,
                             "tk_ulonglong") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_ulonglong);
    else if (ACE_OS::strcmp (code_value,
                             "tk_longdouble") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_longdouble);
    else if (ACE_OS::strcmp (code_value,
                             "tk_wchar") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_wchar);
    else if (ACE_OS::strcmp (code_value,
                             "tk_wstring") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_wstring);
    else if (ACE_OS::strcmp (code_value,
                             "tk_any") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_any);
    else if (ACE_OS::strcmp (code_value,
                             "tk_TypeCode") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_TypeCode);
    else if (ACE_OS::strcmp (code_value,
                             "tk_Object") == 0)
      type = CORBA::TypeCode::_duplicate (CORBA::_tc_Object);
    else
    {
      ACE_DEBUG ((LM_DEBUG, "Config_Handlers::DT_Handler::process_Datatype \
                            unsupported TCkind value %s encountered \
                            abortingi\n", type));
      ACE_THROW (CORBA::INTERNAL ());
    }
}

void
CIAO::Config_Handler::DT_Handler::process_DataType (DOMNodeIterator * iter,
                                                    ::CORBA::TypeCode_ptr &type,
                                                    ::CORBA::ORB_ptr )
{
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());
  if (name == XStr (ACE_TEXT ("kind")))
    DT_Handler::process_basic_tc (iter, type);
  else
  {
    ACE_DEBUG ((LM_DEBUG, "Config_Handlers::Property_Handler::process_ \
                           DataType supports only TCKind types\
                           support for recursive types will be added later\n"));
      ACE_THROW (CORBA::INTERNAL ());

  }
}
