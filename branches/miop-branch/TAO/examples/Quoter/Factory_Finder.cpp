// $Id$

// ============================================================================
//
// = FILENAME
//    FactoryFinder.cpp
//
// = DESCRIPTION
//    A Factory Finder for the Quoter example. This example conforms
//    to the CosLifeCycle Factory Finder notion.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "tao/corba.h"
#include "Factory_Finder.h"

ACE_RCSID(Quoter, Factory_Finder, "$Id$")

Quoter_Factory_Finder_Server::Quoter_Factory_Finder_Server (void)
:   debug_level_ (1)
{
  // Nothing
}

Quoter_Factory_Finder_Server::~Quoter_Factory_Finder_Server (void)
{
  ACE_TRY_NEW_ENV
    {
      // Unbind the Quoter Factory Finder.
      CosNaming::Name factory_Finder_Name (2);
      factory_Finder_Name.length (2);
      factory_Finder_Name[0].id = CORBA::string_dup ("IDL_Quoter");
      factory_Finder_Name[1].id = CORBA::string_dup ("Quoter_Factory_Finder");
      if (this->quoterNamingContext_var_.ptr () != 0)
        this->quoterNamingContext_var_->unbind (factory_Finder_Name TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Could not unbind the Factor Finder from the Name Service\n"));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "~Quoter_Factor_Finder_Server");
    }
  ACE_ENDTRY;
}

int
Quoter_Factory_Finder_Server::init (int argc,
                                    char *argv[]
                                    TAO_ENV_ARG_DECL)
{
  const char *exception_message = "Null Message";

  ACE_TRY
    {
      exception_message = "While ORB_Manager::init";
      if (this->orb_manager_.init (argc,
                                   argv
                                   TAO_ENV_ARG_PARAMETER) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "init"),
                          -1);
      ACE_TRY_CHECK;

      // Activate the POA manager
      exception_message = "While activating the POA manager";
      int result = this->orb_manager_.activate_poa_manager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "activate_poa_manager"), -1);

      // Copy them, because parse_args expects them there.
      this->argc_ = argc;
      this->argv_ = argv;

      this->parse_args ();


      ACE_NEW_RETURN (this->quoter_Factory_Finder_i_ptr_,
                      Quoter_Factory_Finder_i(this->debug_level_),
                      -1);

      // Activate the object.
      exception_message = "Failure while activating the Quoter Factory Finder Impl";
      CORBA::String_var str  =
        this->orb_manager_.activate (this->quoter_Factory_Finder_i_ptr_
                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Print the IOR.
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Factory Finder: IOR is: <%s>\n", str.in ()));

      // Register the Quoter Factory Finder with the Naming Service
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,"Factory Finder: Trying to get a reference to the Naming Service.\n"));

      // Get the Naming Service object reference.
      exception_message = "While resolving the Name Service";
      CORBA::Object_var namingObj_var =
        orb_manager_.orb()->resolve_initial_references ("NameService" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (namingObj_var.in ()))
        ACE_ERROR ((LM_ERROR,
                    " (%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      exception_message = "While narrowing the Naming Context";
      CosNaming::NamingContext_var namingContext_var =
        CosNaming::NamingContext::_narrow (namingObj_var.in ()
                                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the IDL_Quoter naming context.
      CosNaming::Name quoterContextName (1);  // max = 1
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      exception_message = "While resolving the Quoter Naming Context";
      CORBA::Object_var quoterNamingObj_var =
        namingContext_var->resolve (quoterContextName TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      exception_message = "While narrowing the Quoter Naming Context";
      quoterNamingContext_var_ =
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in ()
                                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "Factory Finder: Have a proper reference to the Quoter Naming Context.\n"));

      // Bind the QuoterFactory Finder to the IDL_Quoter naming
      // context.
      CosNaming::Name quoter_Factory_Finder_Name_ (1);
      quoter_Factory_Finder_Name_.length (1);
      quoter_Factory_Finder_Name_[0].id = CORBA::string_dup ("Quoter_Factory_Finder");

      exception_message = "Factory_Factory::_this";
      CORBA::Object_var ff_obj = this->quoter_Factory_Finder_i_ptr_->_this(TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      exception_message = "While binding the Factory Finder";
      quoterNamingContext_var_->bind (quoter_Factory_Finder_Name_,
                                      ff_obj.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "Factory_Finder: Bound the Quoter Factory Finder to the Quoter Naming Context.\n"));
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Factor_Finder_Server::init - %s\n", exception_message));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "SYS_EX");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Quoter_Factory_Finder_Server::run (TAO_ENV_SINGLE_ARG_DECL)
{
  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "\nQuoter Example: Quoter_Factory_Finder_Server is running\n"));

  orb_manager_.orb()->run (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}


// Function get_options.

u_int
Quoter_Factory_Finder_Server::parse_args (void)
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
  Quoter_Factory_Finder_Server quoter_Factory_Finder_Server;

  ACE_TRY_NEW_ENV
    {
      int result = quoter_Factory_Finder_Server.init (argc, argv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result == -1)
        return 1;
      else
        {
          quoter_Factory_Finder_Server.run (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}
