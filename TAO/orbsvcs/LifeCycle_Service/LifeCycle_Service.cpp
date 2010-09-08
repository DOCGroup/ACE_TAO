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

#include "LifeCycle_Service.h"

#include "ace/Argv_Type_Converter.h"
#include "ace/OS_main.h"

Life_Cycle_Service_Server::Life_Cycle_Service_Server (void)
:   debug_level_ (1)
{
}

Life_Cycle_Service_Server::~Life_Cycle_Service_Server (void)
{
  try
    {
      // Unbind the Factory Finder.
      CosNaming::Name generic_Factory_Name (2);
      generic_Factory_Name.length (2);
      generic_Factory_Name[0].id = CORBA::string_dup ("LifeCycle_Service");
      this->namingContext_var_->unbind (generic_Factory_Name);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("User Exception");
    }
}

int
Life_Cycle_Service_Server::init (int argc,
                                 ACE_TCHAR *argv[])
{
  int retval = 0;

  // Copy command line parameter.
  ACE_Argv_Type_Converter command(argc, argv);

  retval = this->orb_manager_.init (command.get_argc(),
                                    command.get_TCHAR_argv());

  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("%p\n"),
                       ACE_TEXT("init")),
                      -1);

  // Activate the POA manager
  retval = this->orb_manager_.activate_poa_manager ();

  if (retval == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "activate_poa_manager"), -1);


  this->parse_args (command.get_argc(), command.get_TCHAR_argv());

  ACE_NEW_RETURN (this->life_Cycle_Service_i_ptr_,
                  Life_Cycle_Service_i(this->debug_level_),
                  -1);

  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (this->life_Cycle_Service_i_ptr_);

  if (this->debug_level_ >= 2)
    ACE_DEBUG ((LM_DEBUG, "LifeCycle_Service: IOR is: <%C>\n", str.in ()));

  // Register the LifeCycle Service with the Naming Service.
  try
    {
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("LifeCycle_Service: Trying to get a reference to the Naming Service.\n")));

      // Get the Naming Service object reference.
      CORBA::Object_var namingObj_var =
        orb_manager_.orb()->resolve_initial_references ("NameService");

      if (CORBA::is_nil (namingObj_var.in ()))
        ACE_ERROR ((LM_ERROR,
                   " LifeCycle_Service: Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      namingContext_var_ = CosNaming::NamingContext::_narrow (namingObj_var.in ());


      if (CORBA::is_nil (namingContext_var_.in ()))
        ACE_ERROR ((LM_ERROR,
                   "LifeCycle_Service: Unable get the Naming Service.\n"));

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("LifeCycle_Service: Have a proper reference to the Naming Service.\n")));

      CosNaming::Name life_Cycle_Service_Name (1);
      life_Cycle_Service_Name.length (1);
      life_Cycle_Service_Name[0].id = CORBA::string_dup ("Life_Cycle_Service");

      CORBA::Object_ptr tmp = this->life_Cycle_Service_i_ptr_->_this();

      namingContext_var_->bind (life_Cycle_Service_Name,
                                tmp);

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("LifeCycle_Service: Bound the LifeCycle Service to the Naming Context.\n")));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Life_Cycle_Service_Server::init");
    }

  return 0;
}


int
Life_Cycle_Service_Server::run (void)
{
  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("\nLifeCycle Service: Life_Cycle_Service_Server is running\n")));

  orb_manager_.orb()->run ();

  return 0;
}


// Function get_options.

u_int
Life_Cycle_Service_Server::parse_args (int argc,
                                       ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("?d:"));
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
                    argv[0], char(opt)));
      case '?':
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("usage:  %s")
                    ACE_TEXT(" [-d] <debug level> - Set the debug level\n")
                    ACE_TEXT(" [-?]               - Prints this message\n")
                    ACE_TEXT("\n"),
                    argv[0]));
        ACE_OS::exit (exit_code);
        break;
      }
  return 0;
}

// function main

int
ACE_TMAIN (int argc, ACE_TCHAR* argv [])
{
  Life_Cycle_Service_Server life_Cycle_Service_Server;

  try
    {
      int check = life_Cycle_Service_Server.init (argc,
                                                  argv);

      if (check)
        return 1;
      else
        {
          life_Cycle_Service_Server.run ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("LifeCycleService::main");
      return -1;
    }
  return 0;
}
