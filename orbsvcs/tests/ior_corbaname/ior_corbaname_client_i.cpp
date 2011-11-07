
//=============================================================================
/**
 *  @file    ior_corbaname_client_i.cpp
 *
 *  $Id$
 *
 *  This example implements a simple client which sends a corbaname:
 *  style url to the server and gets a response from the
 *  server to indicate that the server has received the request.
 *
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#include "ior_corbaname_client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/SString.h"

// Constructor
IOR_corbaname_Client_i::IOR_corbaname_Client_i (void)
{
}

// Destructor
IOR_corbaname_Client_i::~IOR_corbaname_Client_i (void)
{
}

int
IOR_corbaname_Client_i::run (void)
{

  try
    {
      CosNaming::Name name (1);

      name.length (1);
      name[0].id = CORBA::string_dup ("STATUS");

      // The corbaname URL contains the host at which an NamingContext
      // Object can be found and also the stringified form of an entry
      // in the Naming Service seperated by '#'. For simplicity,
      // in this example, we are specifying the stringified form of
      // the binding directly without using <to_string> method on
      // <name>. "#" refers to the seperator between the host and the
      // entry.

      ACE_CString corbaname_url (ACE_TEXT_ALWAYS_CHAR (this->argv_[1]), 0, 1);

      // Append the seperator '#' to the host.
      corbaname_url += "#";

      // Append the stringified name to the resultant.
      corbaname_url += "STATUS";

      // Resolve the stringified name.
      CORBA::Object_var factory_object =
        this->orb_->string_to_object (corbaname_url.c_str ());

      // Narrow
      corbaname::Status_var factory =
        corbaname::Status::_narrow (factory_object.in ());

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             this->argv_[1]),
                            1);
        }

      // Invoke a request on the server
      CORBA::Boolean const ret_value = factory->print_status ();

      if (!ret_value)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "The server has been successfully contacted.\n",
                      0));
        }
    }
  catch (const CosNaming::NamingContext::NotFound& ex)
    {
      ex._tao_print_exception ("CosNaming::NamingContext::NotFound");
    }
  catch (const CORBA::SystemException& ex)
    {
      ex._tao_print_exception ("A system exception on client side");
      return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
      return -1;
    }

  return 0;
}

int
IOR_corbaname_Client_i::init (int argc, ACE_TCHAR **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      // First initialize the ORB, that will remove some arguments...
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      // There must be at least one argument, the file that has to be
      // retrieved
      if (this->argc_ < 2)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: Address of naming context not specified\n",
                           this->argv_[0]),
                           -1);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
      return 1;
    }

  return 0;
}
