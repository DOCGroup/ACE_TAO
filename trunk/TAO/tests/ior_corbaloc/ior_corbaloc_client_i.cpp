// $Id$
//

// ===========================================================
//
// = LIBRARY
//    TAO/tests/ior_corbaloc/
//
// = FILENAME
//    ior_corbaloc_client_i.cpp
//
// = DESCRIPTION
//    This example implements a simple client which sends a corbaloc:
//    style url to the server and gets a response from the
//    server to indicate that the server has received the request.
//
// = AUTHORS
//    Priyanka Gontla <pgontla@ece.uci.edu>
//
//============================================================

#include "ior_corbaloc_client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// Constructor
IOR_corbaloc_Client_i::IOR_corbaloc_Client_i (void)
{
}

IOR_corbaloc_Client_i::~IOR_corbaloc_Client_i (void)
{
}

int
IOR_corbaloc_Client_i::run (CORBA::Environment &ACE_TRY_ENV)
{

  ACE_TRY
    {
      CosNaming::Name name (1);

      name.length (1);
      name[0].id = CORBA::string_dup ("STATUS");

      // Resolve the name
      CORBA::Object_var factory_object =
        this->naming_context_->resolve (name,
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Narrow
      corbaloc::Status_var factory =
        corbaloc::Status::_narrow (factory_object.in (), ACE_TRY_ENV);
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
        factory->print_status (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (ret_value == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "The server has been contacted !!\n",
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
IOR_corbaloc_Client_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (this->argc_,
                         this->argv_,
                         "" /* the ORB name, it can be anything! */,
                         ACE_TRY_ENV);

      // Get a reference to the Naming Service
      CORBA::Object_var naming_context_object =
        orb->string_to_object (this->argv_[1], 
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_context_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve Naming Service\n : client"),
                          1);

      // Narrow to get the correct reference
      this->naming_context_ =
        CosNaming::NamingContextExt::_narrow (naming_context_object.in (),
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->naming_context_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot narrow Naming Service\n :client"),
                          1);
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
