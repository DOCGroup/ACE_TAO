
//=============================================================================
/**
 *  @file    LifeCycle_Service.cpp
 *
 *  The server for the LifeCycleService of the quoter example.
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================

#include "orbsvcs/Log_Macros.h"
#include "LifeCycle_Service.h"

#include "ace/Argv_Type_Converter.h"
#include "ace/OS_main.h"

Life_Cycle_Service_Server::~Life_Cycle_Service_Server ()
{
  try
    {
      // Unbind the Factory Finder.
      CosNaming::Name generic_factory_name (2);
      generic_factory_name.length (2);
      generic_factory_name[0].id = CORBA::string_dup ("LifeCycle_Service");
      this->namingContext_var_->unbind (generic_factory_name);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("User Exception");
    }
}

int
Life_Cycle_Service_Server::init (int argc,
                                 ACE_TCHAR * argv[])
{
  int retval {};

  // Copy command line parameter.
  ACE_Argv_Type_Converter command(argc, argv);

  retval = this->orb_manager_.init (command.get_argc (),
                                    command.get_TCHAR_argv ());

  if (retval == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("%p\n"),
                       ACE_TEXT("init")),
                      -1);

  // Activate the POA manager
  retval = this->orb_manager_.activate_poa_manager ();

  if (retval == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR, "%p\n", "activate_poa_manager"), -1);

  this->parse_args (command.get_argc (), command.get_TCHAR_argv ());

  Life_Cycle_Service_i *servant {};
  ACE_NEW_RETURN (servant,
                  Life_Cycle_Service_i (this->debug_level_),
                  -1);
  PortableServer::ServantBase_var servant_owner (servant);

  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (servant);

  if (this->debug_level_ >= 2)
    ORBSVCS_DEBUG ((LM_DEBUG, "LifeCycle_Service: IOR is: <%C>\n", str.in ()));

  // Register the LifeCycle Service with the Naming Service.
  try
    {
      if (this->debug_level_ >= 2)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT("LifeCycle_Service: Trying to get a reference to the Naming Service.\n")));

      // Get the Naming Service object reference.
      CORBA::Object_var naming_obj_var =
        this->orb_manager_.orb ()->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj_var.in ()))
        ORBSVCS_ERROR ((LM_ERROR,
                   " LifeCycle_Service: Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      this->namingContext_var_ = CosNaming::NamingContext::_narrow (naming_obj_var.in ());

      if (CORBA::is_nil (this->namingContext_var_.in ()))
        ORBSVCS_ERROR ((LM_ERROR,
                   "LifeCycle_Service: Unable get the Naming Service.\n"));

      if (this->debug_level_ >= 2)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT("LifeCycle_Service: Have a proper reference to the Naming Service.\n")));

      CosNaming::Name life_cycle_service_name (1);
      life_cycle_service_name.length (1);
      life_cycle_service_name[0].id = CORBA::string_dup ("Life_Cycle_Service");

      CORBA::Object_var const tmp = servant->_this ();

      this->namingContext_var_->bind (life_cycle_service_name,
                                      tmp.in ());

      if (this->debug_level_ >= 2)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT("LifeCycle_Service: Bound the LifeCycle Service to the Naming Context.\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Life_Cycle_Service_Server::init");
    }

  return 0;
}

int
Life_Cycle_Service_Server::run ()
{
  if (this->debug_level_ >= 1)
    ORBSVCS_DEBUG ((LM_DEBUG,
                ACE_TEXT("\nLifeCycle Service: Life_Cycle_Service_Server is running\n")));

  this->orb_manager_.orb ()->run ();

  return 0;
}

u_int
Life_Cycle_Service_Server::parse_args (int argc,
                                       ACE_TCHAR * argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("?d:"));
  int opt {};
  int exit_code {};

  while ((opt = get_opt ()) != EOF)
    switch (opt)
      {
      case 'd':  // debug flag.
        this->debug_level_ = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        exit_code = 1;
        ORBSVCS_ERROR ((LM_ERROR,
                    "%s: unknown arg, -%c\n",
                    argv[0], char(opt)));
        ACE_FALLTHROUGH;
      case '?':
        ORBSVCS_DEBUG ((LM_DEBUG,
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

int
ACE_TMAIN (int argc, ACE_TCHAR * argv [])
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
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("LifeCycleService::main");
      return -1;
    }
  return 0;
}
