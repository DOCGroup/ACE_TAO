// $Id$
//

// ===========================================================
//
// = LIBRARY
//    TAO/tests/ior_corbaname/
//
// = FILENAME
//    ior_corbaname_client_i.cpp
//
// = DESCRIPTION
//    This example implements a simple client which sends a corbaname:
//    style url to the server and gets a response from the
//    server to indicate that the server has received the request.
//
// = AUTHORS
//    Priyanka Gontla <pgontla@ece.uci.edu>
//
//============================================================

#include "ior_corbaname_client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// Constructor
IOR_corbaname_Client_i::IOR_corbaname_Client_i (void)
{
}

// Destructor
IOR_corbaname_Client_i::~IOR_corbaname_Client_i (void)
{
}

int
IOR_corbaname_Client_i::run (ACE_ENV_SINGLE_ARG_DECL)
{

  ACE_TRY
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

      ACE_CString corbaname_url (this->argv_[1], 0, 1);

      // Append the seperator '#' to the host.
      corbaname_url += "#";

      // Append the stringified name to the resultant.
      corbaname_url += "STATUS";

      // Resolve the stringified name.
      CORBA::Object_var factory_object =
        this->orb_->string_to_object (corbaname_url.c_str ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow
      corbaname::Status_var factory =
        corbaname::Status::_narrow (factory_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             this->argv_[1]),
                            1);
        }

      // Invoke a request on the server
      CORBA::Boolean ret_value =
        factory->print_status (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ret_value == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "The server has been successfully contacted.\n",
                      0));
        }
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "CosNaming::NamingContext::NotFound");
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "A system exception on client side");
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
IOR_corbaname_Client_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      // First initialize the ORB, that will remove some arguments...
      this->orb_ =
        CORBA::ORB_init (this->argc_,
                         this->argv_,
                         "" /* the ORB name, it can be anything! */
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // There must be at least one argument, the file that has to be
      // retrieved
      if (this->argc_ < 2)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Address of naming context not specified\n",
                           this->argv_[0]),
                           -1);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
