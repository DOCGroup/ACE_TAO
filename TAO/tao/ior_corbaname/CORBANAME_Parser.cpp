// $Id$

#include "CORBANAME_Parser.h"

#include "ace/Profile_Timer.h"

#include "tao/Object_Loader.h"
#include "tao/Object.h"
#include "tao/corba.h"
#include "tao/ORB.h"
#include "tao/Exception.h"
#include "tao/Environment.h"
#include "ace/Read_Buffer.h"

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
parse_string_dynamic_request_helper (CosNaming::NamingContextExt_var
                                     &naming_context,
                                     ACE_CString &key_string,
                                     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::Object_ptr obj = CORBA::Object::_nil ();
  ACE_TRY
    {
      // Now that you got a reference to the naming context, use
      // resolve_str.
      CORBA::Request_var rs_req (naming_context->_request ("resolve_str",
                                                       ACE_TRY_ENV));
      ACE_TRY_CHECK;
      
      // resolve_str takes a str as an argument: so we must include
      // that in the request
      rs_req->add_in_arg () <<= key_string.c_str ();
  
      // Set the return type
      rs_req->set_return_type (CORBA::_tc_Object);
      
      // Finally invoke the <resolve_str> operation.
      rs_req->invoke (ACE_TRY_ENV);
      ACE_TRY_CHECK;
  
      // Extract the returned object reference from the request
      rs_req->return_value () >>= CORBA::Any::to_object (obj);
      
      if (CORBA::is_nil (obj))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not obtain object <%s>\n"),
                          0);
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "CORBANAME_Parser");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return obj;
}

CORBA::Object_ptr
TAO_CORBANAME_Parser::parse_string (const char *ior,
                                    CORBA::ORB_ptr orb,
                                    CORBA::Environment &ACE_TRY_ENV)
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
      CORBA::Object_var name_context_obj =
        orb->string_to_object (corbaloc_addr.c_str (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Check if the Object reference is nil.
      if (CORBA::is_nil (name_context_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve Naming Service: CORBANAME_Parser\n"),
                          0);
      
      // Narrow to get the correct reference
      CosNaming::NamingContextExt_var naming_context =
        CosNaming::NamingContextExt::_narrow (name_context_obj.in (),
                                              ACE_TRY_ENV);
       ACE_TRY_CHECK;

       // Check if the object reference is nil.
      if (CORBA::is_nil (naming_context.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot narrow Naming Service: CORBANAME_Parser\n"),
                          0);

      // Make a dynamic request for resolve_str in this naming context
      obj = this->parse_string_dynamic_request_helper (naming_context,
                                                       key_string,
                                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "CORBANAME_Parser");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  
  return obj;
}

ACE_FACTORY_DEFINE (TAO_IOR_CORBANAME, TAO_CORBANAME_Parser)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
