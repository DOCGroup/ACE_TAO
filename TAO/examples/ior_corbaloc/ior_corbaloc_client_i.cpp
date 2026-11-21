
//=============================================================================
/**
 *  @file    ior_corbaloc_client_i.cpp
 *
 *  This example implements a simple client which sends a corbaloc:
 *  style url to the server and gets a response from the
 *  server to indicate that the server has received the request.
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================

#include "ior_corbaloc_client_i.h"

int
IOR_corbaloc_Client_i::run ()
{
  try
    {
      CosNaming::Name name (1);

      name.length (1);
      name[0].id = CORBA::string_dup ("STATUS");

      CORBA::Object_var const factory_object =
        this->naming_context_->resolve (name);

      corbaloc::Status_var const factory =
        corbaloc::Status::_narrow (factory_object.in ());

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference is nil for: %s\n"),
                             this->corbaloc_url_.c_str ()),
                            1);
        }

      CORBA::Boolean const ret_value =
        factory->print_status ();

      if (ret_value)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("The server has not been contacted. Error!!\n")));
          return -1;
        }
    }
  catch (CosNaming::NamingContext::NotFound const &ex)
    {
      ex._tao_print_exception ("CosNaming::NamingContext::NotFound");
      return -1;
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("A system exception on client side");
      return -1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
      return -1;
    }

  return 0;
}

int
IOR_corbaloc_Client_i::init (int &argc,
                             ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (argc < 2)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\nUsage:\n  %s [corbaloc URL for NameService]\n"),
                      argv [0]));
          return -1;
        }

      this->corbaloc_url_ = argv[1];

      CORBA::Object_var const naming_context_object =
        orb->string_to_object (this->corbaloc_url_.c_str ());

      if (CORBA::is_nil (naming_context_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot resolve Naming Service\n : client")),
                          -1);

      this->naming_context_ =
        CosNaming::NamingContextExt::_narrow (naming_context_object.in ());

      if (CORBA::is_nil (this->naming_context_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot narrow Naming Service\n :client")),
                          -1);
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("client");
      return -1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
      return -1;
    }

  return 0;
}
