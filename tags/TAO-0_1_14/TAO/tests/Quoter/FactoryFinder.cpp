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
#include "FactoryFinder.h"


static const char usage [] = 
"[-? |\n[-O[RBport] ORB port number]]";

QuoterFactoryFinder_Server::QuoterFactoryFinder_Server () 
{
}

QuoterFactoryFinder_Server::~QuoterFactoryFinder_Server () 
{
  TAO_TRY
    {
      // Unbind the Quoter Factory Finder.
      CosNaming::Name factory_name (2);
      factory_name.length (2);
      factory_name[0].id = CORBA::string_dup ("IDL_Quoter");
      factory_name[1].id = CORBA::string_dup ("QuoterFactoryFinder");
      this->quoterNamingContext_var_->unbind (factory_name,TAO_TRY_ENV);
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

QuoterFactoryFinder_Server::init (int argc, char *argv[], CORBA::Environment& env)
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


  ACE_NEW_RETURN (this->quoterFactoryFinderImpl_ptr_,
                  QuoterFactoryFinderImpl(),
                  -1);

  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (this->quoterFactoryFinderImpl_ptr_,
                                 env);

  // Failure while activating the Quoter Factory Finder object
  if (str == 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "init: Failure while activating the Quoter Factory Finder Impl.\n"),
                      -1);


  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  // Register the Quoter Factory Finder with the Naming Service
  TAO_TRY 
    {
      ACE_DEBUG ((LM_DEBUG,"Trying to get a reference to the Naming Service.\n"));

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
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,"Have a proper reference to the Naming Service.\n"));

      // Get the IDL_Quoter naming context.
      CosNaming::Name quoterContextName (1);  // max = 1 
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      CORBA::Object_var quoterNamingObj_var = 
        namingContext_var->resolve (quoterContextName, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      quoterNamingContext_var_ = 
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in (),
                                           TAO_TRY_ENV);
       
      ACE_DEBUG ((LM_DEBUG,"Have a proper reference to the Quoter Naming Context.\n"));

      // Bind the QuoterFactory Finder to the IDL_Quoter naming
      // context.
      CosNaming::Name quoterFactoryFinderName_ (1);
      quoterFactoryFinderName_.length (1);
      quoterFactoryFinderName_[0].id = CORBA::string_dup ("QuoterFactoryFinder");

      quoterNamingContext_var_->bind (quoterFactoryFinderName_,
                                            this->quoterFactoryFinderImpl_ptr_->_this(TAO_TRY_ENV),
                                            TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,"Bound the Quoter Factory Finder to the Quoter Naming Context.\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;
  

  return 0;
}

int
QuoterFactoryFinder_Server::run (CORBA::Environment& env)
{
  if (orb_manager_.orb()->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "run"),
                      -1);
  return 0;
}


// Function get_options.

unsigned int
QuoterFactoryFinder_Server::parse_args ()
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
  QuoterFactoryFinder_Server quoterFactoryFinder_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Quoter:QuoterFactoryFinder \n \n"));
  TAO_TRY
    {
      if (quoterFactoryFinder_server.init (argc,argv,TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          quoterFactoryFinder_server.run (TAO_TRY_ENV);
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
