//$Id$

#include "Any_Handler.h"
#include "DT_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

void
CIAO::Config_Handler::Any_Handler::validate_ValueNode (DOMNodeIterator * iter,
                                                       const char * name)
{
  DOMNode * node = iter->nextNode ();
  XStr temp_name (node->getNodeName ());
  if (temp_name != XStr (ACE_TEXT (name)))
    {
      ACE_DEBUG ((LM_DEBUG, "CIAO::Config_Handler::Any_Handler::validate\
                             valueNode (), element mismatch expected \
                             <%s> \n", name));
      ACE_THROW (CORBA::INTERNAL ());
    }
}

void
CIAO::Config_Handler::Any_Handler::process_Any (DOMNodeIterator * iter,
                                                ::CORBA::Any &any_obj)
{
  // Process the name value definitions within the Any
  DOMNode * node = iter->nextNode ();

  // Get name of the current element
  XStr name (node->getNodeName ());

  // Initialize the ORB so that CORBA::Any will work
  int argc = 0;
  char ** argv = 0;
  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "");

  if (name == XStr (ACE_TEXT ("type")))
    {
      // Process DataType
      ::CORBA::TypeCode_var type;
      DT_Handler::process_DataType (iter, type.out (), orb);

      //Switch on the type of the BasicType
      switch (type->kind ())
        {
        case CORBA::tk_short:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "short");
          // Process the short value
          any_obj <<= static_cast<CORBA::Short> (Utils::parse_long (iter));
          break;

        case CORBA::tk_ushort:

          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "ushort");
          // Process the ushort value
          any_obj <<= static_cast<CORBA::UShort> (Utils::parse_ulong (iter));
          break;

        case CORBA::tk_long:

          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <long> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "long");
          // Process the long value
          any_obj <<= static_cast<CORBA::Long> (Utils::parse_long (iter));
          break;

        case CORBA::tk_ulong:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <ulong> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "ulong");
          // Process the ulong value
          any_obj <<= static_cast<CORBA::ULong> (Utils::parse_long (iter));
          break;

        case CORBA::tk_float:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <float> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "float");
          // Process the float value
          any_obj <<= static_cast<CORBA::Float> (Utils::parse_float (iter));
          break;

        case CORBA::tk_double:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "double");
          // Process the double value
          any_obj <<= static_cast<CORBA::Double> (Utils::parse_double (iter));
          break;

        case CORBA::tk_boolean:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "bool");
          // Process the bool value
          any_obj <<= CORBA::Any::from_boolean (Utils::parse_bool (iter));
          break;

        case CORBA::tk_char:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "char");
          // Process the char value
          any_obj <<= CORBA::Any::from_char (Utils::parse_char (iter));
          break;

        case CORBA::tk_octet:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "octet");
          // Process the octet value
          any_obj <<= CORBA::Any::from_octet (Utils::parse_octet (iter));
          break;

        case CORBA::tk_string:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "string");
          // Process the string value
          {
            char * temp = Utils::parse_string (iter);
            any_obj <<= temp;
            XMLString::release (&temp); 
          }
          break;

        case CORBA::tk_longlong:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "longlong");
          // Process the longlong value
          any_obj <<= static_cast<CORBA::LongLong> (Utils::parse_long (iter));
          break;

        case CORBA::tk_ulonglong:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "ulonglong");
          // Process the longlong value
          any_obj <<= static_cast<CORBA::ULongLong> (Utils::parse_ulong (iter));
          break;

        case CORBA::tk_longdouble:
          // Check if next node is a <value> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "value");
          // Check if the next node is a <short> node
          CIAO::Config_Handler::Any_Handler::validate_ValueNode (iter,
                                                                 "longdouble");
          // Process the longlong value -- Create a scope to define the
          // lifetime for the ret_val object created within the case statement.
          {
#if ACE_SIZEOF_LONG_DOUBLE == 16
            CORBA::LongDouble ret_str = Utils::parse_double (iter);
#else
            CORBA::LongDouble ret_str;
            char * ret_val = Utils::parse_string (iter);
            ACE_OS::memcpy (&ret_str.ld, ret_val, 16);
            XMLString::release (&ret_val);
#endif
            any_obj <<= ret_str;
          }
          break;

        default:
          ACE_DEBUG ((LM_DEBUG, "Currently the DnC handlers support only \
                                basic types, complex and recusive type \
                                support will be added\n"));
        }
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Config_Handlers::Any_Handler::process_ \
                         Any element mismatch expected <type>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

    // Release ORB resource
    CORBA::release (orb);
}
