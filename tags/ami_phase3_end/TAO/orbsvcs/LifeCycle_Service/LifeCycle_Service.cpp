// $Id$

// ============================================================================
//
// = FILENAME
//    Life_Cycle_Service.cpp
//
// = DESCRIPTION
//    The server for the LifeCycleService of the quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "tao/corba.h"
#include "LifeCycle_Service.h"

ACE_RCSID(LifeCycle_Service, LifeCycle_Service, "$Id$")

static const char usage [] = "[-? |\n[-O[RBport] ORB port number]]";

Life_Cycle_Service_Server::Life_Cycle_Service_Server (void)
{
}

Life_Cycle_Service_Server::~Life_Cycle_Service_Server (void)
{
  TAO_TRY
    {
      // Unbind the Factory Finder.
      CosNaming::Name generic_Factory_Name (2);
      generic_Factory_Name.length (2);
      generic_Factory_Name[0].id = CORBA::string_dup ("LifeCycle_Service");
      this->namingContext_var_->unbind (generic_Factory_Name,TAO_TRY_ENV);
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

int
Life_Cycle_Service_Server::init (int argc,
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


  ACE_NEW_RETURN (this->life_Cycle_Service_i_ptr_,
                  Life_Cycle_Service_i(),
                  -1);

  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (this->life_Cycle_Service_i_ptr_,
                                 env);

  // Failure while activating the  Factory Finder object
  // @@ TODO Is this the right way to check this? Shouldn't env
  // contain an exception?
  if (env.exception () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init: Failure while activating the LifeCycle Service Impl.\n"),
                      -1);


  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  // Register the LifeCycle Service with the Naming Service.
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
      namingContext_var_ = CosNaming::NamingContext::_narrow (namingObj_var.in (),
							      TAO_TRY_ENV);

      if (CORBA::is_nil (namingContext_var_.in ()))
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));


      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
                  "Have a proper reference to the Naming Service.\n"));

      CosNaming::Name life_Cycle_Service_Name (1);
      life_Cycle_Service_Name.length (1);
      life_Cycle_Service_Name[0].id = CORBA::string_dup ("Life_Cycle_Service");

      namingContext_var_->bind (life_Cycle_Service_Name,
			       this->life_Cycle_Service_i_ptr_->_this(TAO_TRY_ENV),
			       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG,
                  "Bound the LifeCycle Service to the Naming Context.\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;

  return 0;
}


int
Life_Cycle_Service_Server::run (CORBA::Environment &)
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
Life_Cycle_Service_Server::parse_args (void)
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
  Life_Cycle_Service_Server life_Cycle_Service_Server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_LifeCycleService: Life_Cycle_Service_Server \n\n"));
  TAO_TRY
    {
      if (life_Cycle_Service_Server.init (argc,
					  argv,
					  TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          life_Cycle_Service_Server.run (TAO_TRY_ENV);
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


