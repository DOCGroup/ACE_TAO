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
IOR_corbaloc_Client_i::run (TAO_ENV_SINGLE_ARG_DECL)
{

  ACE_TRY
    {
      CosNaming::Name name (1);

      name.length (1);
      name[0].id = CORBA::string_dup ("STATUS");

      // Resolve the name
      CORBA::Object_var factory_object =
        this->naming_context_->resolve (name TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow
      corbaloc::Status_var factory =
        corbaloc::Status::_narrow (factory_object.in ()
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference is nil for: %s\n",
                             corbaloc_url_.c_str() ),
                            1);
        }

      // Invoke a request on the server
      CORBA::Boolean ret_value =
        factory->print_status (TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ret_value != 0)
        {
        ACE_DEBUG ((LM_DEBUG,
                    "The server has not been contacted. Error!!\n",
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
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
IOR_corbaloc_Client_i::init (int& argc,
                             char *argv[]
                             TAO_ENV_ARG_DECL)
{

  ACE_TRY
    {

      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "" /* the ORB name, it can be anything! */
                         TAO_ENV_ARG_PARAMETER);

      if(argc < 2){
              ACE_DEBUG((LM_DEBUG, "\nUsage:\n  %s [corbaloc URL for NameService]\n", argv[0]));
              ACE_OS::exit(-1);
      }

      corbaloc_url_ = argv[1];

      // Get a reference to the Naming Service
      CORBA::Object_var naming_context_object =
        orb->string_to_object (corbaloc_url_.c_str()
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_context_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve Naming Service\n : client"),
                          -1);

      // Narrow to get the correct reference
      this->naming_context_ =
        CosNaming::NamingContextExt::_narrow (naming_context_object.in ()
                                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->naming_context_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot narrow Naming Service\n :client"),
                          -1);
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "client");
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
