//=============================================================================
/**
 *  @file    ior_corbaname_client_i.cpp
 *
 *  This example implements a simple client which sends a corbaname:
 *  style url to the server and gets a response from the
 *  server to indicate that the server has received the request.
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#include "ior_corbaname_client_i.h"

#include <string>

int
IOR_corbaname_Client_i::run ()
{
  try
    {
      CosNaming::Name name (1);

      name.length (1);
      name[0].id = CORBA::string_dup ("STATUS");

      // The corbaname URL contains the host at which an NamingContext
      // Object can be found and also the stringified form of an entry
      // in the Naming Service separated by '#'. For simplicity,
      // in this example, we are specifying the stringified form of
      // the binding directly without using <to_string> method on
      // <name>. "#" refers to the seperator between the host and the
      // entry.

      std::string corbaname_url (ACE_TEXT_ALWAYS_CHAR (this->name_service_address_.c_str ()));

      // Append the seperator '#' to the host.
      corbaname_url += "#";

      // Append the stringified name to the resultant.
      corbaname_url += "STATUS";

      // Resolve the stringified name.
      CORBA::Object_var const factory_object =
        this->orb_->string_to_object (corbaname_url.c_str ());

      // Narrow
      corbaname::Status_var const factory =
        corbaname::Status::_narrow (factory_object.in ());

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil\n"),
                             this->name_service_address_.c_str ()),
                            1);
        }

      // Invoke a request on the server
      CORBA::Boolean const ret_value = factory->print_status ();

      if (!ret_value)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("The server has been successfully contacted.\n")));
        }
    }
  catch (CosNaming::NamingContext::NotFound const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("CosNaming::NamingContext::NotFound"));
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("A system exception on client side"));
      return -1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("client"));
      return -1;
    }

  return 0;
}

int
IOR_corbaname_Client_i::init (int argc, ACE_TCHAR **argv)
{
  try
    {
      // First initialize the ORB, that will remove some arguments...
      this->orb_ = CORBA::ORB_init (argc, argv);

      // There must be at least one argument, the file that has to be
      // retrieved
      if (argc < 2)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%s: Address of naming context not specified\n"),
                           argv[0]),
                           -1);
      this->name_service_address_ = argv[1];
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("client"));
      return 1;
    }

  return 0;
}
