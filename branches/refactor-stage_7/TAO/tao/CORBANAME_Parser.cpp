// $Id$

#include "CORBANAME_Parser.h"
#include "Stub.h"
#include "DynamicC.h"
#include "Object.h"
#include "UB_String_Arguments.h"
#include "Invocation_Adapter.h"
#include "debug.h"

#if !defined(__ACE_INLINE__)
#include "CORBANAME_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, CORBANAME_Parser, "$Id$")

TAO_CORBANAME_Parser::~TAO_CORBANAME_Parser (void)
{
}

static const char corbaname_prefix[] = "corbaname:";

int
TAO_CORBANAME_Parser::match_prefix (const char *ior_string) const
{
  return (ACE_OS::strncmp (ior_string,
                           corbaname_prefix,
                           sizeof corbaname_prefix - 1) == 0);
}

CORBA::Object_ptr
TAO_CORBANAME_Parser::
parse_string_dynamic_request_helper (CORBA::Object_ptr naming_context,
                                     ACE_CString &key_string
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                                    0);

  tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (_tao_retval.excp ());

  return _tao_retval.retn ();
}

CORBA::Object_ptr
TAO_CORBANAME_Parser::parse_string (const char *ior,
                                    CORBA::ORB_ptr orb
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  // Skip the prefix, we know it is there because this method in only
  // called if <match_prefix> returns 1.
  const char *corbaname =
    ior + sizeof corbaname_prefix - 1;

  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  ACE_TRY
    {
      char rir_prot [] = "rir:";

      // The position of the seperator between the obj_addr and key
      // string
      CORBA::ULong pos_seperator = 0;

      ACE_CString corbaname_str (corbaname, 0, 1);

      pos_seperator = corbaname_str.find ("#", 0);

      // Get the Key String
      ACE_CString key_string =
        corbaname_str.substring (pos_seperator + 1,
                                 -1);

      // Make it in a form understandable by <corbaloc> scheme
      ACE_CString corbaloc_addr ("corbaloc:", 0, 1);

      if (ACE_OS::strncmp (corbaname,
                           rir_prot,
                           sizeof (rir_prot)) != 0)
        {
          // Implies that <iiop:> protocol is to be used.
          // So .. we need to get the host address where an object of
          // type NamingContext can be found.
          // Get the obj_addr
          ACE_CString obj_addr = corbaname_str.substring (0,
                                                          pos_seperator);

          corbaloc_addr += obj_addr;

        }

      ACE_CString name_service ("/NameService", 0, 1);
      corbaloc_addr += name_service;

      // Obtain a reference to the naming context
      CORBA::Object_var name_context =
        orb->string_to_object (corbaloc_addr.c_str ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Check if the Object reference is nil.
      if (CORBA::is_nil (name_context.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve Naming Service: CORBANAME_Parser\n"),
                          0);

      CORBA::Boolean is_a =
        name_context->_is_a ("IDL:omg.org/CosNaming/NamingContextExt:1.0"
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!is_a)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot narrow Naming Service: "
                             "CORBANAME_Parser\n"),
                            0);
        }

      // Make a dynamic request for resolve_str in this naming context
      obj = this->parse_string_dynamic_request_helper (name_context.in (),
                                                       key_string
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (ex, "CORBANAME_Parser");
        }
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
