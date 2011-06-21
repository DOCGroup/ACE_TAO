
//=============================================================================
/**
 *  @file    Factory_Finder.cpp
 *
 *  $Id$
 *
 *  A Factory Finder for the Quoter example. This example conforms
 *  to the CosLifeCycle Factory Finder notion.
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "Factory_Finder.h"

Quoter_Factory_Finder_Server::Quoter_Factory_Finder_Server (void)
:   debug_level_ (1)
{
  // Nothing
}

Quoter_Factory_Finder_Server::~Quoter_Factory_Finder_Server (void)
{
  try
    {
      // Unbind the Quoter Factory Finder.
      CosNaming::Name factory_Finder_Name (2);
      factory_Finder_Name.length (2);
      factory_Finder_Name[0].id = CORBA::string_dup ("IDL_Quoter");
      factory_Finder_Name[1].id = CORBA::string_dup ("Quoter_Factory_Finder");
      if (!CORBA::is_nil (this->quoterNamingContext_var_.in ()))
        this->quoterNamingContext_var_->unbind (factory_Finder_Name);
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "Could not unbind the Factor Finder from the Name Service\n"));
      ex._tao_print_exception ("~Quoter_Factor_Finder_Server");
    }
}

int
Quoter_Factory_Finder_Server::init (int argc,
                                    ACE_TCHAR *argv[])
{
  const char *exception_message = "Null Message";

  try
    {
      exception_message = "While ORB_Manager::init";
      int initvalue = this->orb_manager_.init (argc,
                                               argv);

      if (initvalue == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "init"),
                          -1);

      // Activate the POA manager
      exception_message = "While activating the POA manager";
      int result = this->orb_manager_.activate_poa_manager ();

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
        this->orb_manager_.activate (this->quoter_Factory_Finder_i_ptr_);

      // Print the IOR.
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Factory Finder: IOR is: <%C>\n", str.in ()));

      // Register the Quoter Factory Finder with the Naming Service
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,"Factory Finder: Trying to get a reference to the Naming Service.\n"));

      // Get the Naming Service object reference.
      exception_message = "While resolving the Name Service";
      CORBA::Object_var namingObj_var =
        orb_manager_.orb()->resolve_initial_references ("NameService");

      if (CORBA::is_nil (namingObj_var.in ()))
        ACE_ERROR ((LM_ERROR,
                    " (%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      exception_message = "While narrowing the Naming Context";
      CosNaming::NamingContext_var namingContext_var =
        CosNaming::NamingContext::_narrow (namingObj_var.in ());

      // Get the IDL_Quoter naming context.
      CosNaming::Name quoterContextName (1);  // max = 1
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      exception_message = "While resolving the Quoter Naming Context";
      CORBA::Object_var quoterNamingObj_var =
        namingContext_var->resolve (quoterContextName);

      exception_message = "While narrowing the Quoter Naming Context";
      quoterNamingContext_var_ =
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in ());

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "Factory Finder: Have a proper reference to the Quoter Naming Context.\n"));

      // Bind the QuoterFactory Finder to the IDL_Quoter naming
      // context.
      CosNaming::Name quoter_Factory_Finder_Name_ (1);
      quoter_Factory_Finder_Name_.length (1);
      quoter_Factory_Finder_Name_[0].id = CORBA::string_dup ("Quoter_Factory_Finder");

      exception_message = "Factory_Factory::_this";
      CORBA::Object_var ff_obj = this->quoter_Factory_Finder_i_ptr_->_this();

      exception_message = "While binding the Factory Finder";
      quoterNamingContext_var_->bind (quoter_Factory_Finder_Name_,
                                      ff_obj.in ());

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    "Factory_Finder: Bound the Quoter Factory Finder to the Quoter Naming Context.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Factor_Finder_Server::init - %C\n", exception_message));
      ex._tao_print_exception ("SYS_EX");
      return -1;
    }

  return 0;
}

int
Quoter_Factory_Finder_Server::run (void)
{
  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "\nQuoter Example: Quoter_Factory_Finder_Server is running\n"));

  orb_manager_.orb()->run ();

  return 0;
}


// Function get_options.

u_int
Quoter_Factory_Finder_Server::parse_args (void)
{
  ACE_Get_Opt get_opt (this->argc_, this->argv_, ACE_TEXT("?d:"));
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Quoter_Factory_Finder_Server quoter_Factory_Finder_Server;

  try
    {
      int result = quoter_Factory_Finder_Server.init (argc, argv);

      if (result == -1)
        return 1;
      else
        {
          quoter_Factory_Finder_Server.run ();
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
