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

Life_Cycle_Service_Server::Life_Cycle_Service_Server (void)
:   debug_level_ (1)
{
}

Life_Cycle_Service_Server::~Life_Cycle_Service_Server (void)
{
  ACE_TRY_NEW_ENV
    {
      // Unbind the Factory Finder.
      CosNaming::Name generic_Factory_Name (2);
      generic_Factory_Name.length (2);
      generic_Factory_Name[0].id = CORBA::string_dup ("LifeCycle_Service");
      this->namingContext_var_->unbind (generic_Factory_Name TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "User Exception");
    }
  ACE_ENDTRY;
}

int
Life_Cycle_Service_Server::init (int argc,
                                     char *argv[]
                                     TAO_ENV_ARG_DECL)
{
  int retval = 0;

  retval = this->orb_manager_.init (argc,
                                    argv
                                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init"),
                      -1);


  // Activate the POA manager
  retval = this->orb_manager_.activate_poa_manager (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "activate_poa_manager"), -1);

  ACE_CHECK_RETURN (-1);
  // Copy them, because parse_args expects them there.
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();


  ACE_NEW_RETURN (this->life_Cycle_Service_i_ptr_,
                  Life_Cycle_Service_i(this->debug_level_),
                  -1);

  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (this->life_Cycle_Service_i_ptr_
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (this->debug_level_ >= 2)
    ACE_DEBUG ((LM_DEBUG, "LifeCycle_Service: IOR is: <%s>\n", str.in ()));

  // Register the LifeCycle Service with the Naming Service.
  ACE_TRY
    {
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "LifeCycle_Service: Trying to get a reference to the Naming Service.\n"));

      // Get the Naming Service object reference.
      CORBA::Object_var namingObj_var =
        orb_manager_.orb()->resolve_initial_references ("NameService" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (namingObj_var.in ()))
        ACE_ERROR ((LM_ERROR,
                   " LifeCycle_Service: Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      namingContext_var_ = CosNaming::NamingContext::_narrow (namingObj_var.in ()
                                                              TAO_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      if (CORBA::is_nil (namingContext_var_.in ()))
        ACE_ERROR ((LM_ERROR,
                   "LifeCycle_Service: Unable get the Naming Service.\n"));

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "LifeCycle_Service: Have a proper reference to the Naming Service.\n"));

      CosNaming::Name life_Cycle_Service_Name (1);
      life_Cycle_Service_Name.length (1);
      life_Cycle_Service_Name[0].id = CORBA::string_dup ("Life_Cycle_Service");

      CORBA::Object_ptr tmp = this->life_Cycle_Service_i_ptr_->_this(TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      namingContext_var_->bind (life_Cycle_Service_Name,
                                tmp
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "LifeCycle_Service: Bound the LifeCycle Service to the Naming Context.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Life_Cycle_Service_Server::init");
    }
  ACE_ENDTRY;

  return 0;
}


int
Life_Cycle_Service_Server::run (TAO_ENV_SINGLE_ARG_DECL)
{
  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "\nLifeCycle Service: Life_Cycle_Service_Server is running\n"));

  orb_manager_.orb()->run (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}


// Function get_options.

u_int
Life_Cycle_Service_Server::parse_args (void)
{
  ACE_Get_Opt get_opt (this->argc_, this->argv_, "?d:");
  int opt;
  int exit_code = 0;

  while ((opt = get_opt ()) != EOF)
    switch (opt)
      {
      case 'd':  // debug flag.
        this->debug_level_ = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        exit_code = 1;
        ACE_ERROR ((LM_ERROR,
                    "%s: unknown arg, -%c\n",
                    this->argv_[0], char(opt)));
      case '?':
        ACE_DEBUG ((LM_DEBUG,
                    "usage:  %s"
                    " [-d] <debug level> - Set the debug level\n"
                    " [-?]               - Prints this message\n"
                    "\n",
                    this->argv_[0]));
        ACE_OS::exit (exit_code);
        break;
      }
  return 0;
}

// function main

int
main (int argc, char *argv [])
{
  Life_Cycle_Service_Server life_Cycle_Service_Server;

  ACE_TRY_NEW_ENV
    {
      int check = life_Cycle_Service_Server.init (argc,
                                                  argv
                                                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (check)
        return 1;
      else
        {
          life_Cycle_Service_Server.run (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "LifeCycleService::main");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}
