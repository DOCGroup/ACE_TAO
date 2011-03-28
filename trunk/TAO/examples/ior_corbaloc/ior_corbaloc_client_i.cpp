
//=============================================================================
/**
 *  @file    ior_corbaloc_client_i.cpp
 *
 *  $Id$
 *
 *  This example implements a simple client which sends a corbaloc:
 *  style url to the server and gets a response from the
 *  server to indicate that the server has received the request.
 *
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


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
IOR_corbaloc_Client_i::run (void)
{

  try
    {
      CosNaming::Name name (1);

      name.length (1);
      name[0].id = CORBA::string_dup ("STATUS");

      // Resolve the name
      CORBA::Object_var factory_object =
        this->naming_context_->resolve (name);

      // Narrow
      corbaloc::Status_var factory =
        corbaloc::Status::_narrow (factory_object.in ());

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference is nil for: %s\n",
                             corbaloc_url_.c_str() ),
                            1);
        }

      // Invoke a request on the server
      CORBA::Boolean ret_value =
        factory->print_status ();

      if (ret_value != 0)
        {
        ACE_DEBUG ((LM_DEBUG,
                    "The server has not been contacted. Error!!\n",
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
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
    }

  return 0;
}

int
IOR_corbaloc_Client_i::init (int& argc,
                             ACE_TCHAR *argv[])
{
  try
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if(argc < 2){
              ACE_DEBUG((LM_DEBUG, "\nUsage:\n  %s [corbaloc URL for NameService]\n", argv[0]));
              ACE_OS::exit(-1);
      }

      corbaloc_url_ = argv[1];

      // Get a reference to the Naming Service
      CORBA::Object_var naming_context_object =
        orb->string_to_object (corbaloc_url_.c_str());

      if (CORBA::is_nil (naming_context_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve Naming Service\n : client"),
                          -1);

      // Narrow to get the correct reference
      this->naming_context_ =
        CosNaming::NamingContextExt::_narrow (naming_context_object.in ());

      if (CORBA::is_nil (this->naming_context_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot narrow Naming Service\n :client"),
                          -1);
    }
  catch (const CORBA::SystemException& ex)
    {
      ex._tao_print_exception ("client");
      return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
    }

  return 0;
}
