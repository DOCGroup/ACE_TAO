// $Id$

#include "CORBANAME_Parser.h"
#include "Invocation.h"
#include "Stub.h"
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
                                     TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_ptr _tao_retval = CORBA::Object::_nil ();

  CORBA::Object_var _tao_safe_retval (_tao_retval);

  TAO_Stub *istub = naming_context->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "resolve_str",
      11,
      1,
      istub->orb_core ()
    );

  for (;;)
    {
      _tao_call.start (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      CORBA::Short _tao_response_flag = TAO_TWOWAY_RESPONSE_FLAG;

      _tao_call.prepare_header (
          ACE_static_cast (CORBA::Octet, _tao_response_flag)
           TAO_ENV_ARG_PARAMETER
        );
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
      if (!(
            (_tao_out << key_string.c_str ())
          ))
        ACE_THROW_RETURN (
            CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO),
            CORBA::Object::_nil ()
          );

      int _invoke_status = 0;
      ACE_TRY
        {
          _invoke_status = _tao_call.invoke (0, 0 TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK (CORBA::Object::_nil ());
        }
      ACE_CATCH (CORBA::UNKNOWN, ex)
        {
          ACE_UNUSED_ARG (ex);

          ACE_THROW_RETURN (
              CORBA::BAD_PARAM (TAO_OMG_VMCID | 10,
                                CORBA::COMPLETED_YES),
              CORBA::Object::_nil ()
            );
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      if (_invoke_status == TAO_INVOKE_RESTART)
        {
          _tao_call.restart_flag (1);
          continue;
        }
      if (_invoke_status != TAO_INVOKE_OK)
        {
          // @@ Is there any way we can reach this point?  Any
          //    USER_EXCEPTION response will be caught by the UNKNOWN
          //    catch block above.  Any SYSTEM_EXCEPTION response will
          //    be simply raised, and any RESTART is handled
          //    already...  leave this here "just in case".
          ACE_THROW_RETURN (
              CORBA::BAD_PARAM (TAO_OMG_VMCID | 10,
                                CORBA::COMPLETED_YES),
              CORBA::Object::_nil ()
            );
        }
      TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
      if (!(
            (_tao_in >> _tao_safe_retval.inout ())
          ))
        {
          ACE_THROW_RETURN (
              CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
              CORBA::Object::_nil ()
            );
        }
      break;
    }

  return _tao_safe_retval._retn ();
}

CORBA::Object_ptr
TAO_CORBANAME_Parser::parse_string (const char *ior,
                                    CORBA::ORB_ptr orb
                                    TAO_ENV_ARG_DECL)
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
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Check if the Object reference is nil.
      if (CORBA::is_nil (name_context.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve Naming Service: CORBANAME_Parser\n"),
                          0);

      CORBA::Boolean is_a =
        name_context->_is_a ("IDL:omg.org/CosNaming/NamingContextExt:1.0"
                              TAO_ENV_ARG_PARAMETER);
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
                                                        TAO_ENV_ARG_PARAMETER);
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
