// $Id$

// ============================================================================
//
// = FILENAME
//    QuoterFactoryFinder.cpp
//
// = DESCRIPTION
//    A Factory Finder for the Quoter example. This example conforms
//    to the CosLifeCycle Factory Finder notion.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "QuoterFactoryFinder.h"

static const char usage [] = 
"[-? |\n"
"            [-O[RBport] ORB port number]]";
// @@ The string above looks odd.  Do you really need it to look like
// this?!

// Constructor
QuoterFactoryFinder_i::QuoterFactoryFinder_i (void)
{
  TAO_TRY 
    {
      // Get a reference to the ORB.
      CORBA::ORB_var orb_var = TAO_ORB_Core_instance ()->orb ();
      TAO_CHECK_ENV;

      // Get the Naming Service object reference.
      CORBA::Object_var namingObj_var = 
        orb_var->resolve_initial_references ("NameService");
      TAO_CHECK_ENV;

      if (CORBA::is_nil (namingObj_var.in ())) 
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      CosNaming::NamingContext_var namingContext_var =
        CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

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
       
      // Bind the QuoterFactory Finder to the IDL_Quoter naming
      // context.

      CosNaming::Name quoterFactoryFinderName_ (1);
      quoterFactoryFinderName_.length (1);
      quoterFactoryFinderName_[0].id = CORBA::string_dup ("QuoterFactoryFinder");

      this->quoterNamingContext_var_->bind (quoterFactoryFinderName_,
                                            (CORBA::Object *) this,
                                            TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;
}

// Destructor.
QuoterFactoryFinder_i::~QuoterFactoryFinder_i (void)
{
}


CosLifeCycle::Factories *
QuoterFactoryFinder_i::find_factories (const CosLifeCycle::Key &factory_key,
                                       CORBA::Environment &_env_there)
{
  CORBA::Environment env_here;

  // fill in the name of the Quoter Factory
  // CosNaming::Name factoryName (1);  // max = 1 
  // factoryName.length (1);
  // factoryName[0].id = CORBA::string_dup ("quoter_factory");
  // or
  CosNaming::Name factoryName = (CosNaming::Name) factory_key;

  // Try to get a reference to a Quoter Factory
  CORBA::Object_var quoterObject_var =  
    quoterNamingContext_var_->resolve (factoryName, env_here);
  
  // See if there is an exception, if yes then throw the NoFactory
  // exception.
  if (env_here.exception () != 0) 
    {
      // Throw a NoFactory exception.
      _env_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }

  // Check if it is a valid Quoter Factory reference.
  if (CORBA::is_nil (quoterObject_var.in ())) 
    {
      // throw a NoFactory exception.
      _env_there.exception (new CosLifeCycle::NoFactory (factory_key));      
      return 0;
    }

  // Were able to get a reference to Quoter Factory.
  else 
    {
      // Create a sequence of factories object.
      CosLifeCycle::Factories *factories_ptr = new
        CosLifeCycle::Factories (1);      
      // Using the Naming Service only one reference is available.
      factories_ptr->length (1);      

      // Dereference the Object pointer.
      (*factories_ptr)[0] = quoterObject_var;

      ACE_DEBUG ((LM_DEBUG,
                  "Have reference to a Quoter Factory.\n"));
      return factories_ptr;
    }
}

// Function get_options.

static u_int
get_options (int argc, char *argv [])
{
  // We need the 'O' in get_opt () because we also want to have ORB
  // parameters, they all start with 'O'.
  ACE_Get_Opt get_opt (argc, argv, "O?");
  int opt;

  while ((opt = get_opt ()) != EOF)
    switch (opt) 
      {
      case '?':
        ACE_DEBUG ((LM_DEBUG,
                    "Usage: %s %s\n",
                    argv[0], usage));
        ACE_OS::exit (0);
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: unknown arg, -%c\n"
                           "Usage: %s %s\n",
                           argv[0], char (opt),
                           argv[0],
                           usage),
                          1);
      }

  if (argc != get_opt.optind)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s: too many arguments\n"
                       "Usage: %s %s\n",
                       argv[0],
                       argv[0],
                       usage),
                      1);
  return 0;
}

// function main

int
main (int argc, char *argv [])
{
  TAO_TRY
    {
      // Initialize ORB.

      // @@ Can you please replace all this with the TAO_ORB_Manager?
      CORBA::ORB_ptr orb_ptr = CORBA::ORB_init (argc,
                                                argv,
                                                "internet",
                                                TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Connect to the RootPOA.
      CORBA::Object_var poa_object =
        orb_ptr->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);

      TAO_CHECK_ENV;

      // Get the Options.
      if (get_options (argc, argv))
        ACE_OS::exit (-1);

      // Instantiate the QuoterFactoryFinder.
      QuoterFactoryFinder_i *quoterFactoryFinder_i_ptr_;
      ACE_NEW_RETURN (quoterFactoryFinder_i_ptr_,
                      QuoterFactoryFinder_i (),
                      -1);

  
      ACE_DEBUG ((LM_DEBUG,
                  "Quoter Factory Finder is instantiated.\n"));

      // The POA Manager has to be activated before starting the ORB
      // event loop.
      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Run the ORB.
      if (orb_ptr->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "CORBA::ORB::run"),
                          -1);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;

  return 0;
}
