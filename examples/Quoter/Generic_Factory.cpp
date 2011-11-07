
//=============================================================================
/**
 *  @file    Generic_Factory.cpp
 *
 *  $Id$
 *
 *  The server for the GenericFactory for the quoter example.
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "Generic_Factory.h"

#include "orbsvcs/LifeCycleServiceC.h"

Quoter_Generic_Factory_Server::Quoter_Generic_Factory_Server (void)
: use_LifeCycle_Service_ (0),
  debug_level_ (1)
{
}

Quoter_Generic_Factory_Server::~Quoter_Generic_Factory_Server (void)
{
  try
    {
      // Unbind the Quoter Factory Finder.
      CosNaming::Name generic_Factory_Name (2);
      generic_Factory_Name.length (2);
      generic_Factory_Name[0].id = CORBA::string_dup ("IDL_Quoter");
      generic_Factory_Name[1].id = CORBA::string_dup ("Quoter_Generic_Factory");
      if (!CORBA::is_nil (this->quoterNamingContext_var_.in ()))
        this->quoterNamingContext_var_->unbind (generic_Factory_Name);
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
    }
}

int
Quoter_Generic_Factory_Server::init (int argc,
                                     ACE_TCHAR *argv[])
{
  const char *exception_message = "Null Message";

  try
    {
      int result = 0;
      // Initialize the ORB Manager
      exception_message = "While initing the orb_manager";
      result = this->orb_manager_.init (argc, argv);

      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);

      // Activate the POA manager
      exception_message = "While activating the POA manager";
      result = this->orb_manager_.activate_poa_manager ();

      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "activate_poa_manager"), -1);

      // Copy them, because parse_args expects them there.
      this->argc_ = argc;
      this->argv_ = argv;
      this->parse_args ();

      ACE_NEW_RETURN (this->quoter_Generic_Factory_i_ptr_,
                      Quoter_Generic_Factory_i(this->debug_level_),
                      -1);

      // Activate the object.
      exception_message = "While activating the Generic Factory";
      CORBA::String_var str  =
        this->orb_manager_.activate (this->quoter_Generic_Factory_i_ptr_);

      // Print the IOR.
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Generic_Factory: IOR is: <%C>\n", str.in ()));

      // Register the Quoter GenericFactory with the Naming Service.
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "Generic_Factory: Trying to get a reference to the Naming Service.\n"));

      // Get the Naming Service object reference.
      exception_message = "While getting the Naming Service Reference";
      CORBA::Object_var namingObj_var =
        orb_manager_.orb()->resolve_initial_references ("NameService");

      if (CORBA::is_nil (namingObj_var.in ()))
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      exception_message = "While narrowing the Naming Context";
      CosNaming::NamingContext_var namingContext_var =
        CosNaming::NamingContext::_narrow (namingObj_var.in ());

      if (CORBA::is_nil (namingContext_var.in ()))
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));


      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "Generic_Factory: Have a proper reference to the Naming Service.\n"));

      // Get the IDL_Quoter naming context.
      CosNaming::Name quoterContextName (1);  // max = 1
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      exception_message = "While resolving the Quoter";
      CORBA::Object_var quoterNamingObj_var =
        namingContext_var->resolve (quoterContextName);

      exception_message = "While narrowing the Quoter";
      quoterNamingContext_var_ =
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in ());

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "Generic_Factory: Have a proper reference to the Quoter Naming Context.\n"));

      // Bind the Quoter GenericFactory to the IDL_Quoter naming
      // context.
      CosNaming::Name quoter_Generic_Factory_Name (1);
      quoter_Generic_Factory_Name.length (1);
      quoter_Generic_Factory_Name[0].id = CORBA::string_dup ("Quoter_Generic_Factory");

      exception_message = "Generic_Factory::_this";
      CORBA::Object_var gf_obj = this->quoter_Generic_Factory_i_ptr_->_this();

      exception_message = "While binding the Generic Factory";
      quoterNamingContext_var_->bind (quoter_Generic_Factory_Name,
                                      gf_obj.in ());

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "Generic_Factory: Bound the Quoter GenericFactory to the Quoter Naming Context.\n"));

      // now the Quoter GenericFactory is bound to the Naming Context
      // the Generic Factory should try to register itself to the closest
      // Life Cycle Service is order to be called.

      if (this->use_LifeCycle_Service_)
      {
        // get the Quoter_Life_Cycle_Service
        CosNaming::Name life_Cycle_Service_Name (1);
        life_Cycle_Service_Name.length (1);
        life_Cycle_Service_Name[0].id = CORBA::string_dup ("Life_Cycle_Service");

        exception_message = "While resolving the Life Cycle Service";
        CORBA::Object_var life_Cycle_Service_Obj_var =
          namingContext_var->resolve (life_Cycle_Service_Name);

        exception_message = "While narrowing the Life Cycle Service";
        LifeCycleService::Life_Cycle_Service_var  life_Cycle_Service_var =
          LifeCycleService::Life_Cycle_Service::_narrow (life_Cycle_Service_Obj_var.in ());

        if (this->debug_level_ >= 2)
          ACE_DEBUG ((LM_DEBUG, "Generic_Factory: Have a proper reference to Life Cycle Service.\n"));

        exception_message = "While _this on Generic Factory";
        CORBA::Object_var object_var = this->quoter_Generic_Factory_i_ptr_->_this();

        exception_message = "While registering the generic factory";
        life_Cycle_Service_var->register_factory ("Quoter_Generic_Factory",  // name
                                                  "Bryan 503",               // location
                                                  "Generic Factory",         // description
                                                  object_var.in ());

        if (this->debug_level_ >= 2)
          ACE_DEBUG ((LM_DEBUG,
                      "Generic_Factory: Registered the Quoter GenericFactory to the Life Cycle Service.\n"));
      }

    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Generic_Factory_Server::init - %C\n", exception_message));
      ex._tao_print_exception (
        "Quoter_Generic_Factory_Server::init");
      return -1;
    }


  return 0;
}

int
Quoter_Generic_Factory_Server::run (void)
{
  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "\nQuoter Example: Quoter_Generic_Factory_Server is running\n"));

  orb_manager_.orb()->run ();

  return 0;
}

// Function get_options.

u_int
Quoter_Generic_Factory_Server::parse_args (void)
{
  ACE_Get_Opt get_opt (this->argc_, this->argv_, ACE_TEXT("l?d:"));
  int opt;
  int exit_code = 0;

  while ((opt = get_opt ()) != EOF)
    switch (opt)
      {
      case 'd':  // debug flag.
        this->debug_level_ = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'l':
        this->use_LifeCycle_Service_ = 1;
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
                    " [-l]               - Use the lifecycle service\n"
                    "\n",
                    this->argv_[0]));
        ACE_OS::exit (exit_code);
        break;
      }
  return 0;
}

// function main

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Quoter_Generic_Factory_Server quoter_Generic_Factory_Server;

  try
    {
      if (quoter_Generic_Factory_Server.init (argc,
                                              argv) == -1)
        return 1;
      else
        {
          quoter_Generic_Factory_Server.run ();
        }
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  return 0;
}
