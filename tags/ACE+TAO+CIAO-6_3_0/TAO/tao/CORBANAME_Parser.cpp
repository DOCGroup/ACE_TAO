// -*- C++ -*-
// $Id$

#include "tao/CORBANAME_Parser.h"

#if (TAO_HAS_CORBANAME_PARSER == 1)

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"
#include "tao/UB_String_Arguments.h"
#include "tao/Invocation_Adapter.h"
#include "tao/debug.h"

#include "ace/Log_Msg.h"
#include "ace/SString.h"
#include "ace/OS_NS_string.h"

static const char corbaname_prefix[] = "corbaname:";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CORBANAME_Parser::~TAO_CORBANAME_Parser (void)
{
}

bool
TAO_CORBANAME_Parser::match_prefix (const char *ior_string) const
{
  return (ACE_OS::strncmp (ior_string,
                           corbaname_prefix,
                           sizeof corbaname_prefix - 1) == 0);
}

CORBA::Object_ptr
TAO_CORBANAME_Parser::
parse_string_dynamic_request_helper (CORBA::Object_ptr naming_context,
                                     ACE_CString &key_string)
{
  TAO::Arg_Traits<CORBA::Object>::ret_val _tao_retval;
  TAO::Arg_Traits<CORBA::Char *>::in_arg_val _tao_id (key_string.c_str ());

  TAO::Argument *_tao_signature [] =
      {
        &_tao_retval,
        &_tao_id
      };

  TAO::Invocation_Adapter tao_call (naming_context,
                                    _tao_signature,
                                    2,
                                    "resolve_str",
                                    11,
                                    TAO::TAO_CO_NONE | TAO::TAO_CO_THRU_POA_STRATEGY);

  tao_call.invoke (0, 0);

  return _tao_retval.retn ();
}

CORBA::Object_ptr
TAO_CORBANAME_Parser::parse_string (const char *ior, CORBA::ORB_ptr orb)
{

  // Skip the prefix, we know it is there because this method in only
  // called if <match_prefix> returns 1.
  const char *corbaname =
    ior + sizeof corbaname_prefix - 1;

  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  try
    {
      // The position of the seperator between the obj_addr and key
      // string
      ACE_CString::size_type pos_seperator = 0;

      ACE_CString corbaname_str (corbaname, 0, 1);

      pos_seperator = corbaname_str.find ("#", 0);

      // Get the Key String
      ACE_CString key_string;

      if (pos_seperator != ACE_CString::npos)
        {
          key_string = corbaname_str.substring (pos_seperator + 1,
                                                ACE_CString::npos);
        }

      // Prepare a suitable corbaloc string for the name service.
      // CORBALOC assumes "NameService" for the object key if none
      // is provided, so just pass everything between "corbaname:"
      // and "#" as the address
      ACE_CString corbaloc_addr ("corbaloc:", 0, 1);
      corbaloc_addr += corbaname_str.substring (0, pos_seperator);

      // Obtain a reference to the naming context
      CORBA::Object_var name_context =
        orb->string_to_object (corbaloc_addr.c_str ()
                               );

      // Check if the Object reference is nil.
      if (CORBA::is_nil (name_context.in ()))
        TAOLIB_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve Naming Service: CORBANAME_Parser\n"),
                          0);

      CORBA::Boolean is_a =
        name_context->_is_a ("IDL:omg.org/CosNaming/NamingContextExt:1.0");

      if (!is_a)
        {
          TAOLIB_ERROR_RETURN ((LM_ERROR,
                             "Cannot narrow Naming Service: "
                             "CORBANAME_Parser\n"),
                            0);
        }

      if (key_string.length () != 0)
        {

          // Make a dynamic request for resolve_str in this naming context
          obj = this->parse_string_dynamic_request_helper (name_context.in (),
                                                           key_string);
        }
      else
        { // There was no key string which implies that the caller wants
          // the object reference of the naming service.
          obj = name_context._retn ();
        }
    }
  catch (const ::CORBA::SystemException& ex)
    {
      if (TAO_debug_level >= 4)
        {
          ex._tao_print_exception ("CORBANAME_Parser");
        }
    }

  return obj;
}


ACE_STATIC_SVC_DEFINE (TAO_CORBANAME_Parser,
                       ACE_TEXT ("CORBANAME_Parser"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CORBANAME_Parser),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_CORBANAME_Parser)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBANAME_PARSER == 1 */

