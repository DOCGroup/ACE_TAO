// $Id$

// ============================================================================
//
// = FILENAME
//    Generic_Factory.cpp
//
// = DESCRIPTION
//    The server for the GenericFactory for the quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "tao/corba.h"
#include "Generic_Factory.h"

static const char usage [] = "[-? |\n[-O[RBport] ORB port number]]";

Quoter_Generic_Factory_Server::Quoter_Generic_Factory_Server (void)
{
}

Quoter_Generic_Factory_Server::~Quoter_Generic_Factory_Server (void) 
{
  TAO_TRY
    {
      // Unbind the Quoter Factory Finder.
      CosNaming::Name generic_Factory_Name (2);
      generic_Factory_Name.length (2);
      generic_Factory_Name[0].id = CORBA::string_dup ("IDL_Quoter");
      generic_Factory_Name[1].id = CORBA::string_dup ("Quoter_Generic_Factory");
      this->quoterNamingContext_var_->unbind (generic_Factory_Name,TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
    }
  TAO_ENDTRY;
}

Quoter_Generic_Factory_Server::init (int argc,
                                     char *argv[],
                                     CORBA::Environment& env)
{
  if (this->orb_manager_.init (argc,
                               argv,
                               env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "init"),
                      -1);

  // Copy them, because parse_args expects them there.
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();


  ACE_NEW_RETURN (this->quoter_Generic_Factory_Impl_ptr_,
                  Quoter_Generic_Factory_Impl(),
                  -1);

  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (this->quoter_Generic_Factory_Impl_ptr_,
                                 env);

  // Failure while activating the Quoter Factory Finder object
  if (str == 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "init: Failure while activating the Quoter Generic Factory Impl.\n"),
                      -1);


  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  // Register the Quoter GenericFactory with the Naming Service.
  TAO_TRY 
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Trying to get a reference to the Naming Service.\n"));

      // Get the Naming Service object reference.
      CORBA::Object_var namingObj_var = 
        orb_manager_.orb()->resolve_initial_references ("NameService");
      TAO_CHECK_ENV;

      if (CORBA::is_nil (namingObj_var.in ())) 
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      CosNaming::NamingContext_var namingContext_var =
        CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                           TAO_TRY_ENV);

      if (CORBA::is_nil (namingContext_var.in ())) 
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));

      
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
                  "Have a proper reference to the Naming Service.\n"));

      // Get the IDL_Quoter naming context.
      CosNaming::Name quoterContextName (1);  // max = 1 
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      CORBA::Object_var quoterNamingObj_var = 
        namingContext_var->resolve (quoterContextName,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      quoterNamingContext_var_ = 
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in (),
                                           TAO_TRY_ENV);
       
      ACE_DEBUG ((LM_DEBUG,
                  "Have a proper reference to the Quoter Naming Context.\n"));

      // Bind the Quoter GenericFactory to the IDL_Quoter naming
      // context.
      CosNaming::Name quoter_Generic_Factory_Name (1);
      quoter_Generic_Factory_Name.length (1);
      quoter_Generic_Factory_Name[0].id = CORBA::string_dup ("Quoter_Generic_Factory");

      quoterNamingContext_var_->bind (quoter_Generic_Factory_Name,
                                      this->quoter_Generic_Factory_Impl_ptr_->_this(TAO_TRY_ENV),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
                  "Bound the Quoter GenericFactory to the Quoter Naming Context.\n"));

      // now the Quoter GenericFactory is bound to the Naming Context
      // the Generic Factory should try to register itself to the closest 
      // Life Cycle Service is order to be called.
      
      // get the Quoter_Life_Cycle_Service
      CosNaming::Name quoter_Life_Cycle_Service_Name (1);
      quoter_Life_Cycle_Service_Name.length (1);
      quoter_Life_Cycle_Service_Name[0].id = CORBA::string_dup ("Quoter_Life_Cycle_Service");

      CORBA::Object_var quoter_Life_Cycle_Service_Obj_var = 
        quoterNamingContext_var_->resolve (quoter_Life_Cycle_Service_Name,
					   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      Stock::Quoter_Life_Cycle_Service_var  quoter_Life_Cycle_Service_var = 
        Stock::Quoter_Life_Cycle_Service::_narrow (quoter_Life_Cycle_Service_Obj_var.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
       
      ACE_DEBUG ((LM_DEBUG,
                  "Have a proper reference to the Quoter Life Cycle Service.\n"));

      CORBA::Object_var object_var = this->quoter_Generic_Factory_Impl_ptr_->_this(TAO_TRY_ENV);

      // @@ necessary if you specify an ORB port not equal to 0
      // ACE_Time_Value time_Value = 0;
      // orb_manager_.orb()->run (&time_Value);
      // register with the Quoter_Life_Cycle_Service
      quoter_Life_Cycle_Service_var->register_factory ("Quoter_Generic_Factory",  // name
						      "Bryan 503",               // location
						      "Generic Factory",         // description 
						      object_var,
						      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
                  "Registered the Quoter GenericFactory to the Quoter Life Cycle Service.\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Quoter_Generic_Factory_Server::init: Exception");
    }
  TAO_ENDTRY;
  

  return 0;
}

int
Quoter_Generic_Factory_Server::run (CORBA::Environment& env)
{
  if (orb_manager_.orb()->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "run"),
                      -1);
  return 0;
}

// Function get_options.

u_int
Quoter_Generic_Factory_Server::parse_args (void)
{
  // We need the 'O' in get_opt () because we also want to have ORB
  // parameters, they all start with 'O'.
  ACE_Get_Opt get_opt (this->argc_, this->argv_, "O?");
  int opt;

  while ((opt = get_opt ()) != EOF)
    switch (opt) 
      {
      case '?':
        ACE_DEBUG ((LM_DEBUG,
                    "Usage: %s %s\n",
                    this->argv_[0], usage));
        ACE_OS::exit (0);
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: unknown arg, -%c\n"
                           "Usage: %s %s\n",
                           this->argv_[0], char (opt),
                           this->argv_[0],
                           usage),
                          1);
      }
  return 0;
}

// function main

int
main (int argc, char *argv [])
{
  Quoter_Generic_Factory_Server quoter_Generic_Factory_Server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Quoter: Quoter_Generic_Factory_Server \n\n"));
  TAO_TRY
    {
      if (quoter_Generic_Factory_Server.init (argc,
                                              argv,
                                              TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          quoter_Generic_Factory_Server.run (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}
