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

static const char usage [] = "[-? |\n[-O[RBport] ORB port number]]";

Quoter_Factory_Finder_Server::Quoter_Factory_Finder_Server (void)
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
      this->quoterNamingContext_var_->unbind (factory_Finder_Name, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Could not unbind the Factor Finder from the Name Service\n"));
      ACE_TRY_ENV.print_exception ("~Quoter_Factor_Finder_Server");
    }
  ACE_ENDTRY;
}

int
Quoter_Factory_Finder_Server::init (int argc,
                                    char *argv[],
                                    CORBA::Environment &ACE_TRY_ENV)
{
  const char *exception_message = "Null Message";
  
  ACE_TRY
    {
      exception_message = "While ORB_Manager::init";
      if (this->orb_manager_.init (argc,
                                   argv,
                                   ACE_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "init"),
                          -1);
      ACE_TRY_CHECK;

      // Copy them, because parse_args expects them there.
      this->argc_ = argc;
      this->argv_ = argv;

      this->parse_args ();


      ACE_NEW_RETURN (this->quoter_Factory_Finder_i_ptr_,
                      Quoter_Factory_Finder_i(),
                      -1);

      // Activate the object.
      exception_message = "Failure while activating the Quoter Factory Finder Impl";
      CORBA::String_var str  =
        this->orb_manager_.activate (this->quoter_Factory_Finder_i_ptr_,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "The IOR is: <%s>\n",
                  str.in ()));

      // Register the Quoter Factory Finder with the Naming Service

      ACE_DEBUG ((LM_DEBUG,"Trying to get a reference to the Naming Service.\n"));

      // Get the Naming Service object reference.
      exception_message = "While resolving the Name Service";
      CORBA::Object_var namingObj_var =
        orb_manager_.orb()->resolve_initial_references ("NameService");
      ACE_TRY_CHECK;

      if (CORBA::is_nil (namingObj_var.in ()))
        ACE_ERROR ((LM_ERROR,
                    " (%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      exception_message = "While narrowing the Naming Context";
      CosNaming::NamingContext_var namingContext_var =
        CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the IDL_Quoter naming context.
      CosNaming::Name quoterContextName (1);  // max = 1
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      exception_message = "While resolving the Quoter Naming Context";
      CORBA::Object_var quoterNamingObj_var =
        namingContext_var->resolve (quoterContextName, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      exception_message = "While narrowing the Quoter Naming Context";
      quoterNamingContext_var_ =
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in (),
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Have a proper reference to the Quoter Naming Context.\n"));

      // Bind the QuoterFactory Finder to the IDL_Quoter naming
      // context.
      CosNaming::Name quoter_Factory_Finder_Name_ (1);
      quoter_Factory_Finder_Name_.length (1);
      quoter_Factory_Finder_Name_[0].id = CORBA::string_dup ("Quoter_Factory_Finder");

      exception_message = "While binding the Factory Finder";
      quoterNamingContext_var_->bind (quoter_Factory_Finder_Name_,
                                      this->quoter_Factory_Finder_i_ptr_->_this(ACE_TRY_ENV),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Bound the Quoter Factory Finder to the Quoter Naming Context.\n"));
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Factor_Finder_Server::init - %s\n", exception_message));
      ACE_TRY_ENV.print_exception ("SYS_EX");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Quoter_Factory_Finder_Server::run (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  if (orb_manager_.orb()->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "run"),
                      -1);
  return 0;
}


// Function get_options.

u_int
Quoter_Factory_Finder_Server::parse_args (void)
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
  Quoter_Factory_Finder_Server quoter_Factory_Finder_Server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Quoter:Quoter_Factory_Finder \n \n"));
  ACE_TRY_NEW_ENV
    {
      if (quoter_Factory_Finder_Server.init (argc,argv,ACE_TRY_ENV) == -1)
        return 1;
      else
        {
          quoter_Factory_Finder_Server.run (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      ACE_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      ACE_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}
