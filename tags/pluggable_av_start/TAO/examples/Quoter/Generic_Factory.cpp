// $Id$

// ============================================================================
//
// = FILENAME
//    Generic_Factory.cpp
//
// = DESCRIPTION
//    The server for the GenericFactory for the quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "tao/corba.h"
#include "orbsvcs/LifeCycleServiceC.h"
#include "Generic_Factory.h"

ACE_RCSID(Quoter, Generic_Factory, "$Id$")

static const char usage [] = "[-? |\n[-O[RBport] ORB port number]]";

Quoter_Generic_Factory_Server::Quoter_Generic_Factory_Server (void)
: use_LifeCycle_Service_ (0)
{
}

Quoter_Generic_Factory_Server::~Quoter_Generic_Factory_Server (void)
{
  ACE_TRY_NEW_ENV
    {
      // Unbind the Quoter Factory Finder.
      CosNaming::Name generic_Factory_Name (2);
      generic_Factory_Name.length (2);
      generic_Factory_Name[0].id = CORBA::string_dup ("IDL_Quoter");
      generic_Factory_Name[1].id = CORBA::string_dup ("Quoter_Generic_Factory");
      if (this->quoterNamingContext_var_.ptr () != 0)
        this->quoterNamingContext_var_->unbind (generic_Factory_Name,ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      ACE_TRY_ENV.print_exception ("System Exception");
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      ACE_TRY_ENV.print_exception ("User Exception");
    }
  ACE_ENDTRY;
}

int
Quoter_Generic_Factory_Server::init (int argc,
                                     char *argv[],
                                     CORBA::Environment &ACE_TRY_ENV)
{
  if (this->orb_manager_.init (argc,
                               argv,
                               ACE_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init"),
                      -1);

  // Copy them, because parse_args expects them there.
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();


  ACE_NEW_RETURN (this->quoter_Generic_Factory_i_ptr_,
                  Quoter_Generic_Factory_i(),
                  -1);

  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (this->quoter_Generic_Factory_i_ptr_,
                                 ACE_TRY_ENV);

  // Failure while activating the Quoter Factory Finder object
  if (ACE_TRY_ENV.exception () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init: Failure while activating the Quoter Generic Factory Impl.\n"),
                      -1);


  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  // Register the Quoter GenericFactory with the Naming Service.
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Trying to get a reference to the Naming Service.\n"));

      // Get the Naming Service object reference.
      CORBA::Object_var namingObj_var =
        orb_manager_.orb()->resolve_initial_references ("NameService");
      ACE_TRY_CHECK;

      if (CORBA::is_nil (namingObj_var.in ()))
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      CosNaming::NamingContext_var namingContext_var =
        CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                           ACE_TRY_ENV);

      if (CORBA::is_nil (namingContext_var.in ()))
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));


      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG,
                  "Have a proper reference to the Naming Service.\n"));

      // Get the IDL_Quoter naming context.
      CosNaming::Name quoterContextName (1);  // max = 1
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      CORBA::Object_var quoterNamingObj_var =
        namingContext_var->resolve (quoterContextName,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      quoterNamingContext_var_ =
        CosNaming::NamingContext::_narrow (quoterNamingObj_var.in (),
                                           ACE_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG,
                  "Have a proper reference to the Quoter Naming Context.\n"));

      // Bind the Quoter GenericFactory to the IDL_Quoter naming
      // context.
      CosNaming::Name quoter_Generic_Factory_Name (1);
      quoter_Generic_Factory_Name.length (1);
      quoter_Generic_Factory_Name[0].id = CORBA::string_dup ("Quoter_Generic_Factory");

      quoterNamingContext_var_->bind (quoter_Generic_Factory_Name,
                                      this->quoter_Generic_Factory_i_ptr_->_this(ACE_TRY_ENV),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG,
                  "Bound the Quoter GenericFactory to the Quoter Naming Context.\n"));

      // now the Quoter GenericFactory is bound to the Naming Context
      // the Generic Factory should try to register itself to the closest
      // Life Cycle Service is order to be called.

      /* for now as long as the trading service is not ported to NT we skip this */

      if (this->use_LifeCycle_Service_)
      {
        // get the Quoter_Life_Cycle_Service
        CosNaming::Name life_Cycle_Service_Name (1);
        life_Cycle_Service_Name.length (1);
        life_Cycle_Service_Name[0].id = CORBA::string_dup ("Life_Cycle_Service");

        CORBA::Object_var life_Cycle_Service_Obj_var =
          namingContext_var->resolve (life_Cycle_Service_Name,
                                      ACE_TRY_ENV);
        ACE_TRY_CHECK;

        LifeCycleService::Life_Cycle_Service_var  life_Cycle_Service_var =
          LifeCycleService::Life_Cycle_Service::_narrow (life_Cycle_Service_Obj_var.in (),
                                                         ACE_TRY_ENV);
        ACE_TRY_CHECK;

        ACE_DEBUG ((LM_DEBUG, "Have a proper reference to Life Cycle Service.\n"));

        CORBA::Object_var object_var = this->quoter_Generic_Factory_i_ptr_->_this(ACE_TRY_ENV);

        life_Cycle_Service_var->register_factory ("Quoter_Generic_Factory",  // name
                                                  "Bryan 503",               // location
                                                  "Generic Factory",         // description
                                                  object_var.in (),
                                                  ACE_TRY_ENV);
        ACE_TRY_CHECK;
        ACE_DEBUG ((LM_DEBUG,
                    "Registered the Quoter GenericFactory to the Life Cycle Service.\n"));
      }
            
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("Quoter_Generic_Factory_Server::init: Exception");
    }
  ACE_ENDTRY;


  return 0;
}

int
Quoter_Generic_Factory_Server::run (CORBA::Environment& env)
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
Quoter_Generic_Factory_Server::parse_args (void)
{
  // We need the 'O' in get_opt () because we also want to have ORB
  // parameters, they all start with 'O'.
  ACE_Get_Opt get_opt (this->argc_, this->argv_, "Ol?");
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
      case 'l':
        this->use_LifeCycle_Service_ = 1;
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
  Quoter_Generic_Factory_Server quoter_Generic_Factory_Server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Quoter: Quoter_Generic_Factory_Server \n\n"));
  ACE_TRY_NEW_ENV
    {
      if (quoter_Generic_Factory_Server.init (argc,
                                              argv,
                                              ACE_TRY_ENV) == -1)
        return 1;
      else
        {
          quoter_Generic_Factory_Server.run (ACE_TRY_ENV);
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
